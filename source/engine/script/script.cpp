/** @file
 *  @brief Lua�ű�����
 *  @author jinhailiao
 *  @date 2017/03/06
 *  @version 0.1
 *  @see Lua����ĵ�
 */
#include "script.h"
#include "hstrings.h"
#include "hguidc.h"
#include "input.h"

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
	bool ok = false;
	C_HGUIDC dc(NULL);
	C_HGUIPEN Pen(HGUI_RGB(255,0,0));
	S_WORD x = (S_WORD)lua_tonumber(L, 1);
	S_WORD y = (S_WORD)lua_tonumber(L, 2);
	S_WORD w = (S_WORD)lua_tonumber(L, 3);

	if (w > 4)
	{
		S_RECT Rect;

		ok = true;
		Rect.x = x, Rect.y = y;
		Rect.w = w, Rect.h = w/2;
		dc.SelectObject(&Pen);
		dc.FillRect(Rect);
		dc.DrawHLine(x+w, y+w/2, w/2);
		dc.DrawHLine(x+w-1, y+w/2, w/2);
	}
	lua_pushboolean(L, ok);
	return 1;
}

static int lua_DrawText(lua_State *L)
{
	int offset = 0;
	C_HGUIDC dc(NULL);
	S_WORD x = (S_WORD)lua_tonumber(L, 1);
	S_WORD y = (S_WORD)lua_tonumber(L, 2);
	const char *pString = lua_tostring(L, 3);
	if (pString != NULL)
		offset = dc.DrawString(x, y, pString);
	lua_pushnumber(L, offset);
	return 1;
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

static int lua_debug(lua_State *L)
{
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

	luaL_openlibs(m_L); // ��LUA���ÿ�
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
		{ "MouseState", lua_MouseState},
//		{ "pen_print", L_print },
//		{ "key_down", L_key_down_by_name },
//		{ "key_up", L_key_up_by_name },
//		{ "music_play", L_play_music },
//		{ "music_pause", L_pause_music },
//		{ "music_proceed", L_proceed_music },

		{"msg", lua_debug},
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
	if (lua_isnil(m_L, -1) == 1)	// ��鷽���Ƿ�Ϊ nil
		return -1;
	if (lua_pcall(m_L, 0, 0, 0) == 0)
		return 0;

	LuaError(lua_tostring(m_L, -1));
	return -1;
}

