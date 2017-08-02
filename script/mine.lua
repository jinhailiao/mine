--[[
***************************************************************************
* Copyright (C) 2016, Haisoft���޹�˾
* All rights reserved.

* �ļ�����: mine.lua
* ժ    Ҫ����WINDOWS�е�ɨ�׵���Ϸ

* ��ǰ�汾: 2.0
* ��    ��: jinhailiao@163.com,QQ6033653
* �������: 2016.11.1
***************************************************************************
--]]

---------------------------[[ const define ]]----------------------------
GRIDWIDTH = 17

--��Ϸ״̬
GAMESTART=1
GAMEOVER=2
--���״̬
MOUSE_STATE_CLICK=1
MOUSE_STATE_DBCLICK=2
--ɨ������ {x,y,w}
FACE_PLATE =
{
	{x=106, y=150, w=183},
	{x=46,  y=105, w=291},
	{x=10,  y=75,  w=363}
}
--��Ϸ����
GRID_LINE = {10, 16, 20}
MINE_NUM = {15, 40, 80}
LEVEL_NAME = {"����", "�м�", "�߼�"}

--[[״̬ת����
������MS_INIT->MS_MARK;MS_MARK->MS_DOUBT;MS_DOUBT->MS_INIT
˫����MS_MARK->MS_CLEAR;MS_DOUBT->MS_INIT
--]]
MS_INIT = 1
MS_CLEAR = 2
MS_MARK = 3
MS_DOUBT = 4
--���²�����״̬ת��
MS_BOMB = 5
MS_SHOW = 6
MS_ERROR = 7

---------------------------[[ global define ]]----------------------------
g =
{
	GameLevel = 1,
	--[[ ���������ݽṹ
	{
    	mine;            -1�����ף� 0-8����Χ�˸��׵����
    	status;          MS_INIT:��ʼ��;MS_CLEAR:������;MS_MARK:����;MS_DOUBT:����;MS_BOMB:����;MS_SHOW:��ʾ��;MS_ERROR:���״���
	};
	--]]
	GameBoard = {}, --��������
	GameState = GAMESTART,
	GameTime = 0, --��Ϸ��ʱ
	TimeStart = 0, --��ʱ��ʼ

	Player = {name = "unknow", score = {999,999,999}},
--TCHAR MineStr[3][6]  = {TEXT("15"),TEXT("40"),TEXT("80")};
--TCHAR LevelStr[3][6] = {TEXT("�ͼ�"), TEXT("�м�"), TEXT("�߼�")};

--struct GRID board[20][20];         ����: 20*20;
--struct USER player, players[10];   playerΪ��ǰ��ң�playersΪ��������
--struct HERO hero[3];               heroΪɨ��Ӣ�����а�
--int    gamestart, gametime, timestart;		   gamestart:��Ϸ��ʼ�� gametime:��Ϸ��ʱ��
	MineRestNum = 0,               -- ʣ���׵���Ŀ
}

function GameInit()
	GuiInit()
	DataInit()
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

--[[ ���ݳ�ʼ������
--]]
-- ����GUI�ؼ�
function GuiInit()
	mine.CreateButton("���", 14, 24, 40, 24, "OnButtonPlayer")
	mine.CreateButton("����", 138, 14, 40, 24, "OnButtonGrade");
	mine.CreateButton("Ӣ�۰�", 226, 14, 60, 24, "OnButtonHero");
	mine.CreateButton("����", 288, 14, 40, 24, "OnButtonReset");
	mine.CreateButton("����", 330, 14, 40, 24, "OnButtonAbout");
end

function OnButtonPlayer()
end

function OnButtonGrade()
	mine.MouseState() -- ������Ϣ
	g.GameLevel = g.GameLevel + 1
	if g.GameLevel > 3 then
		g.GameLevel = 1
	end
	DataInit()
end

function OnButtonHero()
end

function OnButtonReset()
	mine.MouseState() -- ������Ϣ
	DataInit()
end

function OnButtonAbout()
end

-- ��ʼ��Ϊlevel���������
function DataInit()
	local gridnum = GRID_LINE[g.GameLevel]				-- ����level�������и�����������
	g.MineRestNum = MINE_NUM[g.GameLevel]

	g.GameTime  = 0
	g.TimeStart = os.time()
	g.GameState = GAMESTART

	g.GameBoard = {}
	for i = 1,gridnum do        -- ����ĳ�ʼ��ֵ
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

	for i = 1,gridnum do        -- ���׸��¼��Χ�İ˸����ӵ�����
		for j = 1,gridnum do
			if g.GameBoard[i][j].mine ~= -1 then
				CountMineOn9Grid(i, j, gridnum)
			end
        end
	end
	return
end

--ͳ����Χ8����׵�����
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

--[[ ����ˢ�º�������
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
--	if g.MineRestNum < 0 then
--		g.MineRestNum = 0
--	elseif g.MineRestNum > MINE_NUM[g.GameLevel] then
--		g.MineRestNum = MINE_NUM[g.GameLevel]
--	end
end

-- �����Ϸ�Ƿ����
function CheckOver()
	local gridnum = GRID_LINE[g.GameLevel]
	for i = 1,gridnum do
		for j = 1,gridnum do
        	if g.GameBoard[i][j].status == MS_INIT or g.GameBoard[i][j].status == MS_DOUBT then -- δ���
				return false,false -- over,win
			end
        	if g.GameBoard[i][j].status == MS_MARK and g.GameBoard[i][j].mine ~= -1 then -- ��Ǵ���
				return false,false -- over,win
			end
        	if g.GameBoard[i][j].status == MS_BOMB then -- ը��
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
		over, win = CheckOver() -- �ж��Ƿ���Ϸ����
	end

	if over == true then -- ��Ϸ��������
		g.TimeStart = 0
		g.GameState = GAMEOVER
	else
		return
	end

	if win == false then
		ChangeGridStateAfterOver()
	else
		PlayerInfoSave()
	end
end

--��������������
function GetGridWithCoordinate(x, y)
	local startx = FACE_PLATE[g.GameLevel].x
	local starty = FACE_PLATE[g.GameLevel].y
	local i = (x - startx) / (GRIDWIDTH+1) -- ���ָ����ӵ������±�ֵ
	local j = (y - starty) / (GRIDWIDTH+1)
	i = math.floor(i)
	j = math.floor(j)

	return i+1,j+1 --�����1��ʼ
end

-- �������������Χ�˸��������ף���ȫ�򿪣��ݹ顣i,jΪ��ǰ�������
function OpenGrid(i, j)
	if g.GameBoard[i][j].mine ~= 0 then  --  ����ķ���ֵ��Ϊ0���򷵻�
		return
	end

	local gridnum = GRID_LINE[g.GameLevel]
	for x = i-1, i+1 do -- ����Χ�˸�
		if x > 0 and x <= gridnum then
			for y = j-1, j+1 do
				if y > 0 and y <= gridnum then
					if g.GameBoard[x][y].mine ~= 0 then          -- ��Χ�˸����׵����
						if g.GameBoard[x][y].status == MS_MARK or g.GameBoard[x][y].status == MS_DOUBT then -- ����������������1
							CountMine(1)
						end
						g.GameBoard[x][y].status = MS_CLEAR
					elseif g.GameBoard[x][y].status ~= MS_CLEAR then --��Χ�˸����׵��������û��
						if g.GameBoard[x][y].status == MS_MARK or g.GameBoard[x][y].status == MS_DOUBT then -- ����������������1
							CountMine(1)
						end
						g.GameBoard[x][y].status = MS_CLEAR
						OpenGrid(x, y);  --  �ݹ����
					end
				end
			end
		end
	end
end

-- ˫���򿪵ĸ��ӣ���ʣ��Ŀո��
function OpenGridWithMS_CLEAR(i, j)
	local gridnum = GRID_LINE[g.GameLevel]
	for x = i-1, i+1 do  --����Χ�˸�
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
	if x>boardX and x<boardX+boardW and y>boardY and y<boardY+boardW then --�����Ƿ�������
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
	if x>boardX and x<boardX+boardW and y>boardY and y<boardY+boardW then --�����Ƿ�������
		local i,j=GetGridWithCoordinate(x,y)
		local status = g.GameBoard[i][j].status
		if status == MS_MARK then --windowsϵͳ˫�����յ�������Ϣ
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
			local over,win = OpenGridWithMS_CLEAR(i, j) --����Χ8��δ�򿪵ĸ���
			HandleOver(over, win)
		end
	end
end

--[[ ��ͼ��������
--]]
function DrawGui()
	local iMineRestNum = g.MineRestNum
	if g.MineRestNum < 0 then
		iMineRestNum = 0
	elseif g.MineRestNum > MINE_NUM[g.GameLevel] then
		iMineRestNum = MINE_NUM[g.GameLevel]
	end

	local strMineRest = string.format("%03d", iMineRestNum)
	local strGameTime = string.format("%03d��", g.GameTime)
	local strScore = string.format("%03d��", g.Player.score[g.GameLevel])

	mine.DrawRect(56, 24, 80, 24, 0xF0F0F0)
	mine.DrawText(58, 30, g.Player.name)

	mine.DrawRect(180, 16, 44, 20, 0xF0F0F0)
	mine.DrawText(190, 20, LEVEL_NAME[g.GameLevel])

	mine.DrawRect(138, 40, 40, 20, 0xF0F0F0)
	mine.DrawText(146, 44, "�ɼ�")

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
	local gridnum = GRID_LINE[g.GameLevel]				-- ����level�������и�����������

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

--[[ ��Ϸ�����Ϣ����
--]]
function PlayerInfoSave()
	if g.GameTime > g.Player.score[g.GameLevel] then
		return
	end
	g.Player.score[g.GameLevel] = g.GameTime
end

--[[
************************ data structure *********************************


struct USER              ��ҵ����ݽṹ
{
	TCHAR name[12];         ����
	int  time[3];         ��ͬ���������ʱ��
};

struct HERO              ɨ��Ӣ�۵����ݽṹ
{
    TCHAR name[12];        ɨ��Ӣ�۵�����
    int time;            ɨ��Ӣ�����õ�����ʱ��
};

struct FACEPLATE
{
	WORD x;
	WORD y;
	WORD w;
};

*********************** function prototype ******************************
BOOL Draw3DRect(HDC hDC, WORD x, WORD y, WORD cx, WORD cy, BOOL IsUp);
BOOL DrawGridding(HDC hDC, WORD x, WORD y, WORD GridCnt);
void DrawMine(HDC hDC, WORD x, WORD y);
void DrawRedRect(HDC hDC, WORD x, WORD y);
void DrawRedFlag(HDC hDC, WORD x, WORD y);
void DataInit(void);
int  Win(int gridnum);
int  PlayGameL(HDC hDC, int startx, int starty, int x, int y, int gridnum);
int  PlayGameLDblClk(HDC hDC, int startx, int starty, int x, int y, int gridnum);
void PlayGameR(HDC hDC, int startx, int starty, int x, int y);    �һ�����Ĵ���
void OpenGrid(HDC hDC, int startx, int starty, int i, int j, int gridnum);
void DisplayOperateResult(HDC hDC, int status, int boardX, int boardY, int i, int j);
void GameOver(int gridnum);
void PlayerInit(void);
void CountMine(int flag); ����ʣ��������flag=-1����һ����,flag=1����һ�ʺ�
void WriteDisk(void);         ��Ϸ����������д��
void Record(int level);   ��¼�ɼ�
void GameMenu(HWND hwnd, int x, int y);
void NameInputOver(void);
void SelectPlayer(void);

************************* main function *********************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i, j;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hDC;
	WORD x, y;
	WORD operate;
	WORD id;
	WORD boardX = FacePlate[Level].x;
	WORD boardY = FacePlate[Level].y;
	TCHAR str[14];

	switch (message)
	{
	case WM_CREATE:
		hwndpler   = CreateWindow(TEXT("BUTTON"), TEXT("���"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_CENTER|WS_TABSTOP, 14, 24, 40, 24, hWnd, (HMENU)1, hInst, NULL);
		hwndedit   = CreateWindow(TEXT("EDIT"), TEXT("����"),WS_CHILD|WS_VISIBLE|ES_LEFT|WS_BORDER, 58, 24, 80, 24, hWnd, (HMENU)2, hInst, NULL);
		hwndlevel  = CreateWindow(TEXT("BUTTON"), TEXT("����"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_CENTER|WS_TABSTOP, 140, 12, 40, 24, hWnd, (HMENU)3, hInst, NULL);
		hwndlevel2 = CreateWindow(TEXT("STATIC"), TEXT("�ͼ�"),WS_CHILD|WS_VISIBLE|SS_CENTER, 182, 14, 44, 20, hWnd, (HMENU)4, hInst, NULL);
		hwndscore  = CreateWindow(TEXT("STATIC"), TEXT("�ɼ�"),WS_CHILD|WS_VISIBLE|SS_CENTER, 140, 38, 40, 20, hWnd, (HMENU)5, hInst, NULL);
		hwndscore2 = CreateWindow(TEXT("STATIC"), TEXT("999��"),WS_CHILD|WS_VISIBLE|SS_CENTER,182, 38, 60, 20, hWnd, (HMENU)6, hInst, NULL);
		hwndhero   = CreateWindow(TEXT("BUTTON"), TEXT("Ӣ�۰�"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_CENTER|WS_TABSTOP, 228, 12, 60, 24, hWnd, (HMENU)7, hInst, NULL);
		hwndreset  = CreateWindow(TEXT("BUTTON"), TEXT("����"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_CENTER|WS_TABSTOP, 290, 12, 40, 24, hWnd, (HMENU)8, hInst, NULL);
		hwndabout  = CreateWindow(TEXT("BUTTON"), TEXT("����"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_CENTER|WS_TABSTOP, 332, 12, 40, 24, hWnd, (HMENU)9, hInst, NULL);
		hwndmine   = CreateWindow(TEXT("STATIC"), TEXT("15"),WS_CHILD|WS_VISIBLE|SS_CENTER, 264, 38, 40, 20, hWnd, (HMENU)10, hInst, NULL);
		hwndtime   = CreateWindow(TEXT("STATIC"), TEXT("000"),WS_CHILD|WS_VISIBLE|SS_CENTER, 320, 38, 40, 20, hWnd, (HMENU)11, hInst, NULL);

		fnEditProc = (WNDPROC)SetWindowLong(hwndedit, GWL_WNDPROC, (LONG)EditProc);
		hBrushwhite = (HBRUSH)GetStockObject(WHITE_BRUSH);
		hBrushblack = (HBRUSH)GetStockObject(BLACK_BRUSH);

		DataInit();
		PlayerInit();

		SetTimer(hWnd, 1, 1000, NULL);
		break;

	case WM_SETFOCUS:
		//			SetFocus(hwndpler);
		break;
	case WM_KILLFOCUS:
		//			if ((HWND)wParam != hwndedit)
		//				SetFocus(hWnd);
		break;

	case WM_CTLCOLORSTATIC:
		id = GetWindowLong((HWND)lParam, GWL_ID);
		if (id == 4 || id == 6)
			return (LRESULT)hBrushwhite;
		if (id == 10 || id == 11)
		{
			SetTextColor((HDC)wParam, RGB(255, 0, 0));
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			return (LRESULT)hBrushblack;
		}
		break;
	case WM_CTLCOLOREDIT:
		return (LRESULT)hBrushwhite;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case 1:
			SetFocus(hWnd);
			SelectPlayer();
			break;
		case 2:
			if (wmEvent == EB_PRESSESC)
			{
				SetWindowText(hwndedit, player.name);
				SetFocus(hWnd);
				break;
			}
			if (wmEvent == EB_PRESSENTER)
			{
				NameInputOver();
				SetFocus(hWnd);
			}
			break;
		case 200:
		case 201:
		case 202:
		case 3:
			if (wmId != 3)
				Level = wmId-200;
			else
				Level = (Level+1)%3;
			SetWindowText(hwndlevel2, LevelStr[Level]);
		case 8:
			wsprintf(str, TEXT("%3d��"), player.time[Level]);
			SetWindowText(hwndscore2, str);
			SetWindowText(hwndmine, MineStr[Level]);
			SetWindowText(hwndtime, TEXT("000"));
			DataInit();
			InvalidateRect(hWnd, NULL, TRUE);
			SetFocus(hWnd);
			break;
		case 7:
			DialogBox(hInst, (LPCTSTR)IDD_HERODLG, hWnd, (DLGPROC)HeroProc);
			SetFocus(hWnd);
			break;
		case 9:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			SetFocus(hWnd);
			break;
		case 400:
		case 401:
		case 402:
		case 403:
		case 404:
		case 405:
		case 406:
		case 407:
		case 408:
		case 409:
			i = wmId - 400;
			player     = players[i];
			players[i] = players[0];
			players[0] = player;
			SetWindowText(hwndedit, player.name);
			SendMessage(hWnd, WM_COMMAND, 8, 0);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			Draw3DRect(hDC, 1, 1, 381, 445, TRUE);
			Draw3DRect(hDC, 10, 10, 363, 50, FALSE);
			Draw3DRect(hDC, boardX, boardY, FacePlate[Level].w, FacePlate[Level].w, FALSE);
			for (i = 0; i < GridLine[Level]; i++)
				for (j = 0; j < GridLine[Level]; j++)
					DisplayOperateResult(hDC, board[i][j].status, boardX, boardY, i, j);
				DrawGridding(hDC, boardX+2, boardY+2, GridLine[Level]);
				EndPaint(hWnd, &ps);
				break;

		case WM_LBUTTONDOWN:
			if (!gamestart)
				break;
			hDC = GetDC(hWnd);
			x = LOWORD(lParam); y = HIWORD(lParam);
			if (x > boardX && x < boardX + FacePlate[Level].w && y > boardY && y < boardY+ FacePlate[Level].w)
			{
				timestart = 1;
				operate = PlayGameL(hDC, boardX, boardY, x, y, GridLine[Level]);
				if (operate)            �����ȷ
				{
					if (Win(GridLine[Level]))    �ж�һ���Ƿ�Ӯ��
					{
						gamestart = 0;        Ϊ0����Ϸ��ʱֹͣ
						timestart = 0;
						Record(Level); �ж��Ƿ��Ƽ�¼
					}
				}
				else          ��Ϸʧ�ܵĴ���
				{
					gamestart = 0;    ��ʱֹͣ
					timestart = 0;
					GameOver(GridLine[Level]); ��ʣ�����
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			ReleaseDC(hWnd, hDC);
			break;

		case WM_RBUTTONDOWN:
			x = LOWORD(lParam); y = HIWORD(lParam);
			if (!gamestart || !timestart && (x <= boardX || x >= boardX + FacePlate[Level].w || y <= boardY || y >= boardY+ FacePlate[Level].w))//
			{
				POINT point;

				point.x = x; point.y = y;
				ClientToScreen(hWnd, &point);
				GameMenu(hWnd, point.x, point.y);
				break;
			}
			hDC = GetDC(hWnd);
			if (x > boardX && x < boardX + FacePlate[Level].w && y > boardY && y < boardY+ FacePlate[Level].w)
			{
				timestart = 1;
				PlayGameR(hDC, boardX, boardY, x, y);    �һ�����Ĵ���
				if (Win(GridLine[Level]))                  �ж��Ƿ�Ӯ
				{
					gamestart = 0;                      ��ͣ��ʱ
					timestart = 0;
					Record(Level);     �Ƿ��Ƽ�¼
				}
			}
			ReleaseDC(hWnd, hDC);
			break;

		case WM_LBUTTONDBLCLK:
			if (!gamestart)
				break;
			hDC = GetDC(hWnd);
			x = LOWORD(lParam); y = HIWORD(lParam);
			if (x > boardX && x < boardX + FacePlate[Level].w && y > boardY && y < boardY+ FacePlate[Level].w)
			{
				i = (x - boardX) / (GRIDWIDTH+1);  ���ָ����ӵ������±�ֵ
				j = (y - boardY) / (GRIDWIDTH+1);

				if (board[i][j].status == 2)
				{
					operate = PlayGameLDblClk(hDC, boardX, boardY, x, y, GridLine[Level]);
					if (operate)            ˫����ȷ
					{
						if (Win(GridLine[Level]))    �ж�һ���Ƿ�Ӯ��
						{
							gamestart = 0;        Ϊ0����Ϸ��ʱֹͣ
							timestart = 0;
							Record(Level); �ж��Ƿ��Ƽ�¼
						}
					}
					else          ��Ϸʧ�ܵĴ���
					{
						gamestart = 0;    ��ʱֹͣ
						timestart = 0;
						GameOver(GridLine[Level]); ��ʣ�����
						InvalidateRect(hWnd, NULL, TRUE);
					}
				}
			}
			ReleaseDC(hWnd, hDC);
			break;

		case WM_CHAR:
			switch (wParam)
			{
			case ' ':
				timestart = 0;
				break;
			case '\t':
				SetFocus(hwndedit);
				break;
			}
			break;

			case WM_TIMER:
				if (timestart)
				{
					if (gametime < 999)
						gametime++;
					wsprintf(str, TEXT("%03d"), gametime);
					SetWindowText(hwndtime, str);
				}
				break;

			case WM_DESTROY:
				KillTimer(hWnd, 1);
				WriteDisk();
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

BOOL Draw3DRect(HDC hDC, WORD x, WORD y, WORD cx, WORD cy, BOOL IsUp)
{
	HPEN hPen;
	COLORREF TopColor, BottomColor;

	if (IsUp)
	{
		TopColor    = C_BTNHIGHLIGHT;
		BottomColor = C_BTNSHADOW;
	}
	else
	{
		TopColor    = C_BTNSHADOW;
		BottomColor = C_BTNHIGHLIGHT;
	}
	hPen = CreatePen(PS_SOLID, 1, TopColor);
	assert(hPen);
	SelectObject(hDC, hPen);

	MoveToEx(hDC, x, y, NULL);
	LineTo(hDC, x+cx, y);
	MoveToEx(hDC, x, y+1, NULL);
	LineTo(hDC, x+cx, y+1);

	MoveToEx(hDC, x, y, NULL);
	LineTo(hDC, x, y+cy);
	MoveToEx(hDC, x+1, y, NULL);
	LineTo(hDC, x+1, y+cy);

	DeleteObject(hPen);
	hPen = CreatePen(PS_SOLID, 1, BottomColor);
	assert(hPen);
	SelectObject(hDC, hPen);

	MoveToEx(hDC, x+cx-1, y+2, NULL);
	LineTo(hDC, x+cx-1, y+cy);
	MoveToEx(hDC, x+cx, y+1, NULL);
	LineTo(hDC, x+cx, y+cy);

	MoveToEx(hDC, x+2, y+cy-1, NULL);
	LineTo(hDC, x+cx, y+cy-1);
	MoveToEx(hDC, x+1, y+cy, NULL);
	LineTo(hDC, x+cx+1, y+cy);

	SelectObject(hDC, GetStockObject(BLACK_PEN));
	DeleteObject(hPen);

	return TRUE;
}
void DrawRedRect(HDC hDC, WORD x, WORD y)
{
	HBRUSH hBrush;

	hBrush = CreateSolidBrush(RGB(255,0,0));
	hBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, x, y, x+GRIDWIDTH, y+GRIDWIDTH);
	hBrush = (HBRUSH)SelectObject(hDC, hBrush);
	DeleteObject(hBrush);
}

void DrawRedFlag(HDC hDC, WORD x, WORD y)
{
	HBRUSH hBrush;
	HPEN   hPen;

	hBrush = CreateSolidBrush(RGB(255,0,0));
	hBrush = (HBRUSH)SelectObject(hDC, hBrush);
	hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	hPen = (HPEN)SelectObject(hDC, hPen);
	Rectangle(hDC, x+4, y+4, x+GRIDWIDTH-4, y+GRIDWIDTH-6);
	MoveToEx(hDC, x+GRIDWIDTH-4, y+4, NULL);
	LineTo(hDC, x+GRIDWIDTH-4, y+GRIDWIDTH-2);
	MoveToEx(hDC, x+GRIDWIDTH-5, y+4, NULL);
	LineTo(hDC, x+GRIDWIDTH-5, y+GRIDWIDTH-2);
	hBrush = (HBRUSH)SelectObject(hDC, hBrush);
	DeleteObject(hBrush);
	hPen = (HPEN)SelectObject(hDC, hPen);
	DeleteObject(hPen);
}

void DrawMine(HDC hDC, WORD x, WORD y)
{
	HBRUSH hBrush;

	hBrush = (HBRUSH)SelectObject(hDC, GetStockObject(BLACK_BRUSH));
	Ellipse(hDC, x+3, y+3, x+GRIDWIDTH-2, y+GRIDWIDTH-2);
	SelectObject(hDC, hBrush);
	MoveToEx(hDC, x+1, y+GRIDWIDTH/2, NULL);
	LineTo(hDC, x+GRIDWIDTH, y+GRIDWIDTH/2);
	MoveToEx(hDC, x+GRIDWIDTH/2, y+1, NULL);
	LineTo(hDC, x+GRIDWIDTH/2, y+GRIDWIDTH);
}

BOOL DrawGridding(HDC hDC, WORD x, WORD y, WORD GridCnt)
{
	WORD i;

	for (i = 0; i < GridCnt+1; i++)
	{
		MoveToEx(hDC, x-1+i*(GRIDWIDTH+1), y, NULL);
		LineTo(hDC, x-1+i*(GRIDWIDTH+1), y-1+GridCnt*(GRIDWIDTH+1));
	}
	for (i = 0; i < GridCnt+1; i++)
	{
		MoveToEx(hDC, x, y-1+i*(GRIDWIDTH+1), NULL);
		LineTo(hDC, x-1+GridCnt*(GRIDWIDTH+1), y-1+i*(GRIDWIDTH+1));
	}
	return TRUE;
}



 �������������Χ�˸��������ף���ȫ�򿪣��ݹ顣i,jΪ��ǰ�������
void OpenGrid(HDC hDC, int startx, int starty, int i, int j, int gridnum)
{
	TCHAR num[2] = {0,0};
	int x, y;

	if (board[i][j].mine != 0)   ����ķ���ֵ��Ϊ0���򷵻�
	{
		return;
	}

	for (x = i-1; x <= i+1; x++)  ����Χ�˸�
	{
		if ((x < 0) || (x >= gridnum))    x�±�Խ�����
		{
			continue;
		}

		for (y = j-1; y <= j+1; y++)
		{
			if ((y < 0) || (y >= gridnum))   y�±�Խ�����
			{
				continue;
			}

			if (board[x][y].mine != 0)           ��Χ�˸����׵����
			{
				if (board[x][y].status == 2)
					continue;
				if (board[x][y].status == 3)    ����������������1
				{
					MineRestNum++;
				}
				board[x][y].status = 2;
				board[x][y].check = 1;
				DisplayOperateResult(hDC, board[x][y].status, startx, starty, x, y);
			}

			else if (board[x][y].status != 2)��Χ�˸����׵��������û��
			{
				if (board[x][y].status == 3) ����������������1
				{
					MineRestNum++;
				}
				board[x][y].status = 2;
				board[x][y].check = 1;
				DisplayOperateResult(hDC, board[x][y].status, startx, starty, x, y);
				OpenGrid(hDC, startx, starty, x, y, gridnum);      �ݹ����
			}
		}
	}
	CountMine(0);
}

 ����������еĴ���
int PlayGameL(HDC hDC, int startx, int starty, int x, int y, int gridnum)
{
	TCHAR num[4];
	int i, j;

	i = (x - startx) / (GRIDWIDTH+1);  ���ָ����ӵ������±�ֵ
	j = (y - starty) / (GRIDWIDTH+1);

	if (board[i][j].status == 1)               ָ���ʼ״̬�ĸ���
	{
		if (board[i][j].mine != -1)           ָ�����׸�
		{
			if (board[i][j].mine != 0) ��Χ�˸������ɸ��׵����,�򿪱���
			{
				board[i][j].status = 2;
				board[i][j].check = 1;
				Draw3DRect(hDC, startx+2+i*(GRIDWIDTH+1), starty+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH, FALSE);
				num[0] = board[i][j].mine+48;
				num[1] = 0;
				SetBkMode(hDC, TRANSPARENT);
				TextOut(hDC, startx+7+i*(GRIDWIDTH+1), starty+3+j*(GRIDWIDTH+1), num, lstrlen(num));
			}
			else                     ��Χ�˸����ף��򿪱������Χ�˸���
			{                     �ݹ�
				OpenGrid(hDC, startx, starty, i, j, gridnum);
			}

			return 1;
		}
		else                         ������󣬱�����ף�����0
		{
			board[i][j].status = 0;
			board[i][j].check = 0;
			return 0;
		}
	}
	else         ������
	{
		return 1;
	}
}

int PlayGameLDblClk(HDC hDC, int startx, int starty, int x, int y, int gridnum)
{
	//	TCHAR num[4];
	int i, j;

	i = (x - startx) / (GRIDWIDTH+1);  ���ָ����ӵ������±�ֵ
	j = (y - starty) / (GRIDWIDTH+1);

	for (x = i-1; x <= i+1; x++)  ����Χ�˸�
	{
		if ((x < 0) || (x >= gridnum))    x�±�Խ�����
		{
			continue;
		}

		for (y = j-1; y <= j+1; y++)
		{
			if ((y < 0) || (y >= gridnum))   y�±�Խ�����
			{
				continue;
			}

			if (board[x][y].status != 2 && board[x][y].status != 3)
			{
				if (board[x][y].mine == -1)
				{
					board[x][y].status = 0;
					DisplayOperateResult(hDC, board[x][y].status, startx, starty, x, y);
					return 0;
				}
				board[x][y].status = 2;
				board[x][y].check = 1;
				DisplayOperateResult(hDC, board[x][y].status, startx, starty, x, y);
				if (board[x][y].mine == 0)
					OpenGrid(hDC, startx, starty, x, y, gridnum);
			}
		}
	}
	return 1;
}

void PlayGameR(HDC hDC, int startx, int starty, int x, int y)    �һ�����Ĵ���
{
	int i, j;

	i = (x - startx) / (GRIDWIDTH+1);   ���ָ�򷽸������
	j = (y - starty) / (GRIDWIDTH+1);

	if (board[i][j].status == 1)      ����
	{
		board[i][j].status = 3;
		if (board[i][j].mine == -1)
		{
			board[i][j].check = 1;
		}
		DrawRedFlag(hDC, i*(GRIDWIDTH+1)+startx+2, j*(GRIDWIDTH+1)+starty+2);
		CountMine(-1);
		return;
	}

	if (board[i][j].status == 3)     �ꣿ����ʾ�˸�������
	{
		board[i][j].status = 4;
		board[i][j].check = 0;
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, i*(GRIDWIDTH+1)+startx+6, j*(GRIDWIDTH+1)+starty+2, TEXT("?"), 1);
		CountMine(1);
		return;
	}

	if (board[i][j].status == 4)     �������
	{
		HPEN   hPen;
		HBRUSH hBrush;
		//		DWORD c = GetPixel(hDC, 50, 40);

		board[i][j].status = 1;
		hPen   = CreatePen(PS_SOLID, 1, 0xD8E9EC);
		hBrush = CreateSolidBrush(0xD8E9EC);
		hPen   = (HPEN)SelectObject(hDC, hPen);
		hBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Rectangle(hDC, i*(GRIDWIDTH+1)+startx+4, j*(GRIDWIDTH+1)+starty+4, i*(GRIDWIDTH+1)+startx+1+GRIDWIDTH, j*(GRIDWIDTH+1)+starty+1+GRIDWIDTH);
		hPen   = (HPEN)SelectObject(hDC, hPen);
		hBrush = (HBRUSH)SelectObject(hDC, hBrush);
		DeleteObject(hPen);
		DeleteObject(hBrush);
		return;
	}
}

int Win(int gridnum)    ���һ���Ƿ�Ӯ��
{
	int i, j;

	for (i = 0; i < gridnum; i++)
	{
		for (j = 0; j < gridnum; j++)
		{
			if (board[i][j].check == 0)
				return 0;
		}
	}

	return 1;
}

void GameOver(int gridnum)   ��Ϸʧ�ܵĴ���
{
	int i, j;

	for (i = 0; i < gridnum; i++)
	{
		for (j = 0; j < gridnum; j++)
		{
			if ((board[i][j].mine == -1) && (board[i][j].status == 1))
            {                               δ��ʾΪ�׵ķ�����ʾ��
				board[i][j].status = 5;
			}
			else if ((board[i][j].mine != -1) && (board[i][j].status == 3))
			{                          ����˵Ļ� X
				board[i][j].status = 6;
			}
		}
	}
}

void PlayerInit(void)
{
	int i, j;
	int m = 0, n = 0;
	FILE *fp;

	lstrcpy(player.name, TEXT("����"));             ��ǰ���ȱʡΪnoname
	player.time[0] = player.time[1] = player.time[2] = 999;

	if ((fp = fopen("c:\\windows\\profile.epl", "rb")) != NULL)���Ѵ�����
	{
		m = fread(hero, sizeof(hero[0]), 3, fp);
		n = fread(players, sizeof(player), 10, fp);

		for (i = 0; i < 3; i++)        ����Ϊ��֤�������ݵ���ȷ�Ժ�������
		{
			hero[i].name[10] = 0;
			if ((hero[i].time <= 0) || (hero[i].time > 999))
			{
				hero[i].time = 999;
			}
		}
		for (i = 0; i < 10; i++)
		{
			players[i].name[10] = 0;
			for (j = 0; j < 3; j++)
			{
				if ((players[i].time[j] <= 0) || (players[i].time[j] > 999))
				{
					players[i].time[j] = 999;
				}
			}
		}
		fclose(fp);
	}

	if ((m != 3) || (n != 10))    �������, ����д
	{
		for (i = 0; i < 3; i++)
		{
			lstrcpy(hero[i].name,TEXT("����"));
			hero[i].time = 999;
		}
		for (i = 0; i < 10; i++)
		{
			players[i] = player;
		}
	}
}

void CountMine(int flag) ����ʣ��������flag=-1����һ����,flag=1����һ�ʺ�
{
	int temp;
	TCHAR mnum[8];

	if (flag == 1)
	{
		MineRestNum++;
	}
	else if (flag == -1)
	{
		MineRestNum--;
	}

	if (MineRestNum < 0)    ����ױ�ʵ���׶�����
	{
		temp = 0;
	}
	else
	{
		temp = MineRestNum;
	}

	wsprintf(mnum, TEXT("%02d"), temp);
	SetWindowText(hwndmine, mnum);
}

void WriteDisk(void)         ��Ϸ����������д��
{
	int i;
	FILE *fp;

	for (i = 0; i < 10; i++)
	{
		if (lstrcmp(players[i].name, TEXT("����")) == 0)
		{
			players[i].time[0] = players[i].time[1]
				= players[i].time[2] = 999;
		}
	}

	if ((fp = fopen("c:\\windows\\profile.epl", "wb")) != NULL)
	{
		fwrite(hero, sizeof(struct HERO), 3, fp);
		fwrite(players, sizeof(struct USER), 10, fp);
		fclose(fp);
	}
}

LRESULT CALLBACK RecardProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	return FALSE;
}

void Record(int level)   ��¼�ɼ�
{
	int i;
	TCHAR  str[8];

	if (player.time[level] > gametime)     ��¼���ٵ�����
	{
		player.time[level] = gametime;
		for (i = 0; i < 10; i++)
		{
			if (lstrcmp(player.name, players[i].name) == 0)
			{
				players[i].time[level] = gametime;
				break;
			}
		}
		wsprintf(str, TEXT("%03d��"), gametime);
		SetWindowText(hwndscore2, str);
	}

	if (hero[level].time > gametime)                �Ƽ�¼
	{
		hero[level].time = gametime;               ���¼�¼
		lstrcpy(hero[level].name, player.name);
		DialogBox(hInst, (LPCTSTR)IDD_RECARD, hWND, (DLGPROC)RecardProc);
	}
}

LRESULT CALLBACK HeroProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	HDC hDC;
	TCHAR str[8];

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	case WM_PAINT:
		hDC = GetDC(hDlg);
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, 20, 10, TEXT("����"), lstrlen(TEXT("����")));
		TextOut(hDC, 100, 10, TEXT("���"), lstrlen(TEXT("���")));
		TextOut(hDC, 180, 10, TEXT("�ɼ�"), lstrlen(TEXT("�ɼ�")));
		for (i = 0; i < 3; i++)
		{
			TextOut(hDC, 20, 35+i*20, LevelStr[i], lstrlen(LevelStr[i]));
			TextOut(hDC, 90, 35+i*20, hero[i].name, lstrlen(hero[i].name));
			wsprintf(str, TEXT("%03d��"), hero[i].time);
			TextOut(hDC, 175, 35+i*20, str, lstrlen(str));
		}
		ReleaseDC(hDlg, hDC);
		break;
	default:
		break;
	}
	return FALSE;
}

LRESULT CALLBACK EditProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CHAR && (wParam == '\r' || wParam == 27ESC))
	{
		if (wParam == '\r')
			SendMessage(hWND, WM_COMMAND, EB_PRESSENTER<<16|2, lParam);
		else
			SendMessage(hWND, WM_COMMAND, EB_PRESSESC<<16|2, lParam);
		return TRUE;
	}
	return CallWindowProc(fnEditProc, hDlg, message, wParam, lParam);
}

void GameMenu(HWND hwnd, int x, int y)
{
	int i, j;
	HMENU hMenu, hPopMenu, hSubMenu;
	TCHAR ScoreStr[20];

	hMenu = CreatePopupMenu();

	hPopMenu = CreatePopupMenu();
	if (!lstrcmp(players[0].name, TEXT("����")))
	{
		AppendMenu(hMenu, MF_POPUP|MF_GRAYED, (DWORD)hPopMenu, TEXT("ѡ�����"));
	}
	else
	{
		AppendMenu(hMenu, MF_POPUP, (DWORD)hPopMenu, TEXT("ѡ�����"));
		for (i = 0; i < 10; i++)
		{
			if (!lstrcmp(players[i].name, TEXT("����")))
				break;
			AppendMenu(hPopMenu, MF_STRING, 400+i, players[i].name);
		}
	}

	hPopMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_POPUP, (DWORD)hPopMenu, TEXT("ѡ�񼶱�"));
	AppendMenu(hPopMenu, MF_STRING, 200+0, TEXT("�ͼ�"));
	AppendMenu(hPopMenu, MF_STRING, 200+1, TEXT("�м�"));
	AppendMenu(hPopMenu, MF_STRING, 200+2, TEXT("�߼�"));

	hPopMenu = CreatePopupMenu();
	if (!lstrcmp(players[0].name, TEXT("����")))
	{
		AppendMenu(hMenu, MF_POPUP|MF_GRAYED, (DWORD)hPopMenu, TEXT("�ɼ���"));
	}
	else
	{
		AppendMenu(hMenu, MF_POPUP, (DWORD)hPopMenu, TEXT("�ɼ���"));
		for (i = 0; i < 10; i++)
		{
			if (!lstrcmp(players[i].name, TEXT("����")))
				break;
			hSubMenu = CreatePopupMenu();
			AppendMenu(hPopMenu, MF_POPUP, (DWORD)hSubMenu, players[i].name);
			for (j = 0; j < 3; j++)
			{
				memset(ScoreStr, 0, 20*sizeof(TCHAR));
				wsprintf(ScoreStr, "%s   ", LevelStr[j]);
				wsprintf(ScoreStr+lstrlen(ScoreStr), "%03d��", players[i].time[j]);
				AppendMenu(hSubMenu, MF_STRING, 0, ScoreStr);
			}
		}
	}

	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_STRING, 7, TEXT("Ӣ�۰�"));
	AppendMenu(hMenu, MF_STRING, 8, TEXT("����"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_STRING, 9, TEXT("����..."));

	TrackPopupMenu(hMenu, TPM_LEFTALIGN, x, y, 0, hwnd, NULL);

	for (i = 0; i < 2; i++)
	{
		hPopMenu = GetSubMenu(hMenu, i);
		DestroyMenu(hPopMenu);
	}
	hPopMenu = GetSubMenu(hMenu, 2);
	for (i = 0; i < 10; i++)
	{
		if (!lstrcmp(players[i].name, TEXT("����")))
			break;
		hSubMenu = GetSubMenu(hPopMenu, i);
		DestroyMenu(hSubMenu);
	}
	DestroyMenu(hPopMenu);

	DestroyMenu(hMenu);
}

void DisplayOperateResult(HDC hDC, int status, int boardX, int boardY, int i, int j)
{
	TCHAR str[14];

	SetBkMode(hDC, TRANSPARENT);
	switch (status)
	{
	case 0:
		DrawRedRect(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1));
		Draw3DRect(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH, FALSE);
		DrawMine(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1));
		break;
	case 1:
		Draw3DRect(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH, TRUE);
		break;
	case 2:
		Draw3DRect(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH, FALSE);
		if (board[i][j].mine > 0)
		{
			str[0] = board[i][j].mine+48;
			str[1] = 0;
			TextOut(hDC, boardX+6+i*(GRIDWIDTH+1), boardY+3+j*(GRIDWIDTH+1), str, lstrlen(str));
		}
		break;
	case 3:
		Draw3DRect(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH, TRUE);
		DrawRedFlag(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1));
		break;
	case 4:
		Draw3DRect(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH, TRUE);
		DrawRedFlag(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1));
		TextOut(hDC, boardX+6+i*(GRIDWIDTH+1), boardY+3+j*(GRIDWIDTH+1), TEXT("?"), lstrlen(TEXT("?")));
		break;
	case 5:
		Draw3DRect(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH, FALSE);
		DrawMine(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1));
		break;
	case 6:
		Draw3DRect(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH, TRUE);
		DrawRedFlag(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1));
		TextOut(hDC, boardX+3+i*(GRIDWIDTH+1), boardY+3+j*(GRIDWIDTH+1), TEXT("��"), lstrlen(TEXT("��")));
		break;
	default:
		Draw3DRect(hDC, boardX+2+i*(GRIDWIDTH+1), boardY+2+j*(GRIDWIDTH+1), GRIDWIDTH, GRIDWIDTH, TRUE);
		break;
	}
}

void NameInputOver(void)
{
	int i;
	TCHAR str[12];

	GetWindowText(hwndedit, str, 12);
	if (*str & lstrcmp(player.name, str))
	{
		lstrcpy(player.name, str);
		for (i = 0; i < 10; i++)
		{
			if (!lstrcmp(players[i].name, str))
				break;
		}
		if (i == 10)                          ��ͬ��
		{
			for (i = 9; i > 0; i--)           ȥ�����һ��
			{
				players[i] = players[i-1];
			}
			 ��������0��λ��
			player.time[0] = player.time[1] = player.time[2] = 999;
			players[0] = player;
		}
		else                                  ͬ����Ϊ����
		{
			player     = players[i];
			players[i] = players[0];
			players[0] = player;
		}
		SendMessage(hWND, WM_COMMAND, 8, (LPARAM)hwndreset);
	}
	SetWindowText(hwndedit, player.name);
}

void SelectPlayer(void)
{
	int i;
	HMENU hPopMenu;
	POINT point = {14, 48};

	hPopMenu = CreatePopupMenu();
	if (lstrcmp(players[0].name, TEXT("����")))
	{
		for (i = 0; i < 10; i++)
		{
			if (!lstrcmp(players[i].name, TEXT("����")))
				break;
			AppendMenu(hPopMenu, MF_STRING, 400+i, players[i].name);
		}
		ClientToScreen(hWND, &point);
		TrackPopupMenu(hPopMenu, TPM_LEFTALIGN, point.x, point.y, 0, hWND, NULL);
	}
	DestroyMenu(hPopMenu);
}

--]]

main_init()
main_update()
main_draw()
