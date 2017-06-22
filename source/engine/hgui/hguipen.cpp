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


