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
#include "hguidef.h"
#include "haibase.h"
#include "hguiobj.h"
#include "hguibmp.h"

#define  HGUI_PS_NONE		0
#define  HGUI_PS_SOLID		1
#define  HGUI_PS_DOT			2
#define  HGUI_PS_DASH		3
#define  HGUI_PS_DASHDOT		4
#define  HGUI_PS_DASHDOTDOT	5
#define  HGUI_PS_DASHDASHDOT 6

class C_HGUIPEN:public C_HGUIOBJ
{
public:
	C_HGUIPEN(S_DWORD color);
	C_HGUIPEN(S_DWORD color, S_DWORD style);
	virtual ~C_HGUIPEN();

	S_DWORD SetColor(S_DWORD color){S_DWORD old = m_color;m_color = color;return old;}
	S_DWORD GetColor(void){return m_color;}
	S_DWORD SetStyle(S_DWORD style){S_DWORD old = m_style;m_style = style;return old;}
	S_DWORD GetStyle(void){return m_style;}
	int ReverseColor(void){m_color = ~m_color; return m_color;}

	virtual int SetPixel(S_WORD x, S_WORD y, C_HGUIBMP *pBMP);
	virtual int GetPixel(S_WORD x, S_WORD y, C_HGUIBMP *pBMP);
	virtual int DrawHLine(S_WORD x, S_WORD y, S_WORD w, C_HGUIBMP *pBMP);
	virtual int DrawVLine(S_WORD x, S_WORD y, S_WORD h, C_HGUIBMP *pBMP);
	virtual int DrawLine(S_WORD sx, S_WORD sy, S_WORD ex, S_WORD ey, S_BYTE BkMode, S_DWORD BkColor, C_HGUIBMP *pBMP);

protected:
	S_DWORD m_color;
	S_DWORD m_style;
};


#endif //__HGUI_PEN_H__

