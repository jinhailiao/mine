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

enum
{
	HGUI_BLACKNESS,
	HGUI_CAPTUREBLT,
	HGUI_DSTINVERT,
	HGUI_MERGECOPY,
	HGUI_MERGEPAINT,
	HGUI_NOMIRRORBITMAP,
	HGUI_NOTSRCCOPY,
	HGUI_NOTSRCERASE,
	HGUI_PATCOPY,
	HGUI_PATINVERT,
	HGUI_PATPAINT,
	HGUI_SRCAND,
	HGUI_SRCCOPY,
	HGUI_SRCERASE,
	HGUI_SRCINVERT,
	HGUI_SRCPAINT,
	HGUI_WHITENESS,
	HGUI_BRIGHTCOPY,
	HGUI_ALPHACOPY,
	HGUI_TRANSCOPY,
	HGUI_TRANSALPHACPY,

	HGUI_BITBLT_MAX
};

class C_WNDBASE;
class C_HGUIDC:public C_HGUIOBJ
{
public:
	C_HGUIDC(C_WNDBASE *pWnd = NULL);
	virtual ~C_HGUIDC();

	S_DWORD SetTextColor(S_DWORD TextColor){S_DWORD old = m_TextColor;m_TextColor = TextColor;return old;}
	S_DWORD GetTextColor(void)const{return m_TextColor;}
	S_DWORD SetColorKey(S_DWORD ColorKey){S_DWORD old = m_ColorKey;m_ColorKey = ColorKey;return old;}
	S_DWORD GetColorKey(void)const{return m_ColorKey;}
	S_BYTE SetAlphaVal(S_BYTE alpha){S_BYTE old = m_Alpha;m_Alpha = alpha;return old;}
	S_BYTE GetAlphaVal(void)const{return m_Alpha;}
	S_SHORT SetBrightVal(S_SHORT bright){return (S_SHORT)SetColorKey(bright);}
	S_SHORT GetBrightVal(void)const{return (S_SHORT)m_ColorKey;}
	S_DWORD SetBkColor(S_DWORD BkColor){S_DWORD old = m_BkColor;m_BkColor = BkColor;return old;}
	S_DWORD GetBkColor(void)const{return m_BkColor;}
	S_BYTE SetBkMode(S_BYTE BkMode){S_BYTE old = m_BkMode;m_BkMode = BkMode;return old;}
	S_BYTE GetBkMode(void)const{return m_BkMode;}

	virtual int DrawPixel(S_WORD x, S_WORD y);
	virtual int DrawHLine(S_WORD x, S_WORD y, S_WORD w);
	virtual int DrawVLine(S_WORD x, S_WORD y, S_WORD h);
	virtual int DrawLine(S_WORD sx, S_WORD sy, S_WORD ex, S_WORD ey);
	virtual int DrawRect(const S_RECT &rRect);
	virtual int FillRect(const S_RECT &rRect);
	virtual int ReverseRect(const S_RECT &rRect);
	virtual int DrawString(S_WORD x, S_WORD y, const char *pText);
	virtual int GetStringExtent(const char *pText);

	C_HGUIOBJ *GetStockGuiObj(S_BYTE ObjName)const;
	C_HGUIOBJ *SelectObject(C_HGUIOBJ *pObj);

	int GetFontHeight(S_WORD font)const{if (m_pFont) return m_pFont->GetHeight(font);return 0;}
	int GetFontWidth(S_WORD font)const{if (m_pFont) return m_pFont->GetWidth(font);return 0;}

	bool FlushScreenEn(bool enable);
	bool FlushScreen(void);

	bool StretchBlit(S_WORD dx, S_WORD dy, S_WORD dw, S_WORD dh, const C_HGUIDC *pdcSrc, S_WORD sx, S_WORD sy, S_WORD sw, S_WORD sh);
	bool BitBlt(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIDC *pdcSrc, S_WORD sx, S_WORD sy, S_DWORD dwRop);
	bool CreateCompatibleDC(const C_HGUIDC *pDC);
	bool DrawBitmap(S_WORD x, S_WORD y, C_HGUIBMP *pbitmap);
	bool DeleteObject(void);

protected:
	int ClientToScreen(S_RECT &rRect)const;

	bool _BitBlt1(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIDC *pdcSrc, S_WORD sx, S_WORD sy, S_DWORD dwRop);
	bool _BitBlt2(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, S_DWORD dwRop);
	bool _BitBlt3(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIDC *pdcSrc, S_WORD sx, S_WORD sy, S_DWORD dwRop);

protected:
	S_RECT m_Rect;
	S_BYTE m_BkMode;
	S_BYTE m_Alpha;
	S_DWORD m_ColorKey;
	S_DWORD m_TextColor;
	S_DWORD m_BkColor;

	C_WNDBASE  *m_pWnd;
	C_HGUIPEN  *m_pPen;
	C_HGUIFONT *m_pFont;
	C_HGUIBMP  *m_pBMP;
};

#endif //__HGUI_DC_H__

