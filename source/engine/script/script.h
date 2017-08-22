/** @file
 *  @brief Lua脚本程序
 *  @author jinhailiao
 *  @date 2017/03/06
 *  @version 0.1
 *  @see Lua相关文档
 */
#ifndef __MINE_LUA_SCRIPT_H__
#define __MINE_LUA_SCRIPT_H__
#include "lua.hpp"

class C_LuaScript
{
public:
	virtual ~C_LuaScript();
	static C_LuaScript &GetInstance(void);

	int Init(void);
	int Release(void);
	int LuaError(const char *pErrorInfo);
	int RegisterAPI(void);
	int loadScritp(const char *pLuaScritp);
	int call(const char* pFunctionName);
	int SetResourcePath(const char *pstrName, const char *pstrPath);

protected:
	C_LuaScript(void);

protected:
	lua_State *m_L;
};

#endif//__MINE_LUA_SCRIPT_H__

