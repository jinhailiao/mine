//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguibmp.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "hguibmp.h"
#include "hguibase.h"

C_HGUIBMP::C_HGUIBMP(S_WORD w, S_WORD h, S_WORD bpp, void *pBuff):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_BMP)
{
	m_Width  = w;
	m_Height = h;
	m_BPP    = bpp;
	m_pBuff  = pBuff;
}

C_HGUIBMP::~C_HGUIBMP()
{
}

void C_HGUIBMP::SetBmpInfo(S_WORD w, S_WORD h, S_WORD bpp)
{
	m_Width  = w;
	m_Height = h;
	m_BPP    = bpp;
}

void C_HGUIBMP::SetBmpData(void *pBuff)
{
	m_pBuff = pBuff;
}

void *C_HGUIBMP::GetBmpData(void)
{
	return m_pBuff;
}


C_HGUISCRN::C_HGUISCRN(void):C_HGUIBMP(160, 160, 1, NULL)
{
}

C_HGUISCRN::~C_HGUISCRN()
{
}

int C_HGUISCRN::SetPixel(S_WORD x, S_WORD y, S_DWORD color)
{
	S_DWORD LcdLine = HGui_LcdLine();
	if (x < m_Width && y < m_Height)
	{
		char *p = (char *)m_pBuff + y * LcdLine + (x >> 3);
		x = x & 0x07;//byte offset
		if (color&0x01)
			*p |=  (1 << x);
		else
			*p &= ~(1 << x);

		return 0;
	}
	return -1;
}

S_DWORD C_HGUISCRN::GetPixel(S_WORD x, S_WORD y)
{
	S_DWORD LcdLine = HGui_LcdLine();
	if (x < m_Width && y < m_Height)
	{
		char *p = (char *)m_pBuff + y * LcdLine + (x >> 3);
		x = x & 0x07;//byte offset
		return (S_DWORD)((*p>>x)&0x01);
	}
	return 0;
}

int C_HGUISCRN::FlipPixel(S_WORD x, S_WORD y)
{
	S_DWORD LcdLine = HGui_LcdLine();
	if (x < m_Width && y < m_Height)
	{
		S_BYTE *p = (S_BYTE*)m_pBuff + y * LcdLine + x / 8;
		x = x & 0x07;//byte offset
		S_BYTE color = (1 << x) & *p;
		if (color)
			*p &= ~color;
		else
			*p |= 1 << x;

		return 0;
	}
	return -1;
}

int C_HGUISCRN::FlipHLine(S_WORD x, S_WORD y, S_WORD w)
{
	if (x + w > m_Width || y > m_Height)
		return -1;

	for (int i = x; i < x + w; ++i)
		FlipPixel(i, y);
	return 0;
}

int C_HGUISCRN::FillHLine(S_WORD x, S_WORD y, S_WORD w, S_DWORD color)
{
	if (x + w > m_Width || y > m_Height)
		return -1;

	S_BYTE loff, roff, cnt, C = 0xFF;
	S_WORD X;
	S_BYTE *p = (S_BYTE*)m_pBuff + y * HGui_LcdLine() + (x >> 3);

	if ((loff = (x&0x07)) != 0)
	{
		cnt = 8 - loff;
		if (cnt > w) cnt = (S_BYTE)w;
		roff = 8 - loff - cnt;
		C >>= loff; C <<= loff; 
		C <<= roff; C >>= roff;
		
		if (color&0x01) *p++ |=  C;
		else *p++ &= ~C;
		x += cnt, w -= cnt;
	}
	
	C = (color&0x01)? 0xFF:0x00;
	for (X = x; X + 8 <= x + w; X += 8)
		*p++  = C;

	C = 0xFF;
	if (X < x + w)
	{
		cnt = (x + w) - X;
		roff = 8 - cnt;
		C <<= roff; C >>= roff;
		if (color&0x01) *p |=  C;
		else *p &= ~C;
	}

	return 0;
}


C_HGUISCRN HGUI_LcdScrn;


