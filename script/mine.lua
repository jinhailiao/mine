--[[
***************************************************************************
* Copyright (C) 2016, Haisoft有限公司
* All rights reserved.

* 文件名称: mine.lua
* 摘    要：仿WINDOWS中的扫雷的游戏

* 当前版本: 2.0
* 作    者: jinhailiao@163.com,QQ6033653
* 完成日期: 2016.11.1
***************************************************************************
--]]

---------------------------[[ const define ]]----------------------------
GRIDWIDTH = 17

--游戏状态
GAMESTART=1
GAMEOVER=2
--鼠标状态
MOUSE_STATE_CLICK=1
MOUSE_STATE_DBCLICK=2
--扫雷区域 {x,y,w}
FACE_PLATE =
{
	{x=106, y=150, w=183},
	{x=46,  y=105, w=291},
	{x=10,  y=75,  w=363}
}
--游戏数据
GRID_LINE = {10, 16, 20}
MINE_NUM = {15, 40, 80}
LEVEL_NAME = {"初级", "中级", "高级"}

--[[状态转换：
单击，MS_INIT->MS_MARK;MS_MARK->MS_DOUBT;MS_DOUBT->MS_INIT
双击，MS_MARK->MS_CLEAR;MS_DOUBT->MS_INIT
--]]
MS_INIT = 1
MS_CLEAR = 2
MS_MARK = 3
MS_DOUBT = 4
--以下不参与状态转换
MS_BOMB = 5
MS_SHOW = 6
MS_ERROR = 7

---------------------------[[ global define ]]----------------------------
g =
{
	GameLevel = 1,
	--[[ 雷区的数据结构
	{
    	mine;            -1：有雷； 0-8：周围八格雷的情况
    	status;          MS_INIT:初始化;MS_CLEAR:已排雷;MS_MARK:标雷;MS_DOUBT:怀疑;MS_BOMB:踩雷;MS_SHOW:显示雷;MS_ERROR:标雷错误
	};
	--]]
	GameBoard = {}, --雷区数据
	GameState = GAMESTART,
	GameTime = 0, --游戏用时
	TimeStart = 0, --计时开始

	Player = {name = "unknow", score = {999,999,999}},
	Players = {},

	MineRestNum = 0,               -- 剩余雷的数目
}

function GameInit()
	GuiInit()
	DataInit()
	PlayerInit()
end

function GameUpdate()
	if g.GameState == GAMESTART then
		HandleGameTime()
		HandleMouseEvent()
	end
end

function GameDraw()
	local boardX = FACE_PLATE[g.GameLevel].x
	local boardY = FACE_PLATE[g.GameLevel].y
	local boardW = FACE_PLATE[g.GameLevel].w

	mine.DrawBoxUp(1, 1, 382, 446);
	mine.DrawBoxDn(10, 10, 363, 54);
	mine.DrawBoxDn(boardX, boardY, boardW, boardW);

	DrawGui()
	DrawGrid(boardX+2, boardY+2, GRID_LINE[g.GameLevel]);
	ShowMineArea();
end

--[[ 按键响应函数
--]]
function OnButtonPlayer()
	local NewName = mine.EditBox("输入姓名", g.Player.name)
	if NewName ~= g.Player.name then
		g.Player = {name = NewName, score = {999,999,999}}
		local n = SearchPlayer()
		if n == 0 then
			table.insert(g.Players, g.Player)
			AddPlayerAndSort()
			PlayerInfoSave()
		else
			g.Player = g.Players[n]
		end
	end
	mine.MouseState() -- 清理消息
end

function OnButtonGrade()
	mine.MouseState() -- 清理消息
	g.GameLevel = g.GameLevel + 1
	if g.GameLevel > 3 then
		g.GameLevel = 1
	end
	DataInit()
end

function OnButtonHero()
	mine.MsgBox("英雄榜", "级别   姓名    成绩\n初级   unknow   999秒\n中级   unknow   999秒\n高级   unknow   999秒")
	mine.MouseState() -- 清理消息
end

function OnButtonReset()
	mine.MouseState() -- 清理消息
	DataInit()
end

function OnButtonAbout()
	mine.MsgBox("关于","探险 v1.0                     \nCopyright(c)2017 Haisoft工作室\nAll rights reserved.          \nE-Mail:jinhailiao@163.com     ")
	mine.MouseState() -- 清理消息
end

--[[ 数据初始化函数
--]]
-- 创建GUI控件
function GuiInit()
	mine.CreateButton("玩家", 14, 24, 40, 24, "OnButtonPlayer")
	mine.CreateButton("级别", 138, 14, 40, 24, "OnButtonGrade");
	mine.CreateButton("英雄榜", 226, 14, 60, 24, "OnButtonHero");
	mine.CreateButton("重置", 288, 14, 40, 24, "OnButtonReset");
	mine.CreateButton("关于", 330, 14, 40, 24, "OnButtonAbout");
end

-- 初始化为level级别的数据
function DataInit()
	local gridnum = GRID_LINE[g.GameLevel]				-- 根据level决定行列格子数和雷数
	g.MineRestNum = MINE_NUM[g.GameLevel]

	g.GameTime  = 0
	g.TimeStart = os.time()
	g.GameState = GAMESTART

	g.GameBoard = {}
	for i = 1,gridnum do        -- 方格的初始各值
		g.GameBoard[i] = {}
		for j = 1,gridnum do
        	g.GameBoard[i][j] = {status = MS_INIT, mine = 0}
        end
	end

	local m = 0
	math.randomseed(os.time())
	while m < MINE_NUM[g.GameLevel] do
		local i = math.random(gridnum)
		local j = math.random(gridnum)
		if g.GameBoard[i][j].mine ~= -1 then
			g.GameBoard[i][j].mine = -1;
			m = m + 1;
		end
	end

	for i = 1,gridnum do        -- 无雷格记录周围的八个格子的雷数
		for j = 1,gridnum do
			if g.GameBoard[i][j].mine ~= -1 then
				CountMineOn9Grid(i, j, gridnum)
			end
        end
	end
	return
end

--统计周围8格的雷的数量
function CountMineOn9Grid(i, j, gridnum)
	for m = i-1,i+1 do
		if m>0 and m<=gridnum then
			for n = j-1,j+1 do
				if n>0 and n<=gridnum then
					if g.GameBoard[m][n].mine == -1 then
						g.GameBoard[i][j].mine = g.GameBoard[i][j].mine + 1
					end
				end
			end
		end
	end
end

--[[ 数据刷新函数集合
--]]
function HandleGameTime()
	if g.TimeStart == 0 then
		return
	end

	local TimeNow = os.time()
	if g.TimeStart == TimeNow then
		return
	end

	g.TimeStart = TimeNow
	g.GameTime = g.GameTime + 1
	if g.GameTime > 999 then
		g.GameTime = 999
	end
end

function CountMine(val)
	g.MineRestNum = g.MineRestNum + val
end

-- 检查游戏是否结束
function CheckOver()
	local gridnum = GRID_LINE[g.GameLevel]
	for i = 1,gridnum do
		for j = 1,gridnum do
        	if g.GameBoard[i][j].status == MS_INIT or g.GameBoard[i][j].status == MS_DOUBT then -- 未完成
				return false,false -- over,win
			end
        	if g.GameBoard[i][j].status == MS_MARK and g.GameBoard[i][j].mine ~= -1 then -- 标记错误
				return false,false -- over,win
			end
        	if g.GameBoard[i][j].status == MS_BOMB then -- 炸了
				return true,false -- over,win
			end
        end
	end
	return true,true -- over,win
end

function ChangeGridStateAfterOver()
	local gridnum = GRID_LINE[g.GameLevel]
	for i = 1,gridnum do
		for j = 1,gridnum do
        	if g.GameBoard[i][j].status == MS_INIT and g.GameBoard[i][j].mine == -1 then
				g.GameBoard[i][j].status = MS_SHOW
			end
        	if g.GameBoard[i][j].status == MS_MARK and g.GameBoard[i][j].mine >= 0 then
				g.GameBoard[i][j].status = MS_ERROR
			end
        	if g.GameBoard[i][j].status == MS_DOUBT and g.GameBoard[i][j].mine >= 0 then
				g.GameBoard[i][j].status = MS_ERROR
			end
        end
	end
end

function HandleOver(over, win)
	if over == false then
		over, win = CheckOver() -- 判断是否游戏结束
	end

	if over == true then -- 游戏结束处理
		g.TimeStart = 0
		g.GameState = GAMEOVER
	else
		return
	end

	if win == false then
		ChangeGridStateAfterOver()
	else
		if RecordScore() == true then
			AddPlayerAndSort()
			PlayerInfoSave()
		end
	end
end

--根据坐标计算格子
function GetGridWithCoordinate(x, y)
	local startx = FACE_PLATE[g.GameLevel].x
	local starty = FACE_PLATE[g.GameLevel].y
	local i = (x - startx) / (GRIDWIDTH+1) -- 鼠标指向格子的行列下标值
	local j = (y - starty) / (GRIDWIDTH+1)
	i = math.floor(i)
	j = math.floor(j)

	return i+1,j+1 --坐标从1开始
end

-- 如果左击方格的周围八个方格无雷，则全打开，递归。i,j为当前格的坐标
function OpenGrid(i, j)
	if g.GameBoard[i][j].mine ~= 0 then  --  左击的方格值不为0，则返回
		return
	end

	local gridnum = GRID_LINE[g.GameLevel]
	for x = i-1, i+1 do -- 打开周围八格
		if x > 0 and x <= gridnum then
			for y = j-1, j+1 do
				if y > 0 and y <= gridnum then
					if g.GameBoard[x][y].mine ~= 0 then          -- 周围八格有雷的情况
						if g.GameBoard[x][y].status == MS_MARK or g.GameBoard[x][y].status == MS_DOUBT then -- 如标雷则错误，雷数加1
							CountMine(1)
						end
						g.GameBoard[x][y].status = MS_CLEAR
					elseif g.GameBoard[x][y].status ~= MS_CLEAR then --周围八格无雷的情况，且没打开
						if g.GameBoard[x][y].status == MS_MARK or g.GameBoard[x][y].status == MS_DOUBT then -- 如标雷则错误，雷数加1
							CountMine(1)
						end
						g.GameBoard[x][y].status = MS_CLEAR
						OpenGrid(x, y);  --  递归调用
					end
				end
			end
		end
	end
end

-- 双击打开的格子，则剩余的空格打开
function OpenGridWithMS_CLEAR(i, j)
	local gridnum = GRID_LINE[g.GameLevel]
	for x = i-1, i+1 do  --打开周围八格
		if x > 0 and x <= gridnum then
			for y = j-1,j+1 do
				if y > 0 and y <= gridnum then
					if g.GameBoard[x][y].status == MS_INIT then
						if g.GameBoard[x][y].mine == -1 then
							g.GameBoard[x][y].status = MS_BOMB
							return true, false -- over, win
						end
						g.GameBoard[x][y].status = MS_CLEAR
						if g.GameBoard[x][y].mine == 0 then
							OpenGrid(x, y);
						end
					end
				end
			end
		end
	end
	return false, false -- over,win
end

function HandleMouseEvent()
	local state,x,y = mine.MouseState()
	if state == MOUSE_STATE_CLICK then
		HandleMouseClick(x, y)
	elseif state == MOUSE_STATE_DBCLICK then
		HandleMouseDoubleClick(x, y)
	end
end

function HandleMouseClick(x, y)
	local boardX = FACE_PLATE[g.GameLevel].x
	local boardY = FACE_PLATE[g.GameLevel].y
	local boardW = FACE_PLATE[g.GameLevel].w
	if x>boardX and x<boardX+boardW and y>boardY and y<boardY+boardW then --坐标是否在雷区
		local i,j=GetGridWithCoordinate(x,y)
		local status = g.GameBoard[i][j].status
		if status == MS_INIT then
			g.GameBoard[i][j].status = MS_MARK;CountMine(-1)
			HandleOver(false, false)
		elseif status == MS_MARK then
			g.GameBoard[i][j].status = MS_DOUBT
		elseif status == MS_DOUBT then
			g.GameBoard[i][j].status = MS_INIT;CountMine(1)
		end
	end
end

function HandleMouseDoubleClick(x, y)
	local boardX = FACE_PLATE[g.GameLevel].x
	local boardY = FACE_PLATE[g.GameLevel].y
	local boardW = FACE_PLATE[g.GameLevel].w
	if x>boardX and x<boardX+boardW and y>boardY and y<boardY+boardW then --坐标是否在雷区
		local i,j=GetGridWithCoordinate(x,y)
		local status = g.GameBoard[i][j].status
		if status == MS_MARK then --windows系统双击会收到单击消息
			if g.GameBoard[i][j].mine == -1 then
				g.GameBoard[i][j].status = MS_BOMB
				HandleOver(true, false)
			else
				g.GameBoard[i][j].status = MS_CLEAR;CountMine(1)
				OpenGrid(i, j);HandleOver(false, false)
			end
		elseif status == MS_DOUBT then
			g.GameBoard[i][j].status = MS_INIT;CountMine(1)
		elseif status == MS_CLEAR then
			local over,win = OpenGridWithMS_CLEAR(i, j) --打开周围8格未打开的格子
			HandleOver(over, win)
		end
	end
end

--[[ 绘图函数集合
--]]
function DrawGui()
	local iMineRestNum = g.MineRestNum
	if g.MineRestNum < 0 then
		iMineRestNum = 0
	elseif g.MineRestNum > MINE_NUM[g.GameLevel] then
		iMineRestNum = MINE_NUM[g.GameLevel]
	end

	local strMineRest = string.format("%03d", iMineRestNum)
	local strGameTime = string.format("%03d秒", g.GameTime)
	local strScore = string.format("%03d秒", g.Player.score[g.GameLevel])

	mine.DrawRect(56, 24, 80, 24, 0xF0F0F0)
	mine.DrawText(58, 30, g.Player.name)

	mine.DrawRect(180, 16, 44, 20, 0xF0F0F0)
	mine.DrawText(190, 20, LEVEL_NAME[g.GameLevel])

	mine.DrawRect(138, 40, 40, 20, 0xF0F0F0)
	mine.DrawText(146, 44, "成绩")

	mine.DrawRect(180, 40, 60, 20, 0xF0F0F0)
	mine.DrawText(194, 44, strScore)

	mine.DrawRect(262, 40, 40, 20, 0xFF0000)
	mine.DrawText(272, 44, strMineRest)

	mine.DrawRect(318, 40, 40, 20, 0xFF0000)
	mine.DrawText(324, 44, strGameTime)
end

function DrawGrid(x, y, GridCnt)
	for i = 0,GridCnt do
		mine.DrawLine(x-1+i*(GRIDWIDTH+1), y, x-1+i*(GRIDWIDTH+1), y-1+GridCnt*(GRIDWIDTH+1));
	end

	for i = 0,GridCnt do
		mine.DrawLine(x, y-1+i*(GRIDWIDTH+1), x-1+GridCnt*(GRIDWIDTH+1), y-1+i*(GRIDWIDTH+1));
	end
end

function ShowMineArea()
	local gridnum = GRID_LINE[g.GameLevel]				-- 根据level决定行列格子数和雷数

	for i=1,gridnum do
		for j=1,gridnum do
			ShowMine(i, j)
		end
	end
end

function ShowMine(i, j)
	local boardX = FACE_PLATE[g.GameLevel].x
	local boardY = FACE_PLATE[g.GameLevel].y
	local status = g.GameBoard[i][j].status
	local mineNum = g.GameBoard[i][j].mine

--	mine.log("i=",i,",j=",j,",status=",status,",mine=",mineNum)

	i = i-1; j = j-1
	if status == MS_INIT then
		mine.DrawBoxUp(boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH);
	elseif status == MS_CLEAR then
		mine.DrawBoxDn(boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH);
		if mineNum > 0 then
			mine.DrawText(boardX+8+i*(GRIDWIDTH+1), boardY+4+j*(GRIDWIDTH+1), mineNum);
		end
	elseif status == MS_MARK then
		mine.DrawBoxUp(boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH);
		mine.DrawFlag(boardX+4+i*(GRIDWIDTH+1), boardY+4+j*(GRIDWIDTH+1), GRIDWIDTH-7);
	elseif status == MS_DOUBT then
		mine.DrawBoxUp(boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH);
		mine.DrawFlag(boardX+4+i*(GRIDWIDTH+1), boardY+4+j*(GRIDWIDTH+1), GRIDWIDTH-7);
		mine.DrawText(boardX+8+i*(GRIDWIDTH+1), boardY+4+j*(GRIDWIDTH+1), "?");
	elseif status == MS_SHOW then
		mine.DrawBoxDn(boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH);
		mine.DrawMine(boardX+4+i*(GRIDWIDTH+1), boardY+4+j*(GRIDWIDTH+1), false);
	elseif status == MS_ERROR then
		mine.DrawBoxUp(boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH);
		mine.DrawFlag(boardX+4+i*(GRIDWIDTH+1), boardY+4+j*(GRIDWIDTH+1), GRIDWIDTH-7);
		mine.DrawText(boardX+8+i*(GRIDWIDTH+1), boardY+4+j*(GRIDWIDTH+1), "X");
	elseif status == MS_BOMB then
		mine.DrawBoxDn(boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH);
		mine.DrawMine(boardX+4+i*(GRIDWIDTH+1), boardY+4+j*(GRIDWIDTH+1), true);
	end
end

--[[ 游戏玩家信息处理
--]]
GAME_PLAYER_BANK="../../script/player.lua"
function PlayerInit()
--	dofile(GAME_PLAYER_BANK)
end

function RecordScore()
	if g.GameTime > g.Player.score[g.GameLevel] then
		return false
	end
	g.Player.score[g.GameLevel] = g.GameTime
	return true
end

function SearchPlayer()
	local n = 0
	for k = 1,table.getn(g.Players) do
		if g.Players[k].name == g.Player.name then
			n = k
			break;
		end
	end

	return n
end

function AddPlayerAndSort()
	local n = SearchPlayer()
	if n == 0 then
		table.insert(g.Players, g.Player)
	else
		if g.Players[n].score[1] > g.Player.score[1] then
			g.Players[n].score[1] = g.Player.score[1]
		end
		if g.Players[n].score[2] > g.Player.score[2] then
			g.Players[n].score[2] = g.Player.score[2]
		end
		if g.Players[n].score[3] > g.Player.score[3] then
			g.Players[n].score[3] = g.Player.score[3]
		end
	end

	table.sort(g.Players, function (a, b) return a.name < b.name end)
end

function PlayerInfoSave()
--	local strData = "Player{name=jin, score={34,56,78}}\nPlayer{name=hai,score={100,200,300}}\n"
--	local f = assert(io.open(GAME_PLAYER_BANK, "w"))
--	f:write(strData)
--	f:close()
end

function Player(tabPlayer)
	table.insert(g.Players, tabPlayer)
end

