/** @file
 *  @brief Lua脚本程序
 *  @author jinhailiao
 *  @date 2017/03/06
 *  @version 0.1
 *  @see Lua相关文档
 */
#include "script.h"
#include "hstrings.h"
#include "hguidc.h"
#include "input.h"
#include "luactrl.h"
#include "hguidlog.h"

/****************Lua API ******************/
static int lua_DrawBoxUp(lua_State *L)
{
	C_HGUIDC dc(NULL);
	S_WORD x = (S_WORD)lua_tonumber(L, 1);
	S_WORD y = (S_WORD)lua_tonumber(L, 2);
	S_WORD w = (S_WORD)lua_tonumber(L, 3);
	S_WORD h = (S_WORD)lua_tonumber(L, 4);
	bool ok = dc.DrawBoxUp(x, y, w, h);
	lua_pushboolean(L, ok);
	return 1;
}

static int lua_DrawBoxDn(lua_State *L)
{
	C_HGUIDC dc(NULL);
	S_WORD x = (S_WORD)lua_tonumber(L, 1);
	S_WORD y = (S_WORD)lua_tonumber(L, 2);
	S_WORD w = (S_WORD)lua_tonumber(L, 3);
	S_WORD h = (S_WORD)lua_tonumber(L, 4);
	bool ok = dc.DrawBoxDn(x, y, w, h);
	lua_pushboolean(L, ok);
	return 1;
}

static int lua_DrawLine(lua_State *L)
{
	C_HGUIDC dc(NULL);
	S_WORD sx = (S_WORD)lua_tonumber(L, 1);
	S_WORD sy = (S_WORD)lua_tonumber(L, 2);
	S_WORD ex = (S_WORD)lua_tonumber(L, 3);
	S_WORD ey = (S_WORD)lua_tonumber(L, 4);
	int ok = dc.DrawLine(sx, sy, ex, ey);
	lua_pushboolean(L, ok);
	return 1;
}

static int lua_DrawFlag(lua_State *L)
{
	C_HGUIDC dc(NULL);
	dc.SetTextColor(HGUI_RGB(255,0,0));
	dc.SelectObject(dc.GetStockGuiObj(HGUI_OBJ_SYM12x12));

	S_WORD x = (S_WORD)lua_tonumber(L, 1);
	S_WORD y = (S_WORD)lua_tonumber(L, 2);
	int offset = dc.DrawString(x, y, "!");
	lua_pushnumber(L, offset);
	return 1;
}

static int lua_DrawText(lua_State *L)
{
	int offset = 0;
	C_HGUIDC dc(NULL);
	dc.SelectObject(dc.GetStockGuiObj(HGUI_OBJ_F12x12));

	S_WORD x = (S_WORD)lua_tonumber(L, 1);
	S_WORD y = (S_WORD)lua_tonumber(L, 2);
	const char *pString = lua_tostring(L, 3);
	if (pString != NULL)
		offset = dc.DrawString(x, y, pString);
	lua_pushnumber(L, offset);
	return 1;
}

static int lua_DrawMine(lua_State *L)
{
	C_HGUIDC dc(NULL);
	dc.SelectObject(dc.GetStockGuiObj(HGUI_OBJ_SYM12x12));

	S_WORD x = (S_WORD)lua_tonumber(L, 1);
	S_WORD y = (S_WORD)lua_tonumber(L, 2);
	int status = lua_toboolean(L, 3); // 状态: 0,未炸;1,爆炸
	if (status != 0)
	{
		dc.SetBkColor(HGUI_RGB(255,0,0));
		dc.SetBkMode(HGUI_BKM_OPAQUE);
	}
	int offset = dc.DrawString(x, y, "#");
	lua_pushnumber(L, offset);
	return 1;
}

static int lua_DrawRect(lua_State *L)
{
	S_RECT Rect;
	Rect.x = (S_WORD)lua_tonumber(L, 1);
	Rect.y = (S_WORD)lua_tonumber(L, 2);
	Rect.w = (S_WORD)lua_tonumber(L, 3);
	Rect.h = (S_WORD)lua_tonumber(L, 4);
	S_DWORD c = (S_DWORD)lua_tonumber(L, 5);

	C_HGUIPEN Pen(c);
	C_HGUIDC dc(NULL);
	dc.SelectObject(&Pen);
	int ok = dc.FillRect(Rect);
	lua_pushboolean(L, ok);
	return 1;
}

static int lua_CreateButton(lua_State *L)
{
	const char *pTitle = lua_tostring(L, 1);
	S_WORD x = (S_WORD)lua_tonumber(L, 2);
	S_WORD y = (S_WORD)lua_tonumber(L, 3);
	S_WORD w = (S_WORD)lua_tonumber(L, 4);
	S_WORD h = (S_WORD)lua_tonumber(L, 5);
	const char *pFunction = lua_tostring(L, 6);

	C_LuaCtrl &LuaCtrl = C_LuaCtrl::GetInstance();
	LuaCtrl.PushButton(pTitle, x, y, w, h, pFunction);
	return 0;
}

static int lua_MouseState(lua_State *L)
{
	S_DWORD position = 0;
	C_INPUT &input = C_INPUT::GetInstance();
	S_WORD State = input.GetMouseState(position);
	lua_pushnumber(L, State); // 
	lua_pushnumber(L, HAI_GETLOWORD(position)); // x
	lua_pushnumber(L, HAI_GETHIWORD(position)); // y
	return 3;
}

static int lua_MsgBox(lua_State *L)
{
	int ok = 0;
	const char *ptitle = lua_tostring(L, 1);
	const char *ptext = lua_tostring(L, 2);
	if (ptitle != NULL && ptext != NULL)
		ok = HGui_MsgBox(ptitle, ptext, MB_BTN_OK|MB_TEXT_CENTER);
	lua_pushnumber(L, ok);
	return 1;
}

static int lua_debug(lua_State *L)
{
	string strInfo;
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++)
	{
		const char *pString = lua_tostring(L, i);
		if (pString != NULL)
			strInfo.append(pString);
	}
	printf(strInfo.c_str());
	return 0;
}

static int lua_quit(lua_State *L)
{
	return 0;
}

//
//
//
C_LuaScript::C_LuaScript(void)
{
	m_L = NULL;
}

C_LuaScript::~C_LuaScript()
{
	if (m_L != NULL)
		lua_close(m_L);
	m_L = NULL;
}

C_LuaScript &C_LuaScript::GetInstance(void)
{
	static C_LuaScript s_MyLuaScript;
	return s_MyLuaScript;
}

int C_LuaScript::Init(void)
{
	m_L = lua_open();
	if (m_L == NULL)
		return -1;

	luaL_openlibs(m_L); // 打开LUA内置库
	RegisterAPI();
	return 0;
}

int C_LuaScript::LuaError(const char *pErrorInfo)
{
	printf("Lua call error:%s.", pErrorInfo);
	return 0;
}

int C_LuaScript::RegisterAPI(void)
{
	if (m_L == NULL)
		return -1;

	luaL_Reg lib[] =
	{
		{"DrawBoxUp", lua_DrawBoxUp},
		{"DrawBoxDn", lua_DrawBoxDn},
		{"DrawLine", lua_DrawLine},
		{"DrawFlag", lua_DrawFlag},
		{"DrawText", lua_DrawText},
		{"DrawMine", lua_DrawMine},
		{"DrawRect", lua_DrawRect},
		{"CreateButton", lua_CreateButton},
		{"MouseState", lua_MouseState},
		{"MsgBox", lua_MsgBox},

		{"log", lua_debug},
		{"quit", lua_quit},

		{NULL, NULL},
	};
	luaL_register(m_L, "mine", lib);
	return 0;
}

int C_LuaScript::loadScritp(const char *pLuaScritp)
{
	if (m_L == NULL)
		return -1;

	string strLuaPath(pLuaScritp);
//	strLuaPath.append(pLuaScritp);
	if (luaL_loadfile(m_L, strLuaPath.c_str()) || lua_pcall(m_L, 0, 0, 0))
	{
		LuaError(lua_tostring(m_L, -1));
		return -1;
	}
	return 0;
}

int C_LuaScript::call(const char* pFunctionName)
{
	if (m_L == NULL)
		return -1;

	lua_getglobal(m_L, pFunctionName);
	if (lua_isnil(m_L, -1) == 1)	// 检查方法是否为 nil
		return -1;
	if (lua_pcall(m_L, 0, 0, 0) == 0)
		return 0;

	LuaError(lua_tostring(m_L, -1));
	return -1;
}


