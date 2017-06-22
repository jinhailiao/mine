//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguipen.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-19  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_PEN_H__
#define __HGUI_PEN_H__
#include "haibase.h"
#include "hguiobj.h"
#include "hguibmp.h"

class C_HGUIPEN:public C_HGUIOBJ
{
public:
	C_HGUIPEN(S_DWORD color);
	virtual ~C_HGUIPEN();

	int ReverseColor(void){m_color = ~m_color; return m_color;}

	virtual int SetPixel(S_WORD x, S_WORD y, C_HGUIBMP *pBMP);
	virtual int GetPixel(S_WORD x, S_WORD y, C_HGUIBMP *pBMP);
	virtual int DrawHLine(S_WORD x, S_WORD y, S_WORD w, C_HGUIBMP *pBMP);
	virtual int DrawVLine(S_WORD x, S_WORD y, S_WORD h, C_HGUIBMP *pBMP);

protected:
	S_DWORD m_color;
};


#endif //__HGUI_PEN_H__

