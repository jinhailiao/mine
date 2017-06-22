//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguicart.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_CARET_H__
#define __HGUI_CARET_H__

#include "hguiwnd.h"

class C_HGUICARET:public C_WNDBASE
{
public:
	C_HGUICARET(C_WNDBASE *pPwnd, S_WORD w, S_WORD h);
	~C_HGUICARET();

public:
	bool GetCaretStatus(void){return m_show;}

protected:
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

protected:
	bool m_show;
};

#endif //__HGUI_CARET_H__



