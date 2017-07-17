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
#include "hguidef.h"

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

bool C_HGUIBMP::BitBltNotSrcCopy(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIBMP *pBMP, S_WORD sx, S_WORD sy)
{
	S_WORD wbak = w;
	S_BYTE *pDst = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	const S_BYTE *pSrc = (S_BYTE *)pBMP->m_HGuiBuffer.m_pBuffer;

	switch (m_BPP)
	{
	case 8:
		pDst += dy * m_WidthBytes + dx;
		pSrc += sy * pBMP->m_WidthBytes + sx;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pDst++ = ~*pSrc++;
			case 7:   *pDst++ = ~*pSrc++;
			case 6:   *pDst++ = ~*pSrc++;
			case 5:   *pDst++ = ~*pSrc++;
			case 4:   *pDst++ = ~*pSrc++;
			case 3:   *pDst++ = ~*pSrc++;
			case 2:   *pDst++ = ~*pSrc++;
			case 1:   *pDst++ = ~*pSrc++;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak;
			pSrc += pBMP->m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pDst += dy * m_WidthBytes + (dx<<1);
		pSrc += sy * pBMP->m_WidthBytes + (sx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pDst = ~*(S_WORD*)pSrc; pDst += 2;pSrc += 2;
			case 7:   *(S_WORD*)pDst = ~*(S_WORD*)pSrc; pDst += 2;pSrc += 2;
			case 6:   *(S_WORD*)pDst = ~*(S_WORD*)pSrc; pDst += 2;pSrc += 2;
			case 5:   *(S_WORD*)pDst = ~*(S_WORD*)pSrc; pDst += 2;pSrc += 2;
			case 4:   *(S_WORD*)pDst = ~*(S_WORD*)pSrc; pDst += 2;pSrc += 2;
			case 3:   *(S_WORD*)pDst = ~*(S_WORD*)pSrc; pDst += 2;pSrc += 2;
			case 2:   *(S_WORD*)pDst = ~*(S_WORD*)pSrc; pDst += 2;pSrc += 2;
			case 1:   *(S_WORD*)pDst = ~*(S_WORD*)pSrc; pDst += 2;pSrc += 2;
					}while (--w);
			}
			pDst += m_WidthBytes - (wbak<<1);
			pSrc += pBMP->m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pDst += dy * m_WidthBytes + dx*3;
		pSrc += sy * pBMP->m_WidthBytes + sx*3;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++;
			case 7:   *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++;
			case 6:   *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++;
			case 5:   *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++;
			case 4:   *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++;
			case 3:   *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++;
			case 2:   *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++;
			case 1:   *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++; *pDst++ = ~*pSrc++;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak*3;
			pSrc += pBMP->m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

bool C_HGUIBMP::BitBltSrcAnd(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIBMP *pBMP, S_WORD sx, S_WORD sy)
{
	S_WORD wbak = w;
	S_BYTE *pDst = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	const S_BYTE *pSrc = (S_BYTE *)pBMP->m_HGuiBuffer.m_pBuffer;

	switch (m_BPP)
	{
	case 8:
		pDst += dy * m_WidthBytes + dx;
		pSrc += sy * pBMP->m_WidthBytes + sx;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pDst++ &= *pSrc++;
			case 7:   *pDst++ &= *pSrc++;
			case 6:   *pDst++ &= *pSrc++;
			case 5:   *pDst++ &= *pSrc++;
			case 4:   *pDst++ &= *pSrc++;
			case 3:   *pDst++ &= *pSrc++;
			case 2:   *pDst++ &= *pSrc++;
			case 1:   *pDst++ &= *pSrc++;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak;
			pSrc += pBMP->m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pDst += dy * m_WidthBytes + (dx<<1);
		pSrc += sy * pBMP->m_WidthBytes + (sx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pDst &= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 7:   *(S_WORD*)pDst &= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 6:   *(S_WORD*)pDst &= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 5:   *(S_WORD*)pDst &= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 4:   *(S_WORD*)pDst &= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 3:   *(S_WORD*)pDst &= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 2:   *(S_WORD*)pDst &= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 1:   *(S_WORD*)pDst &= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
					}while (--w);
			}
			pDst += m_WidthBytes - (wbak<<1);
			pSrc += pBMP->m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pDst += dy * m_WidthBytes + dx*3;
		pSrc += sy * pBMP->m_WidthBytes + sx*3;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pDst++ &= *pSrc++; *pDst++ &= *pSrc++; *pDst++ &= *pSrc++;
			case 7:   *pDst++ &= *pSrc++; *pDst++ &= *pSrc++; *pDst++ &= *pSrc++;
			case 6:   *pDst++ &= *pSrc++; *pDst++ &= *pSrc++; *pDst++ &= *pSrc++;
			case 5:   *pDst++ &= *pSrc++; *pDst++ &= *pSrc++; *pDst++ &= *pSrc++;
			case 4:   *pDst++ &= *pSrc++; *pDst++ &= *pSrc++; *pDst++ &= *pSrc++;
			case 3:   *pDst++ &= *pSrc++; *pDst++ &= *pSrc++; *pDst++ &= *pSrc++;
			case 2:   *pDst++ &= *pSrc++; *pDst++ &= *pSrc++; *pDst++ &= *pSrc++;
			case 1:   *pDst++ &= *pSrc++; *pDst++ &= *pSrc++; *pDst++ &= *pSrc++;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak*3;
			pSrc += pBMP->m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

bool C_HGUIBMP::BitBltSrcCopy(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIBMP *pBMP, S_WORD sx, S_WORD sy)
{
	S_WORD wbak = w;
	S_BYTE *pDst = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	const S_BYTE *pSrc = (S_BYTE *)pBMP->m_HGuiBuffer.m_pBuffer;
	
	switch (m_BPP)
	{
	case 8:
		pDst += dy * m_WidthBytes + dx;
		pSrc += sy * pBMP->m_WidthBytes + sx;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pDst++ = *pSrc++;
			case 7:   *pDst++ = *pSrc++;
			case 6:   *pDst++ = *pSrc++;
			case 5:   *pDst++ = *pSrc++;
			case 4:   *pDst++ = *pSrc++;
			case 3:   *pDst++ = *pSrc++;
			case 2:   *pDst++ = *pSrc++;
			case 1:   *pDst++ = *pSrc++;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak;
			pSrc += pBMP->m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pDst += dy * m_WidthBytes + (dx<<1);
		pSrc += sy * pBMP->m_WidthBytes + (sx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 7:   *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 6:   *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 5:   *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 4:   *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 3:   *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 2:   *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 1:   *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
					}while (--w);
			}
			pDst += m_WidthBytes - (wbak<<1);
			pSrc += pBMP->m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pDst += dy * m_WidthBytes + dx*3;
		pSrc += sy * pBMP->m_WidthBytes + sx*3;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pDst++ = *pSrc++; *pDst++ = *pSrc++; *pDst++ = *pSrc++;
			case 7:   *pDst++ = *pSrc++; *pDst++ = *pSrc++; *pDst++ = *pSrc++;
			case 6:   *pDst++ = *pSrc++; *pDst++ = *pSrc++; *pDst++ = *pSrc++;
			case 5:   *pDst++ = *pSrc++; *pDst++ = *pSrc++; *pDst++ = *pSrc++;
			case 4:   *pDst++ = *pSrc++; *pDst++ = *pSrc++; *pDst++ = *pSrc++;
			case 3:   *pDst++ = *pSrc++; *pDst++ = *pSrc++; *pDst++ = *pSrc++;
			case 2:   *pDst++ = *pSrc++; *pDst++ = *pSrc++; *pDst++ = *pSrc++;
			case 1:   *pDst++ = *pSrc++; *pDst++ = *pSrc++; *pDst++ = *pSrc++;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak*3;
			pSrc += pBMP->m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

bool C_HGUIBMP::BitBltSrcInvert(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIBMP *pBMP, S_WORD sx, S_WORD sy)
{
	S_WORD wbak = w;
	S_BYTE *pDst = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	const S_BYTE *pSrc = (S_BYTE *)pBMP->m_HGuiBuffer.m_pBuffer;
	
	switch (m_BPP)
	{
	case 8:
		pDst += dy * m_WidthBytes + dx;
		pSrc += sy * pBMP->m_WidthBytes + sx;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pDst++ ^= *pSrc++;
			case 7:   *pDst++ ^= *pSrc++;
			case 6:   *pDst++ ^= *pSrc++;
			case 5:   *pDst++ ^= *pSrc++;
			case 4:   *pDst++ ^= *pSrc++;
			case 3:   *pDst++ ^= *pSrc++;
			case 2:   *pDst++ ^= *pSrc++;
			case 1:   *pDst++ ^= *pSrc++;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak;
			pSrc += pBMP->m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pDst += dy * m_WidthBytes + (dx<<1);
		pSrc += sy * pBMP->m_WidthBytes + (sx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pDst ^= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 7:   *(S_WORD*)pDst ^= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 6:   *(S_WORD*)pDst ^= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 5:   *(S_WORD*)pDst ^= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 4:   *(S_WORD*)pDst ^= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 3:   *(S_WORD*)pDst ^= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 2:   *(S_WORD*)pDst ^= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 1:   *(S_WORD*)pDst ^= *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
					}while (--w);
			}
			pDst += m_WidthBytes - (wbak<<1);
			pSrc += pBMP->m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pDst += dy * m_WidthBytes + dx*3;
		pSrc += sy * pBMP->m_WidthBytes + sx*3;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++;
			case 7:   *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++;
			case 6:   *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++;
			case 5:   *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++;
			case 4:   *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++;
			case 3:   *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++;
			case 2:   *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++;
			case 1:   *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++; *pDst++ ^= *pSrc++;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak*3;
			pSrc += pBMP->m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

bool C_HGUIBMP::BitBltBlackness(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h)
{
	S_BYTE c8;
	S_WORD wbak = w;
	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	
	switch (m_BPP)
	{
	case 8:
		pImage += dy * m_WidthBytes + dx;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = HGUI_COLOR_BLACK_8;
			case 7:   *pImage++ = HGUI_COLOR_BLACK_8;
			case 6:   *pImage++ = HGUI_COLOR_BLACK_8;
			case 5:   *pImage++ = HGUI_COLOR_BLACK_8;
			case 4:   *pImage++ = HGUI_COLOR_BLACK_8;
			case 3:   *pImage++ = HGUI_COLOR_BLACK_8;
			case 2:   *pImage++ = HGUI_COLOR_BLACK_8;
			case 1:   *pImage++ = HGUI_COLOR_BLACK_8;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pImage += dy * m_WidthBytes + (dx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pImage = HGUI_COLOR_BLACK_16; pImage += 2;
			case 7:   *(S_WORD*)pImage = HGUI_COLOR_BLACK_16; pImage += 2;
			case 6:   *(S_WORD*)pImage = HGUI_COLOR_BLACK_16; pImage += 2;
			case 5:   *(S_WORD*)pImage = HGUI_COLOR_BLACK_16; pImage += 2;
			case 4:   *(S_WORD*)pImage = HGUI_COLOR_BLACK_16; pImage += 2;
			case 3:   *(S_WORD*)pImage = HGUI_COLOR_BLACK_16; pImage += 2;
			case 2:   *(S_WORD*)pImage = HGUI_COLOR_BLACK_16; pImage += 2;
			case 1:   *(S_WORD*)pImage = HGUI_COLOR_BLACK_16; pImage += 2;
					}while (--w);
			}
			pImage += m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pImage += dy * m_WidthBytes + dx*3;
		c8 = (S_BYTE)HGUI_COLOR_BLACK_24;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 7:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 6:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 5:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 4:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 3:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 2:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 1:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

bool C_HGUIBMP::BitBltWhiteness(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h)
{
	S_BYTE c8;
	S_WORD wbak = w;
	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	
	switch (m_BPP)
	{
	case 8:
		pImage += dy * m_WidthBytes + dx;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = HGUI_COLOR_WHITE_8;
			case 7:   *pImage++ = HGUI_COLOR_WHITE_8;
			case 6:   *pImage++ = HGUI_COLOR_WHITE_8;
			case 5:   *pImage++ = HGUI_COLOR_WHITE_8;
			case 4:   *pImage++ = HGUI_COLOR_WHITE_8;
			case 3:   *pImage++ = HGUI_COLOR_WHITE_8;
			case 2:   *pImage++ = HGUI_COLOR_WHITE_8;
			case 1:   *pImage++ = HGUI_COLOR_WHITE_8;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pImage += dy * m_WidthBytes + (dx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pImage = HGUI_COLOR_WHITE_16; pImage += 2;
			case 7:   *(S_WORD*)pImage = HGUI_COLOR_WHITE_16; pImage += 2;
			case 6:   *(S_WORD*)pImage = HGUI_COLOR_WHITE_16; pImage += 2;
			case 5:   *(S_WORD*)pImage = HGUI_COLOR_WHITE_16; pImage += 2;
			case 4:   *(S_WORD*)pImage = HGUI_COLOR_WHITE_16; pImage += 2;
			case 3:   *(S_WORD*)pImage = HGUI_COLOR_WHITE_16; pImage += 2;
			case 2:   *(S_WORD*)pImage = HGUI_COLOR_WHITE_16; pImage += 2;
			case 1:   *(S_WORD*)pImage = HGUI_COLOR_WHITE_16; pImage += 2;
					}while (--w);
			}
			pImage += m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pImage += dy * m_WidthBytes + dx*3;
		c8 = (S_BYTE)HGUI_COLOR_WHITE_24;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 7:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 6:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 5:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 4:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 3:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 2:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
			case 1:   *(pImage+0) = c8; *(pImage+1) = c8; *(pImage+2) = c8;pImage += 3;
					}while (--w);
			}
			pImage += m_WidthBytes - (wbak*3);
		}
		return true;
	default:
		break;
	}
	return false;
}

bool C_HGUIBMP::BitBltDstinvert(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h)
{
	S_WORD wbak = w;
	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	
	switch (m_BPP)
	{
	case 8:
		pImage += dy * m_WidthBytes + dx;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = ~*pImage;
			case 7:   *pImage++ = ~*pImage;
			case 6:   *pImage++ = ~*pImage;
			case 5:   *pImage++ = ~*pImage;
			case 4:   *pImage++ = ~*pImage;
			case 3:   *pImage++ = ~*pImage;
			case 2:   *pImage++ = ~*pImage;
			case 1:   *pImage++ = ~*pImage;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pImage += dy * m_WidthBytes + (dx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pImage = ~*(S_WORD*)pImage; pImage += 2;
			case 7:   *(S_WORD*)pImage = ~*(S_WORD*)pImage; pImage += 2;
			case 6:   *(S_WORD*)pImage = ~*(S_WORD*)pImage; pImage += 2;
			case 5:   *(S_WORD*)pImage = ~*(S_WORD*)pImage; pImage += 2;
			case 4:   *(S_WORD*)pImage = ~*(S_WORD*)pImage; pImage += 2;
			case 3:   *(S_WORD*)pImage = ~*(S_WORD*)pImage; pImage += 2;
			case 2:   *(S_WORD*)pImage = ~*(S_WORD*)pImage; pImage += 2;
			case 1:   *(S_WORD*)pImage = ~*(S_WORD*)pImage; pImage += 2;
					}while (--w);
			}
			pImage += m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pImage += dy * m_WidthBytes + dx*3;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = ~*pImage; *pImage++ = ~*pImage; *pImage++ = ~*pImage; 
			case 7:   *pImage++ = ~*pImage; *pImage++ = ~*pImage; *pImage++ = ~*pImage; 
			case 6:   *pImage++ = ~*pImage; *pImage++ = ~*pImage; *pImage++ = ~*pImage; 
			case 5:   *pImage++ = ~*pImage; *pImage++ = ~*pImage; *pImage++ = ~*pImage; 
			case 4:   *pImage++ = ~*pImage; *pImage++ = ~*pImage; *pImage++ = ~*pImage; 
			case 3:   *pImage++ = ~*pImage; *pImage++ = ~*pImage; *pImage++ = ~*pImage; 
			case 2:   *pImage++ = ~*pImage; *pImage++ = ~*pImage; *pImage++ = ~*pImage; 
			case 1:   *pImage++ = ~*pImage; *pImage++ = ~*pImage; *pImage++ = ~*pImage; 
					}while (--w);
			}
			pImage += m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

bool C_HGUIBMP::BitBltPatinvert(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, S_DWORD color)
{
	S_BYTE c8, r, g, b;
	S_WORD c16;
	S_WORD wbak = w;
	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	
	switch (m_BPP)
	{
	case 8:
		pImage += dy * m_WidthBytes + dx;
		c8 = (S_BYTE)color;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = *pImage ^ c8;
			case 7:   *pImage++ = *pImage ^ c8;
			case 6:   *pImage++ = *pImage ^ c8;
			case 5:   *pImage++ = *pImage ^ c8;
			case 4:   *pImage++ = *pImage ^ c8;
			case 3:   *pImage++ = *pImage ^ c8;
			case 2:   *pImage++ = *pImage ^ c8;
			case 1:   *pImage++ = *pImage ^ c8;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pImage += dy * m_WidthBytes + (dx<<1);
		c16 = (S_WORD)color;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pImage = *(S_WORD*)pImage ^ c16; pImage += 2;
			case 7:   *(S_WORD*)pImage = *(S_WORD*)pImage ^ c16; pImage += 2;
			case 6:   *(S_WORD*)pImage = *(S_WORD*)pImage ^ c16; pImage += 2;
			case 5:   *(S_WORD*)pImage = *(S_WORD*)pImage ^ c16; pImage += 2;
			case 4:   *(S_WORD*)pImage = *(S_WORD*)pImage ^ c16; pImage += 2;
			case 3:   *(S_WORD*)pImage = *(S_WORD*)pImage ^ c16; pImage += 2;
			case 2:   *(S_WORD*)pImage = *(S_WORD*)pImage ^ c16; pImage += 2;
			case 1:   *(S_WORD*)pImage = *(S_WORD*)pImage ^ c16; pImage += 2;
					}while (--w);
			}
			pImage += m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pImage += dy * m_WidthBytes + dx*3;
		r = (S_BYTE)color; g = (S_BYTE)(color>>8); b = (S_BYTE)(color>>16);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = *pImage ^ r; *pImage++ = *pImage ^ g; *pImage++ = *pImage ^ b;
			case 7:   *pImage++ = *pImage ^ r; *pImage++ = *pImage ^ g; *pImage++ = *pImage ^ b;
			case 6:   *pImage++ = *pImage ^ r; *pImage++ = *pImage ^ g; *pImage++ = *pImage ^ b;
			case 5:   *pImage++ = *pImage ^ r; *pImage++ = *pImage ^ g; *pImage++ = *pImage ^ b;
			case 4:   *pImage++ = *pImage ^ r; *pImage++ = *pImage ^ g; *pImage++ = *pImage ^ b;
			case 3:   *pImage++ = *pImage ^ r; *pImage++ = *pImage ^ g; *pImage++ = *pImage ^ b;
			case 2:   *pImage++ = *pImage ^ r; *pImage++ = *pImage ^ g; *pImage++ = *pImage ^ b;
			case 1:   *pImage++ = *pImage ^ r; *pImage++ = *pImage ^ g; *pImage++ = *pImage ^ b;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

bool C_HGUIBMP::BitBltPatpaint(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, S_DWORD color)
{
	S_BYTE c8, r, g, b;
	S_WORD c16;
	S_WORD wbak = w;
	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	
	switch (m_BPP)
	{
	case 8:
		pImage += dy * m_WidthBytes + dx;
		c8 = (S_BYTE)color;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = *pImage | c8;
			case 7:   *pImage++ = *pImage | c8;
			case 6:   *pImage++ = *pImage | c8;
			case 5:   *pImage++ = *pImage | c8;
			case 4:   *pImage++ = *pImage | c8;
			case 3:   *pImage++ = *pImage | c8;
			case 2:   *pImage++ = *pImage | c8;
			case 1:   *pImage++ = *pImage | c8;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pImage += dy * m_WidthBytes + (dx<<1);
		c16 = (S_WORD)color;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pImage = *(S_WORD*)pImage | c16; pImage += 2;
			case 7:   *(S_WORD*)pImage = *(S_WORD*)pImage | c16; pImage += 2;
			case 6:   *(S_WORD*)pImage = *(S_WORD*)pImage | c16; pImage += 2;
			case 5:   *(S_WORD*)pImage = *(S_WORD*)pImage | c16; pImage += 2;
			case 4:   *(S_WORD*)pImage = *(S_WORD*)pImage | c16; pImage += 2;
			case 3:   *(S_WORD*)pImage = *(S_WORD*)pImage | c16; pImage += 2;
			case 2:   *(S_WORD*)pImage = *(S_WORD*)pImage | c16; pImage += 2;
			case 1:   *(S_WORD*)pImage = *(S_WORD*)pImage | c16; pImage += 2;
					}while (--w);
			}
			pImage += m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pImage += dy * m_WidthBytes + dx*3;
		r = (S_BYTE)color;	g = (S_BYTE)(color>>8); b = (S_BYTE)(color>>16);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = *pImage | r; *pImage++ = *pImage | g; *pImage++ = *pImage | b;
			case 7:   *pImage++ = *pImage | r; *pImage++ = *pImage | g; *pImage++ = *pImage | b;
			case 6:   *pImage++ = *pImage | r; *pImage++ = *pImage | g; *pImage++ = *pImage | b;
			case 5:   *pImage++ = *pImage | r; *pImage++ = *pImage | g; *pImage++ = *pImage | b;
			case 4:   *pImage++ = *pImage | r; *pImage++ = *pImage | g; *pImage++ = *pImage | b;
			case 3:   *pImage++ = *pImage | r; *pImage++ = *pImage | g; *pImage++ = *pImage | b;
			case 2:   *pImage++ = *pImage | r; *pImage++ = *pImage | g; *pImage++ = *pImage | b;
			case 1:   *pImage++ = *pImage | r; *pImage++ = *pImage | g; *pImage++ = *pImage | b;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

bool C_HGUIBMP::BitBltPatcopy(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, S_DWORD color)
{
	S_BYTE c8, r, g, b;
	S_WORD c16;
	S_WORD wbak = w;
	S_BYTE *pImage = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	
	switch (m_BPP)
	{
	case 8:
		pImage += dy * m_WidthBytes + dx;
		c8 = (S_BYTE)color;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = c8;
			case 7:   *pImage++ = c8;
			case 6:   *pImage++ = c8;
			case 5:   *pImage++ = c8;
			case 4:   *pImage++ = c8;
			case 3:   *pImage++ = c8;
			case 2:   *pImage++ = c8;
			case 1:   *pImage++ = c8;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pImage += dy * m_WidthBytes + (dx<<1);
		c16 = (S_WORD)color;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*(S_WORD*)pImage = c16; pImage += 2;
			case 7:   *(S_WORD*)pImage = c16; pImage += 2;
			case 6:   *(S_WORD*)pImage = c16; pImage += 2;
			case 5:   *(S_WORD*)pImage = c16; pImage += 2;
			case 4:   *(S_WORD*)pImage = c16; pImage += 2;
			case 3:   *(S_WORD*)pImage = c16; pImage += 2;
			case 2:   *(S_WORD*)pImage = c16; pImage += 2;
			case 1:   *(S_WORD*)pImage = c16; pImage += 2;
					}while (--w);
			}
			pImage += m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pImage += dy * m_WidthBytes + dx*3;
		r = (S_BYTE)color; g = (S_BYTE)(color>>8); b = (S_BYTE)(color>>16);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{*pImage++ = r; *pImage++ = g; *pImage++ = b;
			case 7:   *pImage++ = r; *pImage++ = g; *pImage++ = b;
			case 6:   *pImage++ = r; *pImage++ = g; *pImage++ = b;
			case 5:   *pImage++ = r; *pImage++ = g; *pImage++ = b;
			case 4:   *pImage++ = r; *pImage++ = g; *pImage++ = b;
			case 3:   *pImage++ = r; *pImage++ = g; *pImage++ = b;
			case 2:   *pImage++ = r; *pImage++ = g; *pImage++ = b;
			case 1:   *pImage++ = r; *pImage++ = g; *pImage++ = b;
					}while (--w);
			}
			pImage += m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

#define BRIGHT_BLEND_16_565(sc, dc, val)		\
	do{		\
		S_SHORT r = (S_SHORT)((sc) & 0x1F);		\
		S_SHORT g = (S_SHORT)((sc)>>5 & 0x3F);	\
		S_SHORT b = (S_SHORT)((sc)>>11 & 0x1F);	\
		r += (val), g += (val), b += (val);		\
		if (r>0x1F) r=0x1F;		\
		else if (r<0) r = 0;			\
		if (g>0x3F) g = 0x3F;		\
		else if (g<0) g = 0;		\
		if (b>0x1F) b = 0x1F;		\
		else if (b<0) b = 0;		\
		(dc) = (S_WORD)(r|g<<5|b<<11);		\
	}while (0)

#define BRIGHT_BLEND_24(ps, pd, val)		\
	do{		\
		S_SHORT r = (S_SHORT)(*((ps)+0));		\
		S_SHORT g = (S_SHORT)(*((ps)+1));		\
		S_SHORT b = (S_SHORT)(*((ps)+2));	\
		r += (val), g += (val), b += (val);		\
		if (r>0xFF) r=0xFF;		\
		else if (r<0) r = 0;			\
		if (g>0xFF) g = 0xFF;		\
		else if (g<0) g = 0;		\
		if (b>0xFF) b = 0xFF;		\
		else if (b<0) b = 0;		\
		*((pd)+0) = (S_BYTE)r;		\
		*((pd)+1) = (S_BYTE)g;		\
		*((pd)+2) = (S_BYTE)b;		\
	}while (0)

bool C_HGUIBMP::BitBltBrightCopy(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIBMP *pBMP, S_WORD sx, S_WORD sy, S_DWORD ColorKey)
{
	S_WORD wbak = w;
	S_WORD bright = (S_WORD)ColorKey;
	S_BYTE *pDst = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	const S_BYTE *pSrc = (S_BYTE *)pBMP->m_HGuiBuffer.m_pBuffer;

	switch (m_BPP)
	{
	case 16:
		bright >>= 3;
		pDst += dy * m_WidthBytes + (dx<<1);
		pSrc += sy * pBMP->m_WidthBytes + (sx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{BRIGHT_BLEND_16_565(*(S_WORD*)pSrc, *(S_WORD*)pDst, bright); pDst += 2; pSrc += 2;
			case 7:   BRIGHT_BLEND_16_565(*(S_WORD*)pSrc, *(S_WORD*)pDst, bright); pDst += 2; pSrc += 2;
			case 6:   BRIGHT_BLEND_16_565(*(S_WORD*)pSrc, *(S_WORD*)pDst, bright); pDst += 2; pSrc += 2;
			case 5:   BRIGHT_BLEND_16_565(*(S_WORD*)pSrc, *(S_WORD*)pDst, bright); pDst += 2; pSrc += 2;
			case 4:   BRIGHT_BLEND_16_565(*(S_WORD*)pSrc, *(S_WORD*)pDst, bright); pDst += 2; pSrc += 2;
			case 3:   BRIGHT_BLEND_16_565(*(S_WORD*)pSrc, *(S_WORD*)pDst, bright); pDst += 2; pSrc += 2;
			case 2:   BRIGHT_BLEND_16_565(*(S_WORD*)pSrc, *(S_WORD*)pDst, bright); pDst += 2; pSrc += 2;
			case 1:   BRIGHT_BLEND_16_565(*(S_WORD*)pSrc, *(S_WORD*)pDst, bright); pDst += 2; pSrc += 2;
					}while (--w);
			}
			pDst += m_WidthBytes - (wbak<<1);
			pSrc += pBMP->m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pDst += dy * m_WidthBytes + dx*3;
		pSrc += sy * pBMP->m_WidthBytes + sx*3;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{BRIGHT_BLEND_24(pSrc, pDst, bright); pSrc += 3; pDst += 3;
			case 7:   BRIGHT_BLEND_24(pSrc, pDst, bright); pSrc += 3; pDst += 3;
			case 6:   BRIGHT_BLEND_24(pSrc, pDst, bright); pSrc += 3; pDst += 3;
			case 5:   BRIGHT_BLEND_24(pSrc, pDst, bright); pSrc += 3; pDst += 3;
			case 4:   BRIGHT_BLEND_24(pSrc, pDst, bright); pSrc += 3; pDst += 3;
			case 3:   BRIGHT_BLEND_24(pSrc, pDst, bright); pSrc += 3; pDst += 3;
			case 2:   BRIGHT_BLEND_24(pSrc, pDst, bright); pSrc += 3; pDst += 3;
			case 1:   BRIGHT_BLEND_24(pSrc, pDst, bright); pSrc += 3; pDst += 3;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak*3;
			pSrc += pBMP->m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

#define ALPHA_BLEND_16_565(ps, pd, alpha)	\
	do {						\
		S_DWORD src = *(S_WORD*)(ps);			\
		S_DWORD dst = *(S_WORD*)(pd);			\
		src = (src | src << 16) & 0x07e0f81f;		\
		dst = (dst | dst << 16) & 0x07e0f81f;		\
		dst += (src - dst) * (alpha) >> 5;			\
		dst &= 0x07e0f81f;				\
		*(S_WORD*)(pd) = (S_WORD)(dst | dst >> 16);			\
		} while(0)

#define ALPHA_BLEND_24(ps, pd, alpha)	\
	do {						\
		S_WORD sr = *((ps)+0);			\
		S_WORD sg = *((ps)+1);			\
		S_WORD sb = *((ps)+2);			\
		S_WORD dr = *((pd)+0);			\
		S_WORD dg = *((pd)+1);			\
		S_WORD db = *((pd)+2);			\
		dr += (sr-dr)*(alpha) >> 8;			\
		dg += (sg-dg)*(alpha) >> 8;			\
		db += (sb-db)*(alpha) >> 8;			\
		*((pd)+0) = (S_BYTE)dr;			\
		*((pd)+1) = (S_BYTE)dg;			\
		*((pd)+2) = (S_BYTE)db;			\
		} while(0)

bool C_HGUIBMP::BitBltAlphaCopy(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIBMP *pBMP, S_WORD sx, S_WORD sy, S_BYTE alpha)
{
	S_SHORT wbak = w;
	S_BYTE *pDst = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	const S_BYTE *pSrc = (S_BYTE *)pBMP->m_HGuiBuffer.m_pBuffer;

	switch (m_BPP)
	{
	case 16:
		pDst += dy * m_WidthBytes + (dx<<1);
		pSrc += sy * pBMP->m_WidthBytes + (sx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 7:   ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 6:   ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 5:   ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 4:   ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 3:   ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 2:   ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 1:   ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
					}while (--w);
			}
			pDst += m_WidthBytes - (wbak<<1);
			pSrc += pBMP->m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pDst += dy * m_WidthBytes + dx*3;
		pSrc += sy * pBMP->m_WidthBytes + sx*3;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 7:   ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 6:   ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 5:   ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 4:   ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 3:   ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 2:   ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 1:   ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak*3;
			pSrc += pBMP->m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
}

#define TRANS_COPY_24(pSrc, pDst, ColorKey)		\
	do{		\
		S_DWORD CurColor = ((S_DWORD)(*((pSrc)+2))<<16)|((S_DWORD)(*((pSrc)+1))<<8)|((S_DWORD)(*(pSrc)));	\
		if (CurColor != (ColorKey))	\
		{	\
			*((pDst)+0) = *((pSrc)+0);	\
			*((pDst)+1) = *((pSrc)+1);	\
			*((pDst)+2) = *((pSrc)+2);	\
		}	\
	}while (0)

bool C_HGUIBMP::BitBltTransCopy(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIBMP *pBMP, S_WORD sx, S_WORD sy, S_DWORD ColorKey)
{
	S_WORD wbak = w;
	S_BYTE ColorKey8 = (S_BYTE)ColorKey;
	S_WORD ColorKey16 = (S_WORD)ColorKey;
	S_DWORD ColorKey24 = (S_DWORD)ColorKey;
	S_BYTE *pDst = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	const S_BYTE *pSrc = (S_BYTE *)pBMP->m_HGuiBuffer.m_pBuffer;

	switch (m_BPP)
	{
	case 8:
		pDst += dy * m_WidthBytes + dx;
		pSrc += sy * pBMP->m_WidthBytes + sx;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{if (*pSrc != ColorKey8) *pDst = *pSrc; pDst++; pSrc++;
			case 7:   if (*pSrc != ColorKey8) *pDst = *pSrc; pDst++; pSrc++;
			case 6:   if (*pSrc != ColorKey8) *pDst = *pSrc; pDst++; pSrc++;
			case 5:   if (*pSrc != ColorKey8) *pDst = *pSrc; pDst++; pSrc++;
			case 4:   if (*pSrc != ColorKey8) *pDst = *pSrc; pDst++; pSrc++;
			case 3:   if (*pSrc != ColorKey8) *pDst = *pSrc; pDst++; pSrc++;
			case 2:   if (*pSrc != ColorKey8) *pDst = *pSrc; pDst++; pSrc++;
			case 1:   if (*pSrc != ColorKey8) *pDst = *pSrc; pDst++; pSrc++;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak;
			pSrc += pBMP->m_WidthBytes - wbak;
		}
		return true;
	case 16:
		pDst += dy * m_WidthBytes + (dx<<1);
		pSrc += sy * pBMP->m_WidthBytes + (sx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{if (*(S_WORD*)pSrc != ColorKey16) *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 7:   if (*(S_WORD*)pSrc != ColorKey16) *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 6:   if (*(S_WORD*)pSrc != ColorKey16) *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 5:   if (*(S_WORD*)pSrc != ColorKey16) *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 4:   if (*(S_WORD*)pSrc != ColorKey16) *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 3:   if (*(S_WORD*)pSrc != ColorKey16) *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 2:   if (*(S_WORD*)pSrc != ColorKey16) *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
			case 1:   if (*(S_WORD*)pSrc != ColorKey16) *(S_WORD*)pDst = *(S_WORD*)pSrc; pDst += 2; pSrc += 2;
					}while (--w);
			}
			pDst += m_WidthBytes - (wbak<<1);
			pSrc += pBMP->m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pDst += dy * m_WidthBytes + dx*3;
		pSrc += sy * pBMP->m_WidthBytes + sx*3;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{TRANS_COPY_24(pSrc, pDst, ColorKey24); pSrc += 3; pDst += 3;
			case 7:   TRANS_COPY_24(pSrc, pDst, ColorKey24); pSrc += 3; pDst += 3;
			case 6:   TRANS_COPY_24(pSrc, pDst, ColorKey24); pSrc += 3; pDst += 3;
			case 5:   TRANS_COPY_24(pSrc, pDst, ColorKey24); pSrc += 3; pDst += 3;
			case 4:   TRANS_COPY_24(pSrc, pDst, ColorKey24); pSrc += 3; pDst += 3;
			case 3:   TRANS_COPY_24(pSrc, pDst, ColorKey24); pSrc += 3; pDst += 3;
			case 2:   TRANS_COPY_24(pSrc, pDst, ColorKey24); pSrc += 3; pDst += 3;
			case 1:   TRANS_COPY_24(pSrc, pDst, ColorKey24); pSrc += 3; pDst += 3;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak*3;
			pSrc += pBMP->m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;;
	}
	return false;
}

bool C_HGUIBMP::BitBltTransAlphaCpy(S_WORD dx, S_WORD dy, S_WORD w, S_WORD h, const C_HGUIBMP *pBMP, S_WORD sx, S_WORD sy, S_BYTE alpha, S_DWORD ColorKey)
{
	S_WORD wbak = w;
	S_WORD ColorKey16 = (S_WORD)ColorKey;
	S_DWORD ColorKey24 = ColorKey;
	S_BYTE *pDst = (S_BYTE *)m_HGuiBuffer.m_pBuffer;
	const S_BYTE *pSrc = (S_BYTE *)pBMP->m_HGuiBuffer.m_pBuffer;

	switch (m_BPP)
	{
	case 16:
		pDst += dy * m_WidthBytes + (dx<<1);
		pSrc += sy * pBMP->m_WidthBytes + (sx<<1);
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{if (*(S_WORD*)pSrc!=ColorKey16) ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 7:   if (*(S_WORD*)pSrc!=ColorKey16) ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 6:   if (*(S_WORD*)pSrc!=ColorKey16) ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 5:   if (*(S_WORD*)pSrc!=ColorKey16) ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 4:   if (*(S_WORD*)pSrc!=ColorKey16) ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 3:   if (*(S_WORD*)pSrc!=ColorKey16) ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 2:   if (*(S_WORD*)pSrc!=ColorKey16) ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
			case 1:   if (*(S_WORD*)pSrc!=ColorKey16) ALPHA_BLEND_16_565(pSrc, pDst, alpha); pDst += 2; pSrc += 2;
					}while (--w);
			}
			pDst += m_WidthBytes - (wbak<<1);
			pSrc += pBMP->m_WidthBytes - (wbak<<1);
		}
		return true;
	case 24:
		pDst += dy * m_WidthBytes + dx*3;
		pSrc += sy * pBMP->m_WidthBytes + sx*3;
		while (h--)
		{
			w = (wbak+7)>>3;
			switch (wbak%8)
			{
			case 0:do{if (HAI_MAKEDWORD3(pSrc)!=ColorKey24) ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 7:   if (HAI_MAKEDWORD3(pSrc)!=ColorKey24) ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 6:   if (HAI_MAKEDWORD3(pSrc)!=ColorKey24) ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 5:   if (HAI_MAKEDWORD3(pSrc)!=ColorKey24) ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 4:   if (HAI_MAKEDWORD3(pSrc)!=ColorKey24) ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 3:   if (HAI_MAKEDWORD3(pSrc)!=ColorKey24) ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 2:   if (HAI_MAKEDWORD3(pSrc)!=ColorKey24) ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
			case 1:   if (HAI_MAKEDWORD3(pSrc)!=ColorKey24) ALPHA_BLEND_24(pSrc, pDst, alpha); pSrc += 3; pDst += 3;
					}while (--w);
			}
			pDst += m_WidthBytes - wbak*3;
			pSrc += pBMP->m_WidthBytes - wbak*3;
		}
		return true;
	default:
		break;
	}
	return false;
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

C_HGUISCRN HGUI_LcdScrn;


