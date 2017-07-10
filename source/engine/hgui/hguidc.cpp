//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguidc.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "hguidc.h"
#include "hguibase.h"
#include "hguiwnd.h"


C_HGUIDC::C_HGUIDC(C_WNDBASE *pWnd):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_DC)
{
	if (pWnd == NULL)
	{//screen dc
		m_Rect.x = 0; m_Rect.w = HGui_LcdWidth();
		m_Rect.y = 0; m_Rect.h = HGui_LcdHeight();
	}
	else
	{
		m_Rect = pWnd->GetWndRect();
	}

	m_ColorKey	= 0;
	m_TextColor	= HGUI_COLOR_BLACK;
	m_BkColor	= HGUI_COLOR_WHITE;
	m_BkMode	= HGUI_BKM_OPAQUE;
	m_Alpha		= 255;

	m_pWnd  = pWnd;
	m_pBMP  = &HGUI_LcdScrn;
	SelectObject(GetStockGuiObj(HGUI_OBJ_BPEN));
	SelectObject(GetStockGuiObj(HGUI_OBJ_F12x12));
}

C_HGUIDC::~C_HGUIDC()
{
}

int C_HGUIDC::DrawPixel(S_WORD x, S_WORD y)
{
	S_RECT rect = {x, y, 1, 1};
	ClientToScreen(rect);

	if (m_pPen->SetPixel(rect.x, rect.y, m_pBMP) == 0)
		FlushScreen();
	return 0;
}

int C_HGUIDC::DrawHLine(S_WORD x, S_WORD y, S_WORD w)
{
	S_RECT rect = {x, y, w, 1};
	ClientToScreen(rect);

	if (m_pPen->DrawHLine(rect.x, rect.y, rect.w, m_pBMP) == 0)
		FlushScreen();
	return 0;
}

int C_HGUIDC::DrawVLine(S_WORD x, S_WORD y, S_WORD h)
{
	S_RECT rect = {x, y, 1, h};
	ClientToScreen(rect);

	if (m_pPen->DrawVLine(rect.x, rect.y, rect.h, m_pBMP) == 0)
		FlushScreen();
	return 0;
}

int C_HGUIDC::DrawLine(S_WORD sx, S_WORD sy, S_WORD ex, S_WORD ey)
{
	S_RECT sRect = {sx, sy, 1, 1};
	S_RECT eRect = {ex, ey, 1, 1};
	ClientToScreen(sRect);
	ClientToScreen(eRect);

	if (m_pPen->DrawLine(sRect.x, sRect.y, eRect.x, eRect.y, m_BkMode, m_BkColor, m_pBMP) == 0)
		FlushScreen();
	return 0;
}

int C_HGUIDC::DrawRect(const S_RECT &rRect)
{
	S_RECT rect = rRect;
	ClientToScreen(rect);

	m_pPen->DrawHLine(rect.x, rect.y, rect.w, m_pBMP);
	m_pPen->DrawHLine(rect.x, rect.y + rect.h - 1, rect.w, m_pBMP);
	m_pPen->DrawVLine(rect.x, rect.y, rect.h, m_pBMP);
	m_pPen->DrawVLine(rect.x + rect.w - 1, rect.y, rect.h, m_pBMP);

	FlushScreen();
	return 0;
}

int C_HGUIDC::FillRect(const S_RECT &rRect)
{//no brush ,then use pen
	S_RECT rect = rRect;
	ClientToScreen(rect);

	for (int i = rect.y; i < rect.y + rect.h; ++i)
		m_pPen->DrawHLine(rect.x, i, rect.w, m_pBMP);

	FlushScreen();
	return 0;
}

int C_HGUIDC::ReverseRect(const S_RECT &rRect)
{
	S_RECT rect = rRect;
	ClientToScreen(rect);

	for (int i = rect.y; i < rect.y + rect.h; ++i)
		m_pBMP->FlipHLine(rect.x, i, rect.w);
	FlushScreen();
	return 0;
}

int C_HGUIDC::DrawString(S_WORD x, S_WORD y, const char *pText)
{
	S_RECT rect = {x, y, 1, 1};
	ClientToScreen(rect);
	x = rect.x, y = rect.y;

	S_BYTE BkMode = m_pFont->SetBkMode(m_BkMode);
	S_DWORD TextColor = m_pFont->SetTextColor(m_TextColor);
	S_DWORD BkColor = m_pFont->SetBkColor(m_BkColor);

	while (S_BYTE ch = *pText++)
	{
		if (ch & 0x80)//Hz font
		{
			S_WORD Hz = ((S_WORD)ch<<8) + ((S_WORD)(*pText++)&0xFF);
			m_pFont->DrawFont(x, y, Hz, m_pBMP);
			x += m_pFont->GetWidth(Hz);
		}
		else//english font
		{
			m_pFont->DrawChar(x, y, ch, m_pBMP);
			x += m_pFont->GetWidth(ch);
		}
	}

	m_pFont->SetBkMode(BkMode);
	m_pFont->SetTextColor(TextColor);
	m_pFont->SetBkColor(BkColor);

	FlushScreen();
	return 0;
}

int C_HGUIDC::GetStringExtent(const char *pText)
{
	if (pText == NULL)
		return 0;

	int size = 0;
	while (S_BYTE ch = *pText++)
	{
		if (ch & 0x80)//Hz font
		{
			S_WORD Hz = ((S_WORD)ch<<8) + ((S_WORD)(*pText++)&0xFF);
			size += m_pFont->GetWidth(Hz);
		}
		else//english font
		{
			size += m_pFont->GetWidth(ch);
		}
	}
	return size;
}

C_HGUIOBJ *C_HGUIDC::GetStockGuiObj(S_BYTE ObjName)const
{
	static C_HGUIPEN HGUI_BPen(HGUI_COLOR_BLACK);
	static C_HGUIPEN HGUI_WPen(HGUI_COLOR_WHITE);
	static C_HGUIF12x12 HGUI_F12x12;
	static C_HGUIF16x16 HGUI_F16x16;
	static C_HGUIE5x8 HGUI_E5x8;
	static C_HGUISYM16x16 HGUI_SYM16x16;
	static C_HGUISYM08x16 HGUI_SYM08x16;

	switch (ObjName)
	{
	case HGUI_OBJ_BPEN:
		return &HGUI_BPen;
	case HGUI_OBJ_WPEN:
		return &HGUI_WPen;
	case HGUI_OBJ_F12x12:
		return &HGUI_F12x12;
	case HGUI_OBJ_F16x16:
		return &HGUI_F16x16;
	case HGUI_OBJ_E5x8:
		return &HGUI_E5x8;
	case HGUI_OBJ_SYM16x16:
		return &HGUI_SYM16x16;
	case HGUI_OBJ_SYM08x16:
		return &HGUI_SYM08x16;
	default:
		break;
	}
	return NULL;
}

C_HGUIOBJ *C_HGUIDC::SelectObject(C_HGUIOBJ *pObj)
{
	C_HGUIOBJ *pOld = NULL;

	if (pObj == NULL)
		return pOld;

	switch (pObj->GetObjType())
	{
	case C_HGUIOBJ::OBJ_T_PEN:
		pOld = m_pPen;
		m_pPen = dynamic_cast<C_HGUIPEN*>(pObj);
		break;
	case C_HGUIOBJ::OBJ_T_FONT:
		pOld = m_pFont;
		m_pFont = dynamic_cast<C_HGUIFONT*>(pObj);
		break;
	case C_HGUIOBJ::OBJ_T_BMP:
		pOld = m_pBMP;
		m_pBMP = dynamic_cast<C_HGUIBMP*>(pObj);
		m_Rect.x = 0, m_Rect.y = 0;
		m_Rect.w = m_pBMP->GetWidth();
		m_Rect.h = m_pBMP->GetHeight();
		break;
	default:
		break;
	}
	return pOld;
}

bool C_HGUIDC::FlushScreenEn(bool enable)
{
	return HGui_FlushScreenEn(enable);
}

bool C_HGUIDC::FlushScreen(void)
{
	if (m_pBMP->GetBmpData() != HGui_LcdBuffer())
		return false;
	
	if (m_pWnd == NULL)
	{
		HGui_flushScreen(0, 0, HGui_LcdWidth(), HGui_LcdHeight());
	}
	else
	{
		if (m_pWnd->InvalidRect() == false)
			m_pWnd->UpdateWnd();
	}
	return true;
}

bool C_HGUIDC::StretchBlit(S_WORD dx, S_WORD dy, S_WORD dw, S_WORD dh, const C_HGUIDC *pdcSrc, S_WORD sx, S_WORD sy, S_WORD sw, S_WORD sh)
{
	if (dw == 0) dw = m_Rect.w;
	if (dh == 0) dh = m_Rect.h;
	if (sw == 0) sw = pdcSrc->m_Rect.w;
	if (sh == 0) sh = pdcSrc->m_Rect.h;

	S_RECT dRect = {dx, dy, dw, dh};
	ClientToScreen(dRect);

	S_RECT sRect = {sx, sy, sw, sh};
	pdcSrc->ClientToScreen(sRect);

	return m_pBMP->StretchBlit(dRect, pdcSrc->m_pBMP, sRect);
}

bool C_HGUIDC::BitBlt(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIDC *pdcSrc, S_WORD sx, S_WORD sy, S_DWORD dwRop)
{
	switch (dwRop)
	{
	case HGUI_NOTSRCCOPY:
	case HGUI_SRCAND:
	case HGUI_SRCCOPY:
	case HGUI_SRCINVERT:
		return _BitBlt1(dx, dy, w, h, pdcSrc, sx, sy, dwRop);

	case HGUI_BLACKNESS:
	case HGUI_DSTINVERT:
	case HGUI_WHITENESS:	
	case HGUI_PATINVERT:
	case HGUI_PATCOPY:
	case HGUI_PATPAINT:
		return _BitBlt2(dx, dy, w, h, dwRop);

	case HGUI_BRIGHTCOPY:
	case HGUI_ALPHACOPY:
	case HGUI_TRANSCOPY:
	case HGUI_TRANSALPHACPY:
		return _BitBlt3(dx, dy, w, h, pdcSrc, sx, sy, dwRop);
	
	default:
		break;
	}
	return false;
}

bool C_HGUIDC::CreateCompatibleDC(const C_HGUIDC *pDC)
{
	*this = *pDC;
	m_pWnd = NULL;
	m_Rect.x = 0, m_Rect.y = 0;
	m_Rect.w = m_pBMP->GetWidth();
	m_Rect.h = m_pBMP->GetHeight();
	return true;
}

bool C_HGUIDC::DrawBitmap(S_WORD x, S_WORD y, C_HGUIBMP *pbitmap)
{
	C_HGUIDC dcMem(NULL);
	dcMem.CreateCompatibleDC(this);
	dcMem.SelectObject(pbitmap);
	return BitBlt(x, y, pbitmap->GetWidth(), pbitmap->GetHeight(), &dcMem, 0, 0, HGUI_SRCCOPY);
}

bool C_HGUIDC::DeleteObject(void)
{
	return true;
}

int C_HGUIDC::ClientToScreen(S_RECT &rRect)const
{
	if (m_pWnd == NULL)
	{
		rRect.x += m_Rect.x, rRect.y += m_Rect.y;
		if (rRect.x+rRect.w > m_Rect.x+m_Rect.w)
			rRect.w = m_Rect.x+m_Rect.w - rRect.x;
		if (rRect.y+rRect.h > m_Rect.y+m_Rect.h)
			rRect.h = m_Rect.y+m_Rect.h - rRect.y;
	}
	else
	{
		m_pWnd->ClientToScreen(rRect);
	}
	return 0;
}

bool C_HGUIDC::_BitBlt1(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIDC *pdcSrc, S_WORD sx, S_WORD sy, S_DWORD dwRop)
{
	if (pdcSrc == NULL)
		return false;
	if (m_pBMP->GetBitsPixel() != pdcSrc->m_pBMP->GetBitsPixel())
		return false;

	if (dx >= m_Rect.w || dy >= m_Rect.h)
		return false;
	if (sx >= pdcSrc->m_Rect.w || sy >= pdcSrc->m_Rect.h)
		return false;

	if (w <= 0) w = m_Rect.w;
	w = HAI_MIN(w, m_Rect.w);
	w = HAI_MIN(w, pdcSrc->m_Rect.w);
	if (h <= 0) h = m_Rect.h;
	h = HAI_MIN(h, m_Rect.h);
	h = HAI_MIN(h, pdcSrc->m_Rect.h);

	S_RECT dRect = {dx, dy, w, h};
	ClientToScreen(dRect);
	S_RECT sRect = {sx, sy, w, h};
	pdcSrc->ClientToScreen(sRect);

	switch (dwRop)
	{
	case HGUI_NOTSRCCOPY:
		return m_pBMP->BitBltNotSrcCopy(dRect.x, dRect.y, w, h, pdcSrc->m_pBMP, sRect.x, sRect.y);
	case HGUI_SRCAND:
		return m_pBMP->BitBltSrcAnd(dRect.x, dRect.y, w, h, pdcSrc->m_pBMP, sRect.x, sRect.y);
	case HGUI_SRCCOPY:
		return m_pBMP->BitBltSrcCopy(dRect.x, dRect.y, w, h, pdcSrc->m_pBMP, sRect.x, sRect.y);
	case HGUI_SRCINVERT:
		return m_pBMP->BitBltSrcInvert(dRect.x, dRect.y, w, h, pdcSrc->m_pBMP, sRect.x, sRect.y);
	default:
		break;
	}

	return false;
}

bool C_HGUIDC::_BitBlt2(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, S_DWORD dwRop)
{
	if (dx >= m_Rect.w || dy >= m_Rect.h)
		return false;

	if (w <= 0) w = m_Rect.w;
	w = HAI_MIN(w, m_Rect.w);
	if (h <= 0) h = m_Rect.h;
	h = HAI_MIN(h, m_Rect.h);

	S_RECT dRect = {dx, dy, w, h};
	ClientToScreen(dRect);

	switch (dwRop)
	{
	case HGUI_BLACKNESS:
		return m_pBMP->BitBltBlackness(dRect.x, dRect.y, w, h);
	case HGUI_WHITENESS:
		return m_pBMP->BitBltWhiteness(dRect.x, dRect.y, w, h);
	case HGUI_DSTINVERT:
		return m_pBMP->BitBltDstinvert(dRect.x, dRect.y, w, h);
	case HGUI_PATINVERT:
		return m_pBMP->BitBltPatinvert(dRect.x, dRect.y, w, h, m_pPen->GetColor());
	case HGUI_PATPAINT:
		return m_pBMP->BitBltPatpaint(dRect.x, dRect.y, w, h, m_pPen->GetColor());
	case HGUI_PATCOPY:
		return m_pBMP->BitBltPatcopy(dRect.x, dRect.y, w, h, m_pPen->GetColor());
	default:
		break;
	}
	return false;
}

bool C_HGUIDC::_BitBlt3(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIDC *pdcSrc, S_WORD sx, S_WORD sy, S_DWORD dwRop)
{
	if (pdcSrc == NULL)
		return false;
	if (m_pBMP->GetBitsPixel() != pdcSrc->m_pBMP->GetBitsPixel())
		return false;

	if (dx >= m_Rect.w || dy >= m_Rect.h)
		return false;
	if (sx >= pdcSrc->m_Rect.w || sy >= pdcSrc->m_Rect.h)
		return false;

	if (w <= 0) w = m_Rect.w;
	w = HAI_MIN(w, m_Rect.w);
	w = HAI_MIN(w, pdcSrc->m_Rect.w);
	if (h <= 0) h = m_Rect.h;
	h = HAI_MIN(h, m_Rect.h);
	h = HAI_MIN(h, pdcSrc->m_Rect.h);

	S_RECT dRect = {dx, dy, w, h};
	ClientToScreen(dRect);
	S_RECT sRect = {sx, sy, w, h};
	pdcSrc->ClientToScreen(sRect);

	switch (dwRop)
	{
	case HGUI_BRIGHTCOPY:
		return m_pBMP->BitBltBrightCopy(dRect.x, dRect.y, w, h, pdcSrc->m_pBMP, sRect.x, sRect.y, pdcSrc->m_ColorKey);
	case HGUI_ALPHACOPY:
		return m_pBMP->BitBltAlphaCopy(dRect.x, dRect.y, w, h, pdcSrc->m_pBMP, sRect.x, sRect.y, pdcSrc->m_Alpha);
	case HGUI_TRANSCOPY:
		return m_pBMP->BitBltTransCopy(dRect.x, dRect.y, w, h, pdcSrc->m_pBMP, sRect.x, sRect.y, pdcSrc->m_ColorKey);
	case HGUI_TRANSALPHACPY:
		return m_pBMP->BitBltTransAlphaCpy(dRect.x, dRect.y, w, h, pdcSrc->m_pBMP, sRect.x, sRect.y, pdcSrc->m_Alpha, pdcSrc->m_ColorKey);
	default:
		break;
	}

	return false;
}



