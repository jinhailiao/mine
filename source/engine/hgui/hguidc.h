//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguidc.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-19  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_DC_H__
#define __HGUI_DC_H__
#include "haibase.h"
#include "hguiobj.h"
#include "hguibmp.h"
#include "hguipen.h"
#include "hguifont.h"
#include "hguibase.h"

enum
{
	HGUI_OBJ_UNKNOW,

	HGUI_OBJ_BPEN,
	HGUI_OBJ_WPEN,
	HGUI_OBJ_F12x12,
	HGUI_OBJ_F16x16,
	HGUI_OBJ_E5x8,
	HGUI_OBJ_SYM16x16,
	HGUI_OBJ_SYM08x16,

	HGUI_OBJ_MAX
};

class C_WNDBASE;
class C_HGUIDC:public C_HGUIOBJ
{
public:
	C_HGUIDC(C_WNDBASE *pWnd = NULL);
	virtual ~C_HGUIDC();

	virtual int DrawPixel(S_WORD x, S_WORD y);
	virtual int DrawHLine(S_WORD x, S_WORD y, S_WORD w);
	virtual int DrawVLine(S_WORD x, S_WORD y, S_WORD h);
	virtual int DrawRect(const S_RECT &rRect);
	virtual int FillRect(const S_RECT &rRect);
	virtual int ReverseRect(const S_RECT &rRect);
	virtual int DrawString(S_WORD x, S_WORD y, const char *pText);
	virtual int GetStringExtent(const char *pText);

	C_HGUIOBJ *GetStockGuiObj(S_BYTE ObjName);
	C_HGUIOBJ *SelectObject(C_HGUIOBJ *pObj);

	int GetFontHeight(S_WORD font)const{if (m_pFont) return m_pFont->GetHeight(font);return 0;}
	int GetFontWidth(S_WORD font)const{if (m_pFont) return m_pFont->GetWidth(font);return 0;}

	bool FlushScreenEn(bool enable);
	bool FlushScreen(void);

protected:
	int ClientToScreen(S_RECT &rRect);

protected:
	S_RECT m_Rect;
	C_WNDBASE  *m_pWnd;
	C_HGUIPEN  *m_pPen;
	C_HGUIFONT *m_pFont;
	C_HGUIBMP  *m_pBMP;
};

#endif //__HGUI_DC_H__

