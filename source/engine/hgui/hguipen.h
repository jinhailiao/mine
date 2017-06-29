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

#define  HGUI_LCD_BITSPERPIX		24

#ifdef HGUI_16BITS_COLOR_REVERSION
#define  HGUI_COLOR_GRAY00	0xF
#define  HGUI_COLOR_GRAY01	0xE
#define  HGUI_COLOR_GRAY02	0xD
#define  HGUI_COLOR_GRAY03	0xC
#define  HGUI_COLOR_GRAY04	0xB
#define  HGUI_COLOR_GRAY05	0xA
#define  HGUI_COLOR_GRAY06	0x9
#define  HGUI_COLOR_GRAY07	0x8
#define  HGUI_COLOR_GRAY08	0x7
#define  HGUI_COLOR_GRAY09	0x6
#define  HGUI_COLOR_GRAY10	0x5
#define  HGUI_COLOR_GRAY11	0x4
#define  HGUI_COLOR_GRAY12	0x3
#define  HGUI_COLOR_GRAY13	0x2
#define  HGUI_COLOR_GRAY14	0x1
#define  HGUI_COLOR_GRAY15	0x0
#else
#define  HGUI_COLOR_GRAY00	0x0
#define  HGUI_COLOR_GRAY01	0x1
#define  HGUI_COLOR_GRAY02	0x2
#define  HGUI_COLOR_GRAY03	0x3
#define  HGUI_COLOR_GRAY04	0x4
#define  HGUI_COLOR_GRAY05	0x5
#define  HGUI_COLOR_GRAY06	0x6
#define  HGUI_COLOR_GRAY07	0x7
#define  HGUI_COLOR_GRAY08	0x8
#define  HGUI_COLOR_GRAY09	0x9
#define  HGUI_COLOR_GRAY10	0xA
#define  HGUI_COLOR_GRAY11	0xB
#define  HGUI_COLOR_GRAY12	0xC
#define  HGUI_COLOR_GRAY13	0xD
#define  HGUI_COLOR_GRAY14	0xE
#define  HGUI_COLOR_GRAY15	0xF
#endif

#if (HGUI_LCD_BITSPERPIX == 4)

#define  HGUI_COLOR_WHITE		HGUI_COLOR_GRAY15
#define  HGUI_COLOR_LIGHT		HGUI_COLOR_GRAY12
#define  HGUI_COLOR_LIGRAY		HGUI_COLOR_GRAY10
#define  HGUI_COLOR_GRAY		HGUI_COLOR_GRAY08
#define  HGUI_COLOR_LIDARK		HGUI_COLOR_GRAY06
#define  HGUI_COLOR_DARK		HGUI_COLOR_GRAY03
#define  HGUI_COLOR_BLACK		HGUI_COLOR_GRAY00

#elif (HGUI_LCD_BITSPERPIX == 8)

#define  HGUI_COLOR_WHITE	0xFF
#define  HGUI_COLOR_LIGHT	0xD2
#define  HGUI_COLOR_LIGRAY	0xA8
#define  HGUI_COLOR_GRAY		0x7E
#define  HGUI_COLOR_LIDARK	0x54
#define  HGUI_COLOR_DARK		0x2A
#define  HGUI_COLOR_BLACK	0x00

#elif (HGUI_LCD_BITSPERPIX == 16)

#define  HGUI_COLOR_WHITE	0xFFFF
#define  HGUI_COLOR_LIGHT	0xCE59
#define  HGUI_COLOR_LIGRAY	0xA514
#define  HGUI_COLOR_GRAY	0x7BCF
#define  HGUI_COLOR_LIDARK	0x528A
#define  HGUI_COLOR_DARK	0x2945
#define  HGUI_COLOR_BLACK	0x0000

#elif (HGUI_LCD_BITSPERPIX == 24)

#define  HGUI_COLOR_WHITE	0xFFFFFF
#define  HGUI_COLOR_LIGHT	0xD2D2D2
#define  HGUI_COLOR_LIGRAY	0xA8A8A8
#define  HGUI_COLOR_GRAY		0x7E7E7E
#define  HGUI_COLOR_LIDARK	0x545454
#define  HGUI_COLOR_DARK		0x2A2A2A
#define  HGUI_COLOR_BLACK	0x000000

#else
#error Unsupport color depth!
#endif

#define  HGUI_BKM_OPAQUE			0
#define  HGUI_BKM_TRANSPARENT	1

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

