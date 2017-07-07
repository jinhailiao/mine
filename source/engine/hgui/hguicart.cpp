//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguicart.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "hguicart.h"

C_HGUICARET::C_HGUICARET(C_WNDBASE *pPwnd, S_WORD w, S_WORD h)
{
	m_ObjT = C_HGUIOBJ::OBJ_T_CARET;
	m_pParent = pPwnd;
	m_WndRect.x = 0;
	m_WndRect.y = 0;
	m_WndRect.w = w;
	m_WndRect.h = h;
	m_flag = 0;
	m_show = false;
}

C_HGUICARET::~C_HGUICARET()
{
}

int C_HGUICARET::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	switch (evt)
	{
	case EVT_CREATE:
		break;
	case EVT_PAINT:
	case EVT_TIMER:
		if ((m_flag & HGUI_WNDF_VISIBLE) && !(m_flag & HGUI_WNDF_DISABLED))
		{
			C_HGUIDC dc(this);
			S_RECT rect = m_WndRect;
			rect.x = 0, rect.y = 0;
			m_show = !m_show;
			dc.ReverseRect(rect);
		}
		break;
	default:
		return C_WNDBASE::DefWndProcess(evt, wParam, lParam);
	}
	return 0;
}

