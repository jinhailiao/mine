//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguipen.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "hguipen.h"

C_HGUIPEN::C_HGUIPEN(S_DWORD color):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_PEN)
{
	m_color = color;
	m_style = HGUI_PS_NONE;
}

C_HGUIPEN::C_HGUIPEN(S_DWORD color, S_DWORD style):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_PEN)
{
	m_color = color;
	m_style = style;
}

C_HGUIPEN::~C_HGUIPEN()
{
}

int C_HGUIPEN::SetPixel(S_WORD x, S_WORD y, C_HGUIBMP *pBMP)
{
	return pBMP->SetPixel(x, y, m_color);
}

int C_HGUIPEN::GetPixel(S_WORD x, S_WORD y, C_HGUIBMP *pBMP)
{
	m_color = pBMP->GetPixel(x, y);
	return m_color;
}

int C_HGUIPEN::DrawHLine(S_WORD x, S_WORD y, S_WORD w, C_HGUIBMP *pBMP)
{
	return pBMP->FillHLine(x, y, w, m_color);
}

int C_HGUIPEN::DrawVLine(S_WORD x, S_WORD y, S_WORD h, C_HGUIBMP *pBMP)
{
	for (S_WORD i = y; i < y + h; ++i)
		pBMP->SetPixel(x, i, m_color);

	return 0;
}

int C_HGUIPEN::DrawLine(S_WORD sx, S_WORD sy, S_WORD ex, S_WORD ey, S_BYTE BkMode, S_DWORD BkColor, C_HGUIBMP *pBMP)
{
	S_WORD SX = sx, SY = sy;
	S_WORD EX = ex, EY = ey;

	if (SX>EX)
	{
		EX=sx, SX=ex;
		EY=sy, SY=ey;
	}

	S_SHORT W = (EX-SX);
	S_SHORT H = (EY-SY);
	S_SHORT Ha= H<0 ? -H : H;
	S_SHORT dY= H<0 ? -1 : 1;

	if (Ha>W)
	{
		/* Y-majored */
		S_WORD f = Ha>>1;
		while (1)
		{
			switch (m_style)
			{
				case HGUI_PS_NONE:
					break;
				case HGUI_PS_DOT://broken line
					if (SY % 2)
						pBMP->SetPixel(SX, SY, m_color);
					else if (BkMode == HGUI_BKM_OPAQUE)
						pBMP->SetPixel(SX, SY, BkColor);
					break;
				case HGUI_PS_DASH://两点实，一点虚
					if (SY % 3)
						pBMP->SetPixel(SX, SY, m_color);
					else if (BkMode == HGUI_BKM_OPAQUE)
						pBMP->SetPixel(SX, SY, BkColor);
					break;
				case HGUI_PS_DASHDOT:
					switch (SY % 5)
					{
					case 0:
					case 1:
					case 3:
						pBMP->SetPixel(SX, SY, m_color);
						break;
					default:
						if (BkMode == HGUI_BKM_OPAQUE)
							pBMP->SetPixel(SX, SY, BkColor);
						break;
					}
					break;
				case HGUI_PS_DASHDOTDOT:
					switch (SY % 7)
					{
					case 0:
					case 1:
					case 3:
					case 5:
						pBMP->SetPixel(SX, SY, m_color);
						break;
					default:
						if (BkMode == HGUI_BKM_OPAQUE)
							pBMP->SetPixel(SX, SY, BkColor);
						break;
					}
					break;
				case HGUI_PS_DASHDASHDOT:
					switch (SY % 9)
					{
					case 0:
					case 1:
					case 2:
					case 3:
					case 6:
						pBMP->SetPixel(SX, SY, m_color);
						break;
					default:
						if (BkMode == HGUI_BKM_OPAQUE)
							pBMP->SetPixel(SX, SY, BkColor);
						break;
					}
					break;
				default:
					pBMP->SetPixel(SX, SY, m_color);
					break;
			}
			if (SY==EY) break;
			if ((f+=W)>=Ha)
				f-=Ha,SX++;
			SY+=dY;
		}
	}
	else
	{
		/* X-majored */
		S_WORD f = W>>1;
		while (1)
		{
			switch (m_style)
			{
				case HGUI_PS_NONE:
					break;
				case HGUI_PS_DOT:
					if (SX % 2)
						pBMP->SetPixel(SX, SY, m_color);
					else if (BkMode == HGUI_BKM_OPAQUE)
						pBMP->SetPixel(SX, SY, BkColor);
					break;
				case HGUI_PS_DASH:
					if (SX % 3)
						pBMP->SetPixel(SX, SY, m_color);
					else if (BkMode == HGUI_BKM_OPAQUE)
						pBMP->SetPixel(SX, SY, BkColor);
					break;
				case HGUI_PS_DASHDOT:
					switch (SX % 5)
					{
					case 0:
					case 1:
					case 3:
						pBMP->SetPixel(SX, SY, m_color);
						break;
					default:
						if (BkMode == HGUI_BKM_OPAQUE)
							pBMP->SetPixel(SX, SY, BkColor);
						break;
					}
					break;
				case HGUI_PS_DASHDOTDOT:
					switch (SX % 7)
					{
					case 0:
					case 1:
					case 3:
					case 5:
						pBMP->SetPixel(SX, SY, m_color);
						break;
					default:
						if (BkMode == HGUI_BKM_OPAQUE)
							pBMP->SetPixel(SX, SY, BkColor);
						break;
					}
					break;
				case HGUI_PS_DASHDASHDOT:
					switch (SX % 9)
					{
					case 0:
					case 1:
					case 2:
					case 3:
					case 6:
						pBMP->SetPixel(SX, SY, m_color);
						break;
					default:
						if (BkMode == HGUI_BKM_OPAQUE)
							pBMP->SetPixel(SX, SY, BkColor);
						break;
					}
					break;
				default:
					pBMP->SetPixel(SX, SY, m_color);
					break;
			}
			if (SX==EX) break;
			if ((f+=Ha)>=W)
				f-=W,SY+=dY;
			SX++;
		}
	}

	return 0;
}


