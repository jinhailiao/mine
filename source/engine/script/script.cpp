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
	return 0;
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

//		{ "surface_draw_sprite", L_draw_sprite },
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
	if (lua_isnil(m_L, -1) == 1)	// 检查方法是否为 nil
		return -1;
	if (lua_pcall(m_L, 0, 0, 0) == 0)
		return 0;

	LuaError(lua_tostring(m_L, -1));
	return -1;
}


