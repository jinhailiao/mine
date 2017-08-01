/** @file
 *  @brief LUA调用控件创建信息
 *  @author jinhailiao
 *  @date 2017/03/06
 *  @version 0.1
 *  @see 
 */
#include "luactrl.h"
#include "hguictrl.h"

S_WORD C_LuaCtrl::m_CtrlID = 100;
//
//
//
C_LuaCtrl::C_LuaCtrl(void)
{
}

C_LuaCtrl::~C_LuaCtrl()
{
}

C_LuaCtrl &C_LuaCtrl::GetInstance(void)
{
	static C_LuaCtrl s_MyLuaCtrl;
	return s_MyLuaCtrl;
}

int C_LuaCtrl::PushButton(const string &strTitle, S_WORD x, S_WORD y, S_WORD w, S_WORD h, const string &strFunction)
{
	S_ButtonInfo ButtonInfo;
	ButtonInfo.m_strTitle = strTitle;
	ButtonInfo.m_x = x;
	ButtonInfo.m_y = y;
	ButtonInfo.m_w = w;
	ButtonInfo.m_h = h;
	ButtonInfo.m_ID = m_CtrlID++;
	ButtonInfo.m_strFunction = strFunction;

	m_ButtonInfos.push_back(ButtonInfo);
	return (int)m_ButtonInfos.size();
}

int C_LuaCtrl::CreateCtrl(C_HGUIWND* pParentWnd)
{
	if (m_ButtonInfos.empty() == true)
		return 0;
	for (S_ButtonInfos::const_iterator iter = m_ButtonInfos.begin(); iter != m_ButtonInfos.end(); iter += 1)
	{
		S_RECT Rect;
		C_ButtonEx *pButton = C_ButtonEx::NewCtrl();
		Rect.x = iter->m_x, Rect.y = iter->m_y;
		Rect.w = iter->m_w, Rect.h = iter->m_h;
		pButton->Create(iter->m_strTitle.c_str(), HGUI_BS_PUSHBTN, Rect, pParentWnd, NULL, NULL, NULL, iter->m_ID);
	}

	return (int)m_ButtonInfos.size();
}

string C_LuaCtrl::GetFunctionWithID(S_WORD CtrlID)
{
	if (m_ButtonInfos.empty() == false)
	{
		for (S_ButtonInfos::const_iterator iter = m_ButtonInfos.begin(); iter != m_ButtonInfos.end(); iter += 1)
		{
			if (iter->m_ID == CtrlID)
				return iter->m_strFunction;
		}
	}
	return string("");
}


