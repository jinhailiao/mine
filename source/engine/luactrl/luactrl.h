/** @file
 *  @brief LUA调用控件创建信息
 *  @author jinhailiao
 *  @date 2017/03/06
 *  @version 0.1
 *  @see 
 */
#ifndef __MINE_LUACTRL_H__
#define __MINE_LUACTRL_H__
#include "haibase.h"
#include "hguiwnd.h"

struct S_ButtonInfo
{
	S_WORD m_x;
	S_WORD m_y;
	S_WORD m_w;
	S_WORD m_h;
	S_WORD m_ID;
	string m_strTitle;
	string m_strFunction;
};

struct S_ButtonInfos:public vector<S_ButtonInfo>
{
};

class C_LuaCtrl
{
public:
	virtual ~C_LuaCtrl();
	static C_LuaCtrl &GetInstance(void);

	int Release(void);
	int PushButton(const string &strTitle, S_WORD x, S_WORD y, S_WORD w, S_WORD h, const string &strFunction);

	int CreateCtrl(C_HGUIWND* pParentWnd);
	string GetFunctionWithID(S_WORD CtrlID);

protected:
	C_LuaCtrl(void);

protected:
	static S_WORD m_CtrlID;
	S_ButtonInfos m_ButtonInfos;
};

#endif//__MINE_LUACTRL_H__

