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
#include "hguipic.h"
#include "haifile.h"

C_HGUIBMP::C_HGUIBMP(void):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_BMP)
{
	m_Width  = 0;
	m_Height = 0;
	m_BPP    = 0;
	m_WidthBytes = 0;
}

C_HGUIBMP::C_HGUIBMP(S_WORD w, S_WORD h, S_WORD bpp):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_BMP)
{
	m_Width  = w;
	m_Height = h;
	m_BPP    = bpp;
	m_WidthBytes = BITMAP_WIDTHBYTES(w, bpp);
}

C_HGUIBMP::~C_HGUIBMP()
{
	DeleteObject();
}

void C_HGUIBMP::SetBmpInfo(S_WORD w, S_WORD h, S_WORD bpp)
{
	m_Width  = w;
	m_Height = h;
	m_BPP    = bpp;
	m_WidthBytes = BITMAP_WIDTHBYTES(w, bpp);
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
	if (x >= m_Width || y >= m_Height)
		return -1;

	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	switch (m_BPP)
	{
	case 8:
		color &= 0xFF;
		pImage += y * m_WidthBytes+ x;
		*pImage = (S_BYTE)color;
		break;
	case 16:
		color &= 0xFFFF;
		pImage += y * m_WidthBytes+ (x<<1);
		HAI_WRITEWORD(pImage, (S_WORD)color);
		break;
	case 24:
		color &= 0xFFFFFF;
		pImage += y * m_WidthBytes+ x*3;
		HAI_WRITEDWORD3(pImage, color);
		break;
	default:
		break;
	}
	return 0;
}

S_DWORD C_HGUIBMP::GetPixel(S_WORD x, S_WORD y)
{
	if (x >= m_Width || y >= m_Height)
		return 0;

	S_DWORD color = 0;
	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	switch (m_BPP)
	{
	case 8:
		color = *(pImage + y * m_WidthBytes + x);
		break;
	case 16:
		pImage += y * m_WidthBytes+ (x<<1);
		color = HAI_MAKEWORD(pImage);
		break;
	case 24:
		pImage += y * m_WidthBytes+ x*3;
		color = HAI_MAKEDWORD3(pImage);
		break;
	default:
		break;
	}
	
	return color;
}

int C_HGUIBMP::FlipPixel(S_WORD x, S_WORD y)
{
	if (x >= m_Width || y >= m_Height)
		return -1;

	S_DWORD color = 0;
	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	switch (m_BPP)
	{
	case 8:
		pImage += y * m_WidthBytes + x;
		color = *pImage;
		*pImage = (S_BYTE)~color;
		break;
	case 16:
		pImage += y * m_WidthBytes+ (x<<1);
		color = HAI_MAKEWORD(pImage);
		HAI_WRITEWORD(pImage, (S_WORD)~color);
		break;
	case 24:
		pImage += y * m_WidthBytes+ x*3;
		color = HAI_MAKEDWORD3(pImage);
		HAI_WRITEDWORD3(pImage, ~color);
		break;
	default:
		break;
	}
	return 0;
}

int C_HGUIBMP::FlipHLine(S_WORD x, S_WORD y, S_WORD w)
{
	if (x + w > m_Width || y > m_Height)
		return -1;

	S_DWORD color = 0;
	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	switch (m_BPP)
	{
	case 8:
		pImage += y * m_WidthBytes + x;
		for (int i = x; i < x + w; ++i,++pImage)
		{
			color = *pImage;
			*pImage = (S_BYTE)~color;
		}
		break;
	case 16:
		pImage += y * m_WidthBytes+ (x<<1);
		for (int i = x; i < x + w; ++i,pImage+=2)
		{
			color = HAI_MAKEWORD(pImage);
			HAI_WRITEWORD(pImage, (S_WORD)~color);
		}
		break;
	case 24:
		pImage += y * m_WidthBytes+ x*3;
		for (int i = x; i < x + w; ++i,pImage+=3)
		{
			color = HAI_MAKEDWORD3(pImage);
			HAI_WRITEDWORD3(pImage, ~color);
		}
		break;
	default:
		break;
	}
	return 0;
}

int C_HGUIBMP::FillHLine(S_WORD x, S_WORD y, S_WORD w, S_DWORD color)
{
	if (x + w > m_Width || y > m_Height)
		return -1;

	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	switch (m_BPP)
	{
	case 8:
		pImage += y * m_WidthBytes + x;
		for (int i = x; i < x + w; ++i,++pImage)
			*pImage = (S_BYTE)color;
		break;
	case 16:
		pImage += y * m_WidthBytes+ (x<<1);
		for (int i = x; i < x + w; ++i,pImage+=2)
			HAI_WRITEWORD(pImage, (S_WORD)color);
		break;
	case 24:
		pImage += y * m_WidthBytes+ x*3;
		for (int i = x; i < x + w; ++i,pImage+=3)
			HAI_WRITEDWORD3(pImage, color);
		break;
	default:
		break;
	}
	return 0;
}

bool C_HGUIBMP::LoadBitmap(const string &strPath)
{
	C_FILE filePic(strPath.c_str(), "rb");
	if (!filePic)
		return false;

	filePic.FileSeek(0, SEEK_END);
	S_DWORD size = (S_DWORD)filePic.FileTell();
	// ???判断size的大小

	string strData;
	strData.append((size_t)(size+4), (char)0);
	
	filePic.FileSeek(0, SEEK_SET);
	if (filePic.FileRead(&strData[0], (size_t)size) != size)
		return false;

	C_HGUIPIC picDecode;
	S_BmpInfo BmpInfo;
	S_IconInfo IconInfo;
	void *pImage = NULL;
	const S_BYTE *pRes = (S_BYTE *)strData.data();
	if (*pRes=='B' && *(pRes+1)=='M')
	{
		pImage = picDecode.BmpDecode(pRes, BmpInfo);
		m_Width = BmpInfo.m_width, m_Height = BmpInfo.m_height;
		m_BPP = BmpInfo.m_BitsPixel, m_WidthBytes = BmpInfo.m_WidthBytes;
	}
	else if (*pRes==0x0A)
	{
		pImage = picDecode.PcxDecode(pRes, BmpInfo);
		m_Width = BmpInfo.m_width, m_Height = BmpInfo.m_height;
		m_BPP = BmpInfo.m_BitsPixel, m_WidthBytes = BmpInfo.m_WidthBytes;
	}
	else if (HAI_MAKEWORD(pRes)==0 && (HAI_MAKEWORD(pRes+2)==1||HAI_MAKEWORD(pRes+2)==2))
	{
		pImage = picDecode.IconDecode(pRes, IconInfo, 1);
		m_Width = IconInfo.m_width, m_Height = IconInfo.m_height;
		m_BPP = IconInfo.m_BitsPixel, m_WidthBytes = IconInfo.m_WidthBytes;
		delete IconInfo.m_lpMask;
	}

	if (pImage == NULL)
		return false;

	SetBmpDataWithAutorelease(pImage);
	return true; 
}

bool C_HGUIBMP::StretchBlit(const S_RECT &dRect, const C_HGUIBMP *pBMP, const S_RECT &sRect)
{
	if (GetBitsPixel() != pBMP->GetBitsPixel())
		return false;

	S_WORD dx = dRect.x, dy = dRect.y;
	S_WORD dw = dRect.w, dh = dRect.h;
	S_WORD sx = sRect.x, sy = sRect.y;
	S_WORD sw = sRect.w, sh = sRect.h;

	S_BYTE *pDstImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer + dy*m_WidthBytes + dx*m_BPP/8;
	const S_BYTE *pSrcImage = (S_BYTE *)pBMP->m_HGuiBuffer.m_pBuffer + sy*pBMP->m_WidthBytes + sx*pBMP->m_BPP/8;

	S_WORD HeightCnt = 0;
	for (S_WORD i = 0; i < sh; i++)
	{
		for (HeightCnt += dh; HeightCnt >= sh; HeightCnt -= sh)
		{
			StretchBlitLine(pDstImage, dw, pSrcImage, sw);
			pDstImage += m_WidthBytes;
		}
		pSrcImage += pBMP->m_WidthBytes;
	}

	return true;
}

void *C_HGUIBMP::SetBmpDataWithAutorelease(void *pBuffer)
{
	if (pBuffer == NULL)
		return NULL;

	void *pOld = SetBmpData(pBuffer);
	m_HGuiBuffer.AutoRelease(); // 缓冲对象由系统自己管理
	m_HGuiBuffer.Retain();
	return pOld;
}

bool C_HGUIBMP::StretchBlitLine(S_BYTE *pDst, S_WORD dw, const S_BYTE *pSrc, S_WORD sw)
{
	S_WORD flag = 0xFFFF;

	S_WORD WidthCnt = 0;
	switch (m_BPP)
	{
	case 8:
		for (S_WORD i = 0; i < sw; i++)
		{
			for (WidthCnt += dw; WidthCnt >= sw; WidthCnt -= sw)
			{
				*pDst++ = *pSrc;
			}
			pSrc++;
		}
		break;
	case 16:
		for (S_WORD i = 0; i < sw; i++)
		{
			for (WidthCnt += dw; WidthCnt >= sw; WidthCnt -= sw)
			{
				*(pDst+0) = *(pSrc+0);
				*(pDst+1) = *(pSrc+1);
				pDst += 2;
			}
			pSrc += 2;
		}
		break;

	case 24:
		for (S_WORD i = 0; i < sw; i++)
		{
			for (WidthCnt += dw; WidthCnt >= sw; WidthCnt -= sw)
			{
				*(pDst+0) = *(pSrc+0);
				*(pDst+1) = *(pSrc+1);
				*(pDst+2) = *(pSrc+2);
				pDst += 3;
			}
			pSrc += 3;
		}
		break;
	default:
		//todo;
		break;
	}
	return true;
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


