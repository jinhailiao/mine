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

C_HGUIOBJ *C_HGUIDC::GetStockGuiObj(S_BYTE ObjName)
{
	static C_HGUIPEN HGUI_BPen(1);
	static C_HGUIPEN HGUI_WPen(0);
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

bool C_HGUIDC::BitBlt(S_SHORT xDst, S_SHORT yDst, S_SHORT w, S_SHORT h, C_HGUIDC *pdcSrc, S_SHORT xSrc, S_SHORT ySrc, S_DWORD dwRop)
{
	return true;
}

bool C_HGUIDC::CreateCompatibleDC(C_HGUIDC *pDC)
{
	return true;
}

bool C_HGUIDC::DeleteObject(void)
{
	return true;
}

int C_HGUIDC::ClientToScreen(S_RECT &rRect)
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



