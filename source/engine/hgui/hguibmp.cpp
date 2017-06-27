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

C_HGUIBMP::C_HGUIBMP(void):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_BMP)
{
	m_Width  = 0;
	m_Height = 0;
	m_BPP    = 0;
}

C_HGUIBMP::C_HGUIBMP(S_WORD w, S_WORD h, S_WORD bpp):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_BMP)
{
	m_Width  = w;
	m_Height = h;
	m_BPP    = bpp;
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

void *C_HGUIBMP::SetBmpData(void *pBuffer)
{
	if (pBuffer == NULL)
		return NULL;

	void *pOld = NULL;
	m_HGuiBuffer.Release();
	if (m_HGuiBuffer.GetReferenceCount() == 0)
	{// 系统管理对象
		delete m_HGuiBuffer.m_pBuffer;
		m_HGuiBuffer.m_pBuffer = NULL;
	}
	pOld = m_HGuiBuffer.m_pBuffer;

	m_HGuiBuffer = C_HGUIBUFFER(); // 置为用户管理对象
	m_HGuiBuffer.Retain(); // nothing
	m_HGuiBuffer.m_pBuffer = pBuffer;
	return pOld;
}

S_WORD C_HGUIBMP::GetWidth(void)
{
	return m_Width;
}

S_WORD C_HGUIBMP::GetHeight(void)
{
	return m_Height;
}


void *C_HGUIBMP::GetBmpData(void)
{
	return m_HGuiBuffer.m_pBuffer;
}

bool C_HGUIBMP::DeleteObject(void)
{
	bool fOk = true;
	m_HGuiBuffer.Release();
	if (m_HGuiBuffer.GetReferenceCount() == 0)
	{// 系统管理对象
		delete m_HGuiBuffer.m_pBuffer;
		m_HGuiBuffer.m_pBuffer = NULL;
		fOk = true;
	}
	else if (m_HGuiBuffer.GetReferenceCount() > 0)
	{// 系统管理对象
		fOk = true;
	}
	else
	{// 用户管理对象
		fOk = false;
	}
	return fOk;
}

int C_HGUIBMP::SetPixel(S_WORD x, S_WORD y, S_DWORD color)
{
	return -1;
}

S_DWORD C_HGUIBMP::GetPixel(S_WORD x, S_WORD y)
{
	return 0;
}

int C_HGUIBMP::FlipPixel(S_WORD x, S_WORD y)
{
	return -1;
}

int C_HGUIBMP::FlipHLine(S_WORD x, S_WORD y, S_WORD w)
{
	return 0;
}

int C_HGUIBMP::FillHLine(S_WORD x, S_WORD y, S_WORD w, S_DWORD color)
{
	return 0;
}

bool C_HGUIBMP::LoadBitmap(const string &strPath)
{
	return true;
}

void *C_HGUIBMP::SetBmpDataWithAutorelease(void *pBuffer)
{
	void *pOld = SetBmpData(pBuffer);
	m_HGuiBuffer.AutoRelease(); // 缓冲对象由系统自己管理
	m_HGuiBuffer.Retain();
	return pOld;
}

int C_HGUIBMP::SetPixel_1(S_WORD x, S_WORD y, S_DWORD color)
{
	S_BYTE *pBuffer = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	S_DWORD LcdLine = HGui_LcdLine();
	if (x < m_Width && y < m_Height)
	{
		char *p = (char *)pBuffer + y * LcdLine + (x >> 3);
		x = x & 0x07;//byte offset
		if (color&0x01)
			*p |=  (1 << x);
		else
			*p &= ~(1 << x);

		return 0;
	}
	return -1;
}

S_DWORD C_HGUIBMP::GetPixel_1(S_WORD x, S_WORD y)
{
	S_BYTE *pBuffer = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	S_DWORD LcdLine = HGui_LcdLine();
	if (x < m_Width && y < m_Height)
	{
		char *p = (char *)pBuffer + y * LcdLine + (x >> 3);
		x = x & 0x07;//byte offset
		return (S_DWORD)((*p>>x)&0x01);
	}
	return 0;
}

int C_HGUIBMP::FlipPixel_1(S_WORD x, S_WORD y)
{
	S_BYTE *pBuffer = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	S_DWORD LcdLine = HGui_LcdLine();
	if (x < m_Width && y < m_Height)
	{
		S_BYTE *p = (S_BYTE*)pBuffer + y * LcdLine + x / 8;
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

int C_HGUIBMP::FlipHLine_1(S_WORD x, S_WORD y, S_WORD w)
{
	if (x + w > m_Width || y > m_Height)
		return -1;

	for (int i = x; i < x + w; ++i)
		FlipPixel(i, y);
	return 0;
}

int C_HGUIBMP::FillHLine_1(S_WORD x, S_WORD y, S_WORD w, S_DWORD color)
{
	if (x + w > m_Width || y > m_Height)
		return -1;

	S_BYTE loff, roff, cnt, C = 0xFF;
	S_WORD X;
	S_BYTE *pBuffer = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	S_BYTE *p = (S_BYTE*)pBuffer + y * HGui_LcdLine() + (x >> 3);

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

//
// Screen Object
//
int C_HGUISCRN::InitScreenBitmap(void)
{
	HGUI_LcdScrn.SetBmpData(HGui_LcdBuffer());
	HGUI_LcdScrn.SetBmpInfo(HGui_LcdWidth(), HGui_LcdHeight(), HGui_LcdBpp());
	HGUI_LcdScrn.BufferNotRelease(); // 防止被系统删除
	return 0;
}

C_HGUISCRN::C_HGUISCRN(void):C_HGUIBMP(0, 0, 0)
{
}

C_HGUISCRN::~C_HGUISCRN()
{
}

bool C_HGUISCRN::BufferNotRelease(void)
{
	m_HGuiBuffer.BufferNotRelease(); // 防止被系统删除
	return true;
}

int C_HGUISCRN::SetPixel(S_WORD x, S_WORD y, S_DWORD color)
{
	return -1;
}

S_DWORD C_HGUISCRN::GetPixel(S_WORD x, S_WORD y)
{
	return 0;
}

int C_HGUISCRN::FlipPixel(S_WORD x, S_WORD y)
{
	return -1;
}

int C_HGUISCRN::FlipHLine(S_WORD x, S_WORD y, S_WORD w)
{
	return 0;
}

int C_HGUISCRN::FillHLine(S_WORD x, S_WORD y, S_WORD w, S_DWORD color)
{
	return 0;
}


C_HGUISCRN HGUI_LcdScrn;


