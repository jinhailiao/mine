//---------------------------------------------------------------------------------
// Copyright (c) Haisoft 2006-9-1
// Copyright jinhailiao 2008-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			HGuibmp.h
// Description:		BMP decode; reference CxImage
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 2.0.0		2006-09-06  create                  Kingsea
// 3.0.0		2008-03-01	update					Kingsea
//---------------------------------------------------------------------------------
#include "hguidef.h"
#include "hguipic.h"

//
// BMP 子函数
//
#define BI_RGB			0L
#define BI_RLE8			1L
#define BI_RLE4			2L
#define BI_BITFIELDS		3L
#define MASKWIDTH(w)	((((w)+31)>>5)<<2)

struct S_RGBQUAD
{
	S_BYTE    rgbRed;
	S_BYTE    rgbGreen;
	S_BYTE    rgbBlue;
	S_BYTE    rgbReserved;
};

struct S_BITMAPINFOHEADER
{
	S_DWORD      biSize;
	S_LONG        biWidth;
	S_LONG        biHeight;
	S_WORD       biPlanes;
	S_WORD       biBitCount;
	S_DWORD      biCompression;
	S_DWORD      biSizeImage;
	S_LONG        biXPelsPerMeter;
	S_LONG        biYPelsPerMeter;
	S_DWORD      biClrUsed;
	S_DWORD      biClrImportant;
};

struct S_BITMAPFILEHEADER
{
	S_WORD    bfType;
	S_DWORD   bfSize;
	S_WORD    bfReserved1;
	S_WORD    bfReserved2;
	S_DWORD   bfOffBits;
};

struct S_BITMAPCOREHEADER
{
	S_DWORD   bcSize;
	S_WORD    bcWidth;
	S_WORD    bcHeight;
	S_WORD    bcPlanes;
	S_WORD    bcBitCount;
};

#define BMP_WIDTH_LIMIT		2000
#define BMP_HEIGHT_LIMIT	2000

#define RLE_COMMAND			0
#define RLE_ENDOFLINE		0
#define RLE_ENDOFBITMAP		1
#define RLE_DELTA			2

#define WIDTHBYTES(i)           ((S_DWORD)(((i)+31)&(~31))>>3)  /* ULONG aligned ! */
#define DibWidthBytesN(lpbi, n) (S_UINT)WIDTHBYTES((S_UINT)(lpbi)->biWidth * (S_UINT)(n))
#define DibWidthBytes(lpbi)     DibWidthBytesN(lpbi, (lpbi)->biBitCount)
#define DibSizeImage(lpbi)      ((lpbi)->biSizeImage == 0 \
                                    ? ((S_DWORD)(S_UINT)DibWidthBytes(lpbi) * (S_DWORD)(S_UINT)(lpbi)->biHeight) \
                                    : (lpbi)->biSizeImage)
#define DibNumColors(lpbi)      ((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8 \
                                    ? (int)(1 << (int)(lpbi)->biBitCount)          \
                                    : (int)(lpbi)->biClrUsed)
#define FixBitmapInfo(lpbi)     if ((lpbi)->biSizeImage == 0)                 \
												(lpbi)->biSizeImage = DibSizeImage(lpbi); \
                                if ((lpbi)->biClrUsed == 0)                   \
                                    (lpbi)->biClrUsed = DibNumColors(lpbi);   \

static S_DWORD _HGui_ReadBitmapFileHeader(const S_BYTE *pBmpData, S_BITMAPFILEHEADER *pbf)
{
	if (pBmpData==S_NULL || pbf==S_NULL)
		return 0;
	
	pbf->bfType      = HAI_MAKEWORD(pBmpData+0); 
	pbf->bfSize      = HAI_MAKEDWORD(pBmpData+2); 
	pbf->bfReserved1 = HAI_MAKEWORD(pBmpData+6); 
	pbf->bfReserved2 = HAI_MAKEWORD(pBmpData+8); 
	pbf->bfOffBits   = HAI_MAKEDWORD(pBmpData+10); 

	if (pbf->bfType == 0x4D42) // "BM"
		return 14;
	return 0;	
}

static S_DWORD _HGui_ReadBitmapInfo(const S_BYTE *pBmpData, S_BITMAPINFOHEADER *pbih)
{
	S_BITMAPCOREHEADER bc;
	
	if (pBmpData==S_NULL || pbih==S_NULL)
		return 0;
	
	pbih->biSize = HAI_MAKEDWORD(pBmpData);
	switch (pbih->biSize)
	{
	case 0x40: //sizeof(OS2_BMP_HEADER):
	case 0x28: //sizeof(S_BITMAPINFOHEADER):
		pbih->biSize          = HAI_MAKEDWORD(pBmpData+0);
		pbih->biWidth         = HAI_MAKEDWORD(pBmpData+4);
		pbih->biHeight        = HAI_MAKEDWORD(pBmpData+8);
		pbih->biPlanes        = HAI_MAKEWORD(pBmpData+12);
		pbih->biBitCount      = HAI_MAKEWORD(pBmpData+14);
		pbih->biCompression   = HAI_MAKEDWORD(pBmpData+16);
		pbih->biSizeImage     = HAI_MAKEDWORD(pBmpData+20);
		pbih->biXPelsPerMeter = HAI_MAKEDWORD(pBmpData+24);
		pbih->biYPelsPerMeter = HAI_MAKEDWORD(pBmpData+28);
		pbih->biClrUsed       = HAI_MAKEDWORD(pBmpData+32);
		pbih->biClrImportant  = HAI_MAKEDWORD(pBmpData+36);
		break;
		
	case 0x0C: //sizeof(S_BITMAPCOREHEADER):
		bc.bcSize             = HAI_MAKEDWORD(pBmpData+0);
		bc.bcWidth            = HAI_MAKEWORD(pBmpData+4);
		bc.bcHeight           = HAI_MAKEWORD(pBmpData+6);
		bc.bcPlanes           = HAI_MAKEWORD(pBmpData+8);
		bc.bcBitCount         = HAI_MAKEWORD(pBmpData+10);
		pbih->biSize          = bc.bcSize;
		pbih->biWidth         = (S_DWORD)bc.bcWidth;
		pbih->biHeight        = (S_DWORD)bc.bcHeight;
		pbih->biPlanes        = bc.bcPlanes;
		pbih->biBitCount      = bc.bcBitCount;
		pbih->biCompression   = BI_RGB;
		pbih->biSizeImage     = 0;
		pbih->biXPelsPerMeter = 0;
		pbih->biYPelsPerMeter = 0;
		pbih->biClrUsed       = 0;
		pbih->biClrImportant  = 0;
		break;
	default:
		pbih->biSize = 0;
		break;
	}

	FixBitmapInfo(pbih);
	return pbih->biSize;
}

static bool _HGui_ExtractBits841BI_RGB(S_BYTE *pImage, S_BmpInfo &BmpInfo, S_BITMAPINFOHEADER *pbih, const S_BYTE *pBmpData, S_RGBQUAD *pPalette, S_BOOL bTopDownDib)
{
	S_BYTE c, *pDst;
	const S_BYTE *pSrc;
	S_WORD i, j;
	S_WORD WidthBytes = (S_WORD)BITMAP_WIDTHBYTES(pbih->biWidth, pbih->biBitCount);

	switch (pbih->biBitCount)
	{
	case 8:
		for (i = 0; i < pbih->biHeight; i++)
		{
			pDst = pImage+i*BmpInfo.m_WidthBytes;
			if (bTopDownDib)
				pSrc = pBmpData+i*WidthBytes;
			else
				pSrc = pBmpData+(pbih->biHeight-i-1)*WidthBytes;
			j = (S_WORD)((pbih->biWidth+7)>>3);
			switch (pbih->biWidth % 8)
			{
			case 0:do{c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
			case 7:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
			case 6:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
			case 5:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
			case 4:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
			case 3:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
			case 2:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
			case 1:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
					}while (--j);
			}
		}
		break;
	case 4:
		for (i = 0; i < pbih->biHeight; i++)
		{
			pDst = pImage+i*BmpInfo.m_WidthBytes;
			if (bTopDownDib)
				pSrc = pBmpData+i*WidthBytes;
			else
				pSrc = pBmpData+(pbih->biHeight-i-1)*WidthBytes;
			j = (S_WORD)pbih->biWidth;
			while (j >= 2){
				j -= 2;
				c = *pSrc++;
				*pDst++ = pPalette[(c>>4)&0X0F].rgbRed;
				*pDst++ = pPalette[(c>>4)&0X0F].rgbGreen;
				*pDst++ = pPalette[(c>>4)&0X0F].rgbBlue;
				*pDst++ = pPalette[c&0X0F].rgbRed;
				*pDst++ = pPalette[c&0X0F].rgbGreen;
				*pDst++ = pPalette[c&0X0F].rgbBlue;
			}
			if (j == 1){
				c = *pSrc++;
				*pDst++ = pPalette[(c>>4)&0X0F].rgbRed;
				*pDst++ = pPalette[(c>>4)&0X0F].rgbGreen;
				*pDst++ = pPalette[(c>>4)&0X0F].rgbBlue;
			}
		}
		break;
	case 1:
		for (i = 0; i < pbih->biHeight; i++)
		{
			pDst = pImage+i*BmpInfo.m_WidthBytes;
			if (bTopDownDib)
				pSrc = pBmpData+i*WidthBytes;
			else
				pSrc = pBmpData+(pbih->biHeight-i-1)*WidthBytes;
			j = (S_WORD)pbih->biWidth;
			while (j >= 8){
				j -= 8;
				c = *pSrc++;
				*pDst++ = pPalette[(c>>7)&0X01].rgbRed;*pDst++ = pPalette[(c>>7)&0X01].rgbGreen;*pDst++ = pPalette[(c>>7)&0X01].rgbBlue;
				*pDst++ = pPalette[(c>>6)&0X01].rgbRed;*pDst++ = pPalette[(c>>6)&0X01].rgbGreen;*pDst++ = pPalette[(c>>6)&0X01].rgbBlue;
				*pDst++ = pPalette[(c>>5)&0X01].rgbRed;*pDst++ = pPalette[(c>>5)&0X01].rgbGreen;*pDst++ = pPalette[(c>>5)&0X01].rgbBlue;
				*pDst++ = pPalette[(c>>4)&0X01].rgbRed;*pDst++ = pPalette[(c>>4)&0X01].rgbGreen;*pDst++ = pPalette[(c>>4)&0X01].rgbBlue;
				*pDst++ = pPalette[(c>>3)&0X01].rgbRed;*pDst++ = pPalette[(c>>3)&0X01].rgbGreen;*pDst++ = pPalette[(c>>3)&0X01].rgbBlue;
				*pDst++ = pPalette[(c>>2)&0X01].rgbRed;*pDst++ = pPalette[(c>>2)&0X01].rgbGreen;*pDst++ = pPalette[(c>>2)&0X01].rgbBlue;
				*pDst++ = pPalette[(c>>1)&0X01].rgbRed;*pDst++ = pPalette[(c>>1)&0X01].rgbGreen;*pDst++ = pPalette[(c>>1)&0X01].rgbBlue;
				*pDst++ = pPalette[(c>>0)&0X01].rgbRed;*pDst++ = pPalette[(c>>0)&0X01].rgbGreen;*pDst++ = pPalette[(c>>0)&0X01].rgbBlue;
			}
			if (j)
			{
				S_SHORT w = (S_SHORT)j;
				c = *pSrc++;
				{*pDst++ = pPalette[(c>>7)&0X01].rgbRed;*pDst++ = pPalette[(c>>7)&0X01].rgbGreen;*pDst++ = pPalette[(c>>7)&0X01].rgbBlue;}
				if (--w > 0)
					{*pDst++ = pPalette[(c>>6)&0X01].rgbRed;*pDst++ = pPalette[(c>>6)&0X01].rgbGreen;*pDst++ = pPalette[(c>>6)&0X01].rgbBlue;}
				if (--w > 0)
					{*pDst++ = pPalette[(c>>5)&0X01].rgbRed;*pDst++ = pPalette[(c>>5)&0X01].rgbGreen;*pDst++ = pPalette[(c>>5)&0X01].rgbBlue;}
				if (--w > 0)
					{*pDst++ = pPalette[(c>>4)&0X01].rgbRed;*pDst++ = pPalette[(c>>4)&0X01].rgbGreen;*pDst++ = pPalette[(c>>4)&0X01].rgbBlue;}
				if (--w > 0)
					{*pDst++ = pPalette[(c>>3)&0X01].rgbRed;*pDst++ = pPalette[(c>>3)&0X01].rgbGreen;*pDst++ = pPalette[(c>>3)&0X01].rgbBlue;}
				if (--w > 0)
					{*pDst++ = pPalette[(c>>2)&0X01].rgbRed;*pDst++ = pPalette[(c>>2)&0X01].rgbGreen;*pDst++ = pPalette[(c>>2)&0X01].rgbBlue;}
				if (--w > 0)
					{*pDst++ = pPalette[(c>>1)&0X01].rgbRed;*pDst++ = pPalette[(c>>1)&0X01].rgbGreen;*pDst++ = pPalette[(c>>1)&0X01].rgbBlue;}
				if (--w > 0)
					{*pDst++ = pPalette[(c>>0)&0X01].rgbRed;*pDst++ = pPalette[(c>>0)&0X01].rgbGreen;*pDst++ = pPalette[(c>>0)&0X01].rgbBlue;}
			}
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}

static bool _HGui_ExtractBits841BI_RLE4(S_BYTE *pImage, S_BmpInfo &BmpInfo, const S_BYTE *pBmpData, S_RGBQUAD *pPalette, S_BOOL bTopDownDib)
{
	bool bContinue = true;
	S_BYTE StatusByte = 0;
	S_BYTE SecondByte = 0;
	S_WORD i, bits = 0;
	S_DWORD ScanLine = 0;
	S_BYTE *pDst;

	while (bContinue == true)
	{
		StatusByte = *pBmpData++;
		switch (StatusByte) {
			case RLE_COMMAND :
				StatusByte = *pBmpData++;
				switch (StatusByte) {
					case RLE_ENDOFLINE :
						bits = 0;
						ScanLine++;
						break;
					case RLE_ENDOFBITMAP :
						bContinue = false;
						break;
					case RLE_DELTA :
						bits       += *pBmpData++;
						ScanLine+= *pBmpData++;
						break;
					default :
						if (bTopDownDib)
							pDst = pImage + ScanLine*BmpInfo.m_WidthBytes + bits*3;
						else
							pDst = pImage + (BmpInfo.m_height-ScanLine-1)*BmpInfo.m_WidthBytes + bits*3;
						bits += StatusByte;
						for (i = 0; i < StatusByte; i++, pBmpData++) {
							*pDst++ = pPalette[*pBmpData>>4].rgbRed;
							*pDst++ = pPalette[*pBmpData>>4].rgbGreen;
							*pDst++ = pPalette[*pBmpData>>4].rgbBlue;
							if (++i >= StatusByte)
							{
								pBmpData++;
								break;
							}
							*pDst++ = pPalette[*pBmpData&0xF].rgbRed;
							*pDst++ = pPalette[*pBmpData&0xF].rgbGreen;
							*pDst++ = pPalette[*pBmpData&0xF].rgbBlue;
						}
						// in absolute mode, each run must be aligned on a word boundary. 
						if (((StatusByte+1)>>1)&1)
							pBmpData++;												
						break;
				};
				break;
			default :
				if (bTopDownDib)
					pDst = pImage + ScanLine*BmpInfo.m_WidthBytes + bits*3;
				else
					pDst = pImage + (BmpInfo.m_height-ScanLine-1)*BmpInfo.m_WidthBytes + bits*3;
				SecondByte = *pBmpData++;
				bits += StatusByte;
				for (i = 0; i < StatusByte; i++) {
					*pDst++ = pPalette[SecondByte>>4].rgbRed;
					*pDst++ = pPalette[SecondByte>>4].rgbGreen;
					*pDst++ = pPalette[SecondByte>>4].rgbBlue;
					
					if (++i >= StatusByte)
					{
						pBmpData++;
						break;
					}
					*pDst++ = pPalette[SecondByte&0x0F].rgbRed;
					*pDst++ = pPalette[SecondByte&0x0F].rgbGreen;
					*pDst++ = pPalette[SecondByte&0x0F].rgbBlue;
				}
				break;
		};
	}
	return true;
}

static bool _HGui_ExtractBits841BI_RLE8(S_BYTE *pImage, S_BmpInfo &BmpInfo, const S_BYTE *pBmpData, S_RGBQUAD *pPalette, S_BOOL bTopDownDib)
{
	bool bContinue = true;
	S_BYTE StatusByte = 0;
	S_BYTE SecondByte = 0;
	S_WORD i, bits = 0;
	S_DWORD ScanLine = 0;
	S_BYTE *pDst = pImage;

	while (bContinue == true)
	{
		StatusByte = *pBmpData++;
		switch (StatusByte) {
			case RLE_COMMAND :
				StatusByte = *pBmpData++;
				switch (StatusByte) {
					case RLE_ENDOFLINE :
						bits = 0;
						ScanLine++;
						break;
					case RLE_ENDOFBITMAP :
						bContinue=false;
						break;
					case RLE_DELTA :
						bits       += *pBmpData++;
						ScanLine += *pBmpData++;
						break;
					default :
						if (bTopDownDib)
							pDst = pImage + BmpInfo.m_WidthBytes*ScanLine + bits*3;
						else
							pDst = pImage + BmpInfo.m_WidthBytes*(BmpInfo.m_height-ScanLine-1) + bits*3;
						for (i = 0; i < StatusByte; i++,pBmpData++)
						{
							*pDst++ = pPalette[*pBmpData].rgbRed;
							*pDst++ = pPalette[*pBmpData].rgbGreen;
							*pDst++ = pPalette[*pBmpData].rgbBlue;
						}
						// in absolute mode, each run must be aligned on a word boundary. 
						if ((StatusByte & 1) == 1)
							pBmpData++;												
						bits += StatusByte;													
						break;								
				};
				break;
			default :
				if (bTopDownDib)
					pDst = pImage + BmpInfo.m_WidthBytes*ScanLine + bits*3;
				else
					pDst = pImage + BmpInfo.m_WidthBytes*(BmpInfo.m_height-ScanLine-1) + bits*3;
				SecondByte = *pBmpData++;
				bits += StatusByte;
				for (i = 0; i < StatusByte; i++) {
					*pDst++ = pPalette[SecondByte].rgbRed;
					*pDst++ = pPalette[SecondByte].rgbGreen;
					*pDst++ = pPalette[SecondByte].rgbBlue;
				}
				break;
		};
	}
	return true;
}

//
// PCX 子函数
//
struct S_PCXHEADER
{
  S_CHAR Manufacturer;		// always 0X0A
  S_CHAR Version;			// version number
  S_CHAR Encoding;			// always 1
  S_CHAR BitsPerPixel;		// color bits
  S_WORD Xmin, Ymin;		// image origin
  S_WORD Xmax, Ymax;		// image dimensions
  S_WORD Hres, Vres;		// resolution values
  S_BYTE ColorMap[16][3];	// color palette
  S_CHAR Reserved;
  S_CHAR ColorPlanes;		// color planes
  S_WORD BytesPerLine;		// line buffer size
  S_WORD PaletteType;		// grey or color palette
  S_CHAR Filter[58];
};

#define PCX_MAGIC 0X0A			// PCX magic number
#define PCX_256_COLORS 0X0C		// magic number for 256 colors
#define PCX_HDR_SIZE 128		// size of PCX header
#define PCX_MAXCOLORS 256
#define PCX_MAXPLANES 4
#define PCX_MAXVAL 255

static bool _hai_PcxDeal3x8(S_BYTE *pPcxPixels, S_BYTE *pBitplanes, S_WORD BytesPerLine, S_WORD Width)
{
	S_WORD i = (Width+7)>>3;
	S_BYTE *pB = pBitplanes;
	S_BYTE *pG = pBitplanes+BytesPerLine;
	S_BYTE *pR = pBitplanes+BytesPerLine+BytesPerLine;

	switch (Width%8)
	{
	case 0:do{*pPcxPixels++ = *pR++; *pPcxPixels++ = *pG++; *pPcxPixels++ = *pB++;
	case 7:     *pPcxPixels++ = *pR++; *pPcxPixels++ = *pG++; *pPcxPixels++ = *pB++;
	case 6:     *pPcxPixels++ = *pR++; *pPcxPixels++ = *pG++; *pPcxPixels++ = *pB++;
	case 5:     *pPcxPixels++ = *pR++; *pPcxPixels++ = *pG++; *pPcxPixels++ = *pB++;
	case 4:     *pPcxPixels++ = *pR++; *pPcxPixels++ = *pG++; *pPcxPixels++ = *pB++;
	case 3:     *pPcxPixels++ = *pR++; *pPcxPixels++ = *pG++; *pPcxPixels++ = *pB++;
	case 2:     *pPcxPixels++ = *pR++; *pPcxPixels++ = *pG++; *pPcxPixels++ = *pB++;
	case 1:     *pPcxPixels++ = *pR++; *pPcxPixels++ = *pG++; *pPcxPixels++ = *pB++;
			}while (--i);
	}
	return true;
}

static bool _hai_PcxPlanesToPixels(S_BYTE *pPixels, S_BYTE *pBitPlanes, S_WORD Width, S_WORD BytesPerLine, S_WORD Planes, S_WORD BitsPerPixel, S_BYTE pPalette[][3])
{
	S_INT i, j;
	S_BYTE *p = pPixels;
	S_INT pixbit, bits, mask;
	
	if (Planes > 4 || BitsPerPixel != 1)
		return false;

	memset(p, 0x00, (BytesPerLine<<3)/BitsPerPixel);
	
	for (i = 0; i < Planes; i++){
		pixbit = (1 << i);
		p = pPixels;
		for (j = 0; j < BytesPerLine; j++){
			bits = *pBitPlanes++;
			for (mask = 0X80; mask != 0; mask >>= 1, p++)
				if (bits & mask) *p |= pixbit;
		}
	}

	p = pPixels + Width*3 - 1;
	pPixels += Width - 1;
	i = (Width+7)>>3;
	switch (Width%8)
	{
	case 0:do{*p-- = pPalette[*pPixels][2]; *p-- = pPalette[*pPixels][1]; *p-- = pPalette[*pPixels][0]; pPixels--;
	case 7:   *p-- = pPalette[*pPixels][2]; *p-- = pPalette[*pPixels][1]; *p-- = pPalette[*pPixels][0]; pPixels--;
	case 6:   *p-- = pPalette[*pPixels][2]; *p-- = pPalette[*pPixels][1]; *p-- = pPalette[*pPixels][0]; pPixels--;
	case 5:   *p-- = pPalette[*pPixels][2]; *p-- = pPalette[*pPixels][1]; *p-- = pPalette[*pPixels][0]; pPixels--;
	case 4:   *p-- = pPalette[*pPixels][2]; *p-- = pPalette[*pPixels][1]; *p-- = pPalette[*pPixels][0]; pPixels--;
	case 3:   *p-- = pPalette[*pPixels][2]; *p-- = pPalette[*pPixels][1]; *p-- = pPalette[*pPixels][0]; pPixels--;
	case 2:   *p-- = pPalette[*pPixels][2]; *p-- = pPalette[*pPixels][1]; *p-- = pPalette[*pPixels][0]; pPixels--;
	case 1:   *p-- = pPalette[*pPixels][2]; *p-- = pPalette[*pPixels][1]; *p-- = pPalette[*pPixels][0]; pPixels--;
			}while (--i);
	}

	return true;
}

static bool _hsi_PcxUnpackPixels(S_BYTE *pPixels, S_BYTE *pBitPlanes, S_WORD Width, S_WORD Planes, S_WORD BitsPerPixel, S_BYTE pPalette[][3])
{
	S_INT bits;
	S_WORD cnt;
	
	if (Planes != 1)
		return false;
	switch (BitsPerPixel)
	{
	case 8:
		cnt = (Width+7)>>3;
		switch (Width%8)
		{
		case 0:do{bits = (S_INT)*pBitPlanes++;*pPixels++ = pPalette[bits][0];*pPixels++ = pPalette[bits][1];*pPixels++ = pPalette[bits][2];
		case 7:   bits = (S_INT)*pBitPlanes++;*pPixels++ = pPalette[bits][0];*pPixels++ = pPalette[bits][1];*pPixels++ = pPalette[bits][2];
		case 6:   bits = (S_INT)*pBitPlanes++;*pPixels++ = pPalette[bits][0];*pPixels++ = pPalette[bits][1];*pPixels++ = pPalette[bits][2];
		case 5:   bits = (S_INT)*pBitPlanes++;*pPixels++ = pPalette[bits][0];*pPixels++ = pPalette[bits][1];*pPixels++ = pPalette[bits][2];
		case 4:   bits = (S_INT)*pBitPlanes++;*pPixels++ = pPalette[bits][0];*pPixels++ = pPalette[bits][1];*pPixels++ = pPalette[bits][2];
		case 3:   bits = (S_INT)*pBitPlanes++;*pPixels++ = pPalette[bits][0];*pPixels++ = pPalette[bits][1];*pPixels++ = pPalette[bits][2];
		case 2:   bits = (S_INT)*pBitPlanes++;*pPixels++ = pPalette[bits][0];*pPixels++ = pPalette[bits][1];*pPixels++ = pPalette[bits][2];
		case 1:   bits = (S_INT)*pBitPlanes++;*pPixels++ = pPalette[bits][0];*pPixels++ = pPalette[bits][1];*pPixels++ = pPalette[bits][2];
				}while (--cnt);
		}
		break;
	case 4:
		while (Width >= 2){
			Width -= 2;
			bits = *pBitPlanes++;
			*pPixels++ = pPalette[(bits>>4)&0X0F][0];
			*pPixels++ = pPalette[(bits>>4)&0X0F][1];
			*pPixels++ = pPalette[(bits>>4)&0X0F][2];
			*pPixels++ = pPalette[bits&0X0F][0];
			*pPixels++ = pPalette[bits&0X0F][1];
			*pPixels++ = pPalette[bits&0X0F][2];
		}
		if (Width == 1){
			bits = *pBitPlanes++;
			*pPixels++ = pPalette[(bits>>4)&0X0F][0];
			*pPixels++ = pPalette[(bits>>4)&0X0F][1];
			*pPixels++ = pPalette[(bits>>4)&0X0F][2];
		}
		break;
	case 2:
		while (Width >= 4){
			Width -= 4;
			bits = *pBitPlanes++;
			*pPixels++ = pPalette[(bits>>6)&0X03][0];*pPixels++ = pPalette[(bits>>6)&0X03][1];*pPixels++ = pPalette[(bits>>6)&0X03][2];
			*pPixels++ = pPalette[(bits>>4)&0X03][0];*pPixels++ = pPalette[(bits>>4)&0X03][1];*pPixels++ = pPalette[(bits>>4)&0X03][2];
			*pPixels++ = pPalette[(bits>>2)&0X03][0];*pPixels++ = pPalette[(bits>>2)&0X03][1];*pPixels++ = pPalette[(bits>>2)&0X03][2];
			*pPixels++ = pPalette[(bits>>0)&0X03][0];*pPixels++ = pPalette[(bits>>0)&0X03][1];*pPixels++ = pPalette[(bits>>0)&0X03][2];
		}
		if (Width)
		{
			S_SHORT w = (S_SHORT)Width;
			bits = *pBitPlanes++;
			*pPixels++ = pPalette[(bits>>6)&0X03][0];*pPixels++ = pPalette[(bits>>6)&0X03][1];*pPixels++ = pPalette[(bits>>6)&0X03][2];
			if (--w > 0)
				{*pPixels++ = pPalette[(bits>>4)&0X03][0];*pPixels++ = pPalette[(bits>>4)&0X03][1];*pPixels++ = pPalette[(bits>>4)&0X03][2];}
			if (--w > 0)
				{*pPixels++ = pPalette[(bits>>2)&0X03][0];*pPixels++ = pPalette[(bits>>2)&0X03][1];*pPixels++ = pPalette[(bits>>2)&0X03][2];}
		}
		break;
	case 1:
		while (Width >= 8){
			Width -= 8;
			bits = *pBitPlanes++;
			*pPixels++ = pPalette[(bits>>7)&0X01][0];*pPixels++ = pPalette[(bits>>7)&0X01][1];*pPixels++ = pPalette[(bits>>7)&0X01][2];
			*pPixels++ = pPalette[(bits>>6)&0X01][0];*pPixels++ = pPalette[(bits>>6)&0X01][1];*pPixels++ = pPalette[(bits>>6)&0X01][2];
			*pPixels++ = pPalette[(bits>>5)&0X01][0];*pPixels++ = pPalette[(bits>>5)&0X01][1];*pPixels++ = pPalette[(bits>>5)&0X01][2];
			*pPixels++ = pPalette[(bits>>4)&0X01][0];*pPixels++ = pPalette[(bits>>4)&0X01][1];*pPixels++ = pPalette[(bits>>4)&0X01][2];
			*pPixels++ = pPalette[(bits>>3)&0X01][0];*pPixels++ = pPalette[(bits>>3)&0X01][1];*pPixels++ = pPalette[(bits>>3)&0X01][2];
			*pPixels++ = pPalette[(bits>>2)&0X01][0];*pPixels++ = pPalette[(bits>>2)&0X01][1];*pPixels++ = pPalette[(bits>>2)&0X01][2];
			*pPixels++ = pPalette[(bits>>1)&0X01][0];*pPixels++ = pPalette[(bits>>1)&0X01][1];*pPixels++ = pPalette[(bits>>1)&0X01][2];
			*pPixels++ = pPalette[(bits>>0)&0X01][0];*pPixels++ = pPalette[(bits>>0)&0X01][1];*pPixels++ = pPalette[(bits>>0)&0X01][2];
		}
		if (Width)
		{
			S_SHORT w = (S_SHORT)Width;
			bits = *pBitPlanes++;
			{*pPixels++ = pPalette[(bits>>7)&0X01][0];*pPixels++ = pPalette[(bits>>7)&0X01][1];*pPixels++ = pPalette[(bits>>7)&0X01][2];}
			if (--w > 0)
				{*pPixels++ = pPalette[(bits>>6)&0X01][0];*pPixels++ = pPalette[(bits>>6)&0X01][1];*pPixels++ = pPalette[(bits>>6)&0X01][2];}
			if (--w > 0)
				{*pPixels++ = pPalette[(bits>>5)&0X01][0];*pPixels++ = pPalette[(bits>>5)&0X01][1];*pPixels++ = pPalette[(bits>>5)&0X01][2];}
			if (--w > 0)
				{*pPixels++ = pPalette[(bits>>4)&0X01][0];*pPixels++ = pPalette[(bits>>4)&0X01][1];*pPixels++ = pPalette[(bits>>4)&0X01][2];}
			if (--w > 0)
				{*pPixels++ = pPalette[(bits>>3)&0X01][0];*pPixels++ = pPalette[(bits>>3)&0X01][1];*pPixels++ = pPalette[(bits>>3)&0X01][2];}
			if (--w > 0)
				{*pPixels++ = pPalette[(bits>>2)&0X01][0];*pPixels++ = pPalette[(bits>>2)&0X01][1];*pPixels++ = pPalette[(bits>>2)&0X01][2];}
			if (--w > 0)
				{*pPixels++ = pPalette[(bits>>1)&0X01][0];*pPixels++ = pPalette[(bits>>1)&0X01][1];*pPixels++ = pPalette[(bits>>1)&0X01][2];}
			if (--w > 0)
				{*pPixels++ = pPalette[(bits>>0)&0X01][0];*pPixels++ = pPalette[(bits>>0)&0X01][1];*pPixels++ = pPalette[(bits>>0)&0X01][2];}
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}

static S_WORD _hai_PcxReadHeader(const S_BYTE *pPcxData, S_PCXHEADER *pph)
{
	if (pPcxData == S_NULL || pph == S_NULL)
		return 0;
	
	pph->Manufacturer = *pPcxData;
	pph->Version      = *(pPcxData+1);
	pph->Encoding     = *(pPcxData+2);
	pph->BitsPerPixel = *(pPcxData+3);
	pph->Xmin = HAI_MAKEWORD(pPcxData+4); 
	pph->Ymin = HAI_MAKEWORD(pPcxData+6);
	pph->Xmax = HAI_MAKEWORD(pPcxData+8);
	pph->Ymax = HAI_MAKEWORD(pPcxData+10);
	pph->Hres = HAI_MAKEWORD(pPcxData+12);
	pph->Vres = HAI_MAKEWORD(pPcxData+14);
	memcpy(pph->ColorMap, pPcxData+16, 16*3);
	pph->Reserved     = *(pPcxData+64);
	pph->ColorPlanes  = *(pPcxData+65);
	pph->BytesPerLine = HAI_MAKEWORD(pPcxData+66);
	pph->PaletteType  = HAI_MAKEWORD(pPcxData+68);
	memcpy(pph->Filter, pPcxData+70, 58);

	return PCX_HDR_SIZE;
}

//
// ICON 子函数
//
struct S_ICONDIRENTRY
{
	S_BYTE  bWidth;
	S_BYTE  bHeight;
	S_BYTE  bColorCount;
	S_BYTE  bReserved;
	S_WORD  wPlanes;
	S_WORD  wBitCount;
	S_DWORD dwBytesInRes;
	S_DWORD dwImageOffset;
};

struct S_ICONHEADER
{
	S_WORD idReserved;
	S_WORD idType;
	S_WORD idCount;
};

//
// C_HGUIPIC
//
S_VOID *C_HGUIPIC::BmpDecode(const S_VOID *pBmpData, S_BmpInfo &BmpInfo)
{
	if (pBmpData == NULL)
		return NULL;

	S_BITMAPFILEHEADER bf;
	const S_BYTE *pBmpDat = (const S_BYTE *)pBmpData;
	S_DWORD SkipBytes = _HGui_ReadBitmapFileHeader(pBmpDat, &bf);
	if (SkipBytes == 0)
		return NULL;

	S_BITMAPINFOHEADER bi;
	pBmpDat += SkipBytes;
	SkipBytes = _HGui_ReadBitmapInfo(pBmpDat, &bi);
	if (SkipBytes == 0)
		return NULL;
	if (bi.biWidth > BMP_WIDTH_LIMIT || bi.biHeight > BMP_HEIGHT_LIMIT)
		return NULL;

	pBmpDat += SkipBytes;

	bool bIsOldBmp = (bi.biSize == 0x0C);//sizeof(S_BITMAPCOREHEADER);
	bool bTopDownDib = (bi.biHeight < 0);
	if (bTopDownDib) 
		bi.biHeight = -bi.biHeight;

	string strPalette;
	S_WORD ColorNum = 0;
	S_RGBQUAD *pPalette = NULL;
	if (ColorNum = DibNumColors(&bi))
	{
		strPalette.append((size_t)(ColorNum*sizeof(S_RGBQUAD)), (char)0);
		pPalette = (S_RGBQUAD *)strPalette.data();
		if (bIsOldBmp){ // convert a old color table (3 byte entries) to a new color table (4 byte entries)
			for (S_WORD i = 0; i < ColorNum; i++){
				pPalette[i].rgbRed      = *pBmpDat++;
				pPalette[i].rgbGreen    = *pBmpDat++;
				pPalette[i].rgbBlue     = *pBmpDat++;
				pPalette[i].rgbReserved = (S_BYTE)0;
			}
		} else {
			for (S_WORD i = 0; i < ColorNum; i++){
				pPalette[i].rgbRed      = *pBmpDat++;
				pPalette[i].rgbGreen    = *pBmpDat++;
				pPalette[i].rgbBlue     = *pBmpDat++;
				pPalette[i].rgbReserved = *pBmpDat++;
			}
			//force rgbReserved=0, to avoid problems with some WinXp bitmaps
			for (S_WORD i = 0; i < bi.biClrUsed; i++)
				pPalette[i].rgbReserved=0;
		}
	}

	S_DWORD WidthBytes = BITMAP_WIDTHBYTES(bi.biWidth, 24);
	string strImage((size_t)(WidthBytes*bi.biHeight+4), (char)0);
	S_BYTE *pImage = (S_BYTE *)strImage.data();

	BmpInfo.m_width     = (S_WORD)bi.biWidth;
	BmpInfo.m_height    = (S_WORD)bi.biHeight;
	BmpInfo.m_BitsPixel = 24;
	BmpInfo.m_WidthBytes = (S_WORD)WidthBytes;

	bool fok = false;
	pBmpDat = (S_BYTE*)pBmpData + bf.bfOffBits;
	WidthBytes = BITMAP_WIDTHBYTES(bi.biWidth, bi.biBitCount);
	switch (bi.biBitCount){
		case 32 :
			if (bi.biCompression == BI_BITFIELDS || bi.biCompression == BI_RGB){
				for (S_WORD i = 0; i < bi.biHeight; i++)
				{
					const S_BYTE *pSrc = NULL;
					S_BYTE *pDst = pImage+i*BmpInfo.m_WidthBytes;
					if (bTopDownDib)
						pSrc = pBmpDat+i*WidthBytes;
					else
						pSrc = pBmpDat+(bi.biHeight-i-1)*WidthBytes;
					S_WORD j = (S_WORD)((bi.biWidth+7)>>3);
					switch (bi.biWidth % 8)
					{
					case 0:do{*pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
					case 7:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
					case 6:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
					case 5:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
					case 4:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
					case 3:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
					case 2:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
					case 1:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
							}while (--j);
					}
				}
				fok = true;
			}
			break;
		case 24 :
			if (bi.biCompression == BI_RGB){
				if (bTopDownDib)
					memcpy(pImage, pBmpDat, BmpInfo.m_WidthBytes*bi.biHeight);
				else
				{
					S_BYTE *pDst = pImage;
					const S_BYTE *pSrc = pBmpDat+(bi.biHeight-1)*BmpInfo.m_WidthBytes;
					for (S_WORD i = 0; i < bi.biHeight; i++)
					{
						memcpy(pDst, pSrc, BmpInfo.m_WidthBytes);
						pDst += BmpInfo.m_WidthBytes;
						pSrc -= BmpInfo.m_WidthBytes;
					}
				}
				fok = true;
			}
			break;
		case 16 :	{
			S_WORD rm, gm, bm;
			S_WORD c, rs=0, gs=0, bs=0;
			if (bi.biCompression == BI_BITFIELDS)
			{
				S_BYTE *pDst = (S_BYTE*)pBmpData+14/*sizeof(S_BITMAPFILEHEADER)*/+SkipBytes;
				rm=(S_WORD)HAI_MAKEDWORD(pDst);
				gm=(S_WORD)HAI_MAKEDWORD(pDst+4);
				bm=(S_WORD)HAI_MAKEDWORD(pDst+8);
			} else {
				rm=0x1F; gm=0x3E0; bm=0x7C00; //RGB555
			}
			for (S_WORD i=0;i<16;i++){
				if ((rm>>i)&0x01) rs++;
				if ((gm>>i)&0x01) gs++;
				if ((bm>>i)&0x01) bs++;
			}
			gs+=rs; bs+=gs; rs=8-rs; gs-=8; bs-=8;
			for (S_WORD i = 0; i < bi.biHeight; i++)
			{
				const S_BYTE *pSrc = NULL;
				S_BYTE *pDst = pImage+i*BmpInfo.m_WidthBytes;
				if (bTopDownDib)
					pSrc = pBmpDat+i*WidthBytes;
				else
					pSrc = pBmpDat+(bi.biHeight-i-1)*WidthBytes;
				S_WORD j = (S_WORD)((bi.biWidth+7)>>3);
				switch (bi.biWidth % 8)
				{
				case 0:do{c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c & rm)<<rs);*pDst++=(S_BYTE)((c & gm)>>gs);*pDst++=(S_BYTE)((c & bm)>>bs);
				case 7:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c & rm)<<rs);*pDst++=(S_BYTE)((c & gm)>>gs);*pDst++=(S_BYTE)((c & bm)>>bs);
				case 6:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c & rm)<<rs);*pDst++=(S_BYTE)((c & gm)>>gs);*pDst++=(S_BYTE)((c & bm)>>bs);
				case 5:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c & rm)<<rs);*pDst++=(S_BYTE)((c & gm)>>gs);*pDst++=(S_BYTE)((c & bm)>>bs);
				case 4:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c & rm)<<rs);*pDst++=(S_BYTE)((c & gm)>>gs);*pDst++=(S_BYTE)((c & bm)>>bs);
				case 3:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c & rm)<<rs);*pDst++=(S_BYTE)((c & gm)>>gs);*pDst++=(S_BYTE)((c & bm)>>bs);
				case 2:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c & rm)<<rs);*pDst++=(S_BYTE)((c & gm)>>gs);*pDst++=(S_BYTE)((c & bm)>>bs);
				case 1:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c & rm)<<rs);*pDst++=(S_BYTE)((c & gm)>>gs);*pDst++=(S_BYTE)((c & bm)>>bs);
						}while (--j);
				}
			}
			fok = true;
			}break;
		case 8 :
		case 4 :
		case 1 :
		switch (bi.biCompression) 
		{
			case BI_RGB :
				fok = _HGui_ExtractBits841BI_RGB(pImage, BmpInfo, &bi, pBmpDat, pPalette, bTopDownDib);
				break;
			case BI_RLE4 :
				fok = _HGui_ExtractBits841BI_RLE4(pImage, BmpInfo, pBmpDat, pPalette, bTopDownDib);
				break;
			case BI_RLE8 :
				fok = _HGui_ExtractBits841BI_RLE8(pImage, BmpInfo, pBmpDat, pPalette, bTopDownDib);
				break;
			default :	
				break;
		}
		default:
			break;
	}

	if (fok == false)
		return NULL;
	pImage = new S_BYTE[strImage.size()];
	if (pImage == NULL)
		return NULL;
	memcpy(pImage, strImage.data(), strImage.size());
	return (S_VOID *)pImage;
}

S_VOID *C_HGUIPIC::PcxDecode(const S_VOID *pPcxData, S_BmpInfo &BmpInfo)
{
	if (pPcxData == S_NULL)
		return NULL;
	
 	S_PCXHEADER pcxHeader;
	_hai_PcxReadHeader((const S_BYTE*)pPcxData, &pcxHeader);
	if (pcxHeader.Manufacturer != PCX_MAGIC) // pcx flag
		return NULL;
	if (pcxHeader.Encoding != 1)// 1 is RLE encode
		return NULL;
	if (pcxHeader.ColorPlanes > 4)
		return NULL;

	S_DWORD Width  = (pcxHeader.Xmax - pcxHeader.Xmin) + 1;
	S_DWORD Height = (pcxHeader.Ymax - pcxHeader.Ymin) + 1;
	S_DWORD nbytes = pcxHeader.BytesPerLine * pcxHeader.ColorPlanes * Height;

	string strPixels;
	strPixels.append((size_t)nbytes, (char)0);
	S_BYTE *pcximage = (S_BYTE *)strPixels.data();
	S_BYTE *pPcxPixels = pcximage;
	const S_BYTE *pPcx = (const S_BYTE*)pPcxData + PCX_HDR_SIZE;
	while (nbytes > 0){
		S_BYTE c = *pPcx++;
		if ((c & 0XC0) != 0XC0){ // Repeated group
			*pPcxPixels++ = c;
			--nbytes;
		}
		else {
			S_INT count = c & 0X3F; // extract count
			c = *pPcx++;
			nbytes -= count;
			while (--count >=0) 
				*pPcxPixels++ = c;
		}
	}

	S_BYTE ColorMap[PCX_MAXCOLORS][3];
	for (S_WORD x = 0; x < 16; x++){
		ColorMap[x][0] = pcxHeader.ColorMap[x][2];
		ColorMap[x][1] = pcxHeader.ColorMap[x][1];
		ColorMap[x][2] = pcxHeader.ColorMap[x][0];
	}
	if (pcxHeader.BitsPerPixel == 8 && pcxHeader.ColorPlanes == 1){
		S_BYTE c = *pPcx++;
		if (c != PCX_256_COLORS)
			return NULL;

		for (S_WORD x = 0; x < PCX_MAXCOLORS; x++){
			ColorMap[x][2] = *pPcx++;
			ColorMap[x][1] = *pPcx++;
			ColorMap[x][0] = *pPcx++;
		}
	}
	if (pcxHeader.BitsPerPixel == 1 && pcxHeader.ColorPlanes == 1){
		ColorMap[0][0] = ColorMap[0][1] = ColorMap[0][2] = 0;
		ColorMap[1][0] = ColorMap[1][1] = ColorMap[1][2] = 255;
	}

	nbytes = BITMAP_WIDTHBYTES(Width, 24);
	string strImage((size_t)(nbytes*Height+4), (char)0);
	S_BYTE *pImage = (S_BYTE *)strImage.data();

	BmpInfo.m_width     = (S_WORD)Width;
	BmpInfo.m_height    = (S_WORD)Height;
	BmpInfo.m_BitsPixel = 24;
	BmpInfo.m_WidthBytes = (S_WORD)nbytes;

	for (S_WORD y = 0; y < Height; y++){
		pPcxPixels = pImage+y*BmpInfo.m_WidthBytes;
		S_BYTE *pBitplanes = pcximage + (y * pcxHeader.BytesPerLine * pcxHeader.ColorPlanes);

		if (pcxHeader.ColorPlanes == 3 && pcxHeader.BitsPerPixel == 8)
			_hai_PcxDeal3x8(pPcxPixels, pBitplanes, pcxHeader.BytesPerLine, (S_WORD)Width);
		else if (pcxHeader.ColorPlanes == 1)
			_hsi_PcxUnpackPixels(pPcxPixels, pBitplanes, (S_WORD)Width, pcxHeader.ColorPlanes, pcxHeader.BitsPerPixel, ColorMap);
		else
			_hai_PcxPlanesToPixels(pPcxPixels, pBitplanes, (S_WORD)Width, pcxHeader.BytesPerLine, pcxHeader.ColorPlanes, pcxHeader.BitsPerPixel, ColorMap);
	}

	pImage = new S_BYTE[strImage.size()];
	if (pImage == NULL)
		return NULL;
	memcpy(pImage, strImage.data(), strImage.size());
	return (S_VOID *)pImage;
}

S_BYTE C_HGUIPIC::GetIconFrameTotal(const S_VOID *pIcoData)
{
	if (pIcoData == S_NULL)
		return 0;

	S_ICONHEADER IconHeader;
	const S_BYTE *picoData = (const S_BYTE *)pIcoData;
	IconHeader.idReserved = HAI_MAKEWORD(picoData+0);
	IconHeader.idType  = HAI_MAKEWORD(picoData+2);
	IconHeader.idCount = HAI_MAKEWORD(picoData+4);

	// check an icon or a cursor
	if (IconHeader.idReserved==0 && (IconHeader.idType==1||IconHeader.idType==2))
		return (S_BYTE)IconHeader.idCount;
	else
		return 0;
}

S_VOID *C_HGUIPIC::IconDecode(const S_VOID *pIcoData, S_IconInfo &IconInfo, S_BYTE frame)
{
	if (pIcoData == NULL)
		return NULL;

	S_ICONHEADER IconHeader;
	const S_BYTE *picoData = (const S_BYTE *)pIcoData;
	IconHeader.idReserved = HAI_MAKEWORD(picoData+0);
	IconHeader.idType  = HAI_MAKEWORD(picoData+2);
	IconHeader.idCount = HAI_MAKEWORD(picoData+4);
	picoData += 6;

	// check an icon or a cursor
	if (!(IconHeader.idReserved==0 && (IconHeader.idType==1||IconHeader.idType==2)))
		return NULL;
	if (frame > IconHeader.idCount)
		return NULL;

	frame -= 1;
	picoData += frame*sizeof(S_ICONDIRENTRY);

	S_ICONDIRENTRY IconDir;
	memcpy(&IconDir, picoData, sizeof(S_ICONDIRENTRY));

	picoData = (S_BYTE *)pIcoData + IconDir.dwImageOffset;
	//read bih;
	S_BITMAPINFOHEADER bih;
	picoData += _HGui_ReadBitmapInfo(picoData, &bih);
	//
	if (bih.biPlanes != 1 || bih.biCompression != BI_RGB)
		return NULL;

	// read the palette
	string strPalette;
	S_RGBQUAD *pPalette = NULL;
	if (bih.biBitCount <= 8)
	{
		strPalette.append((size_t)((1<<bih.biBitCount)*sizeof(S_RGBQUAD)), (char)0);
		pPalette = (S_RGBQUAD *)strPalette.data();
		memcpy(pPalette, picoData, (1<<bih.biBitCount)*sizeof(S_RGBQUAD));
	}
	
	S_DWORD WidthBytes = BITMAP_WIDTHBYTES(bih.biWidth, 24);
	string strImage((size_t)(WidthBytes*bih.biWidth+4), (char)0);

	IconInfo.m_width     = (S_WORD)bih.biWidth;
	IconInfo.m_height    = (S_WORD)bih.biWidth;
	IconInfo.m_BitsPixel = 24;
	IconInfo.m_WidthBytes = (S_WORD)WidthBytes;
	S_BYTE *pImage = (S_BYTE *)strImage.data();
	
	//read the icon
	picoData += (1<<bih.biBitCount)*sizeof(S_RGBQUAD);
	WidthBytes = BITMAP_WIDTHBYTES(bih.biWidth, bih.biBitCount);
	switch (bih.biBitCount)	{
		case 32 :
			for (S_WORD i = 0; i < IconInfo.m_height; i++)
			{
				S_BYTE *pDst = pImage+i*IconInfo.m_WidthBytes;
				const S_BYTE *pSrc = picoData+(IconInfo.m_height-i-1)*WidthBytes;
				S_WORD j = (S_WORD)((bih.biWidth+7)>>3);
				switch (bih.biWidth % 8)
				{
				case 0:do{*pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
				case 7:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
				case 6:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
				case 5:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
				case 4:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
				case 3:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
				case 2:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
				case 1:     *pDst++ = *pSrc++;*pDst++ = *pSrc++;*pDst++ = *pSrc++;pSrc++;
						}while (--j);
				}
			}
			break;
		case 24 :
			memcpy(pImage, picoData, IconInfo.m_WidthBytes*IconInfo.m_height);
			break;
		case 16 :
			for (S_WORD i = 0; i < IconInfo.m_height; i++)
			{
				S_BYTE *pDst = pImage+i*IconInfo.m_WidthBytes;
				const S_BYTE *pSrc = picoData+(IconInfo.m_height-i-1)*WidthBytes;
				S_WORD c = 0, j = (S_WORD)((bih.biWidth+7)>>3);
				switch (bih.biWidth % 8)
				{
				case 0:do{c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c&0x1F)<<3);*pDst++=(S_BYTE)((c&0x3E0)>>2);*pDst++=(S_BYTE)((c&0x7C00)>>7);
				case 7:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c&0x1F)<<3);*pDst++=(S_BYTE)((c&0x3E0)>>2);*pDst++=(S_BYTE)((c&0x7C00)>>7);
				case 6:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c&0x1F)<<3);*pDst++=(S_BYTE)((c&0x3E0)>>2);*pDst++=(S_BYTE)((c&0x7C00)>>7);
				case 5:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c&0x1F)<<3);*pDst++=(S_BYTE)((c&0x3E0)>>2);*pDst++=(S_BYTE)((c&0x7C00)>>7);
				case 4:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c&0x1F)<<3);*pDst++=(S_BYTE)((c&0x3E0)>>2);*pDst++=(S_BYTE)((c&0x7C00)>>7);
				case 3:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c&0x1F)<<3);*pDst++=(S_BYTE)((c&0x3E0)>>2);*pDst++=(S_BYTE)((c&0x7C00)>>7);
				case 2:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c&0x1F)<<3);*pDst++=(S_BYTE)((c&0x3E0)>>2);*pDst++=(S_BYTE)((c&0x7C00)>>7);
				case 1:     c=HAI_MAKEWORD(pSrc);pSrc+=2;*pDst++=(S_BYTE)((c&0x1F)<<3);*pDst++=(S_BYTE)((c&0x3E0)>>2);*pDst++=(S_BYTE)((c&0x7C00)>>7);
						}while (--j);
				}
			}
			break;
		case 8:
			for (S_WORD i = 0; i < IconInfo.m_height; i++)
			{
				S_BYTE *pDst = pImage+i*IconInfo.m_WidthBytes;
				const S_BYTE *pSrc = picoData+(IconInfo.m_height-i-1)*WidthBytes;
				S_WORD c = 0, j = (S_WORD)((bih.biWidth+7)>>3);
				switch (bih.biWidth % 8)
				{
				case 0:do{c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
				case 7:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
				case 6:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
				case 5:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
				case 4:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
				case 3:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
				case 2:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
				case 1:     c=*pSrc++;*pDst++=pPalette[c].rgbRed;*pDst++=pPalette[c].rgbGreen;*pDst++=pPalette[c].rgbBlue;
						}while (--j);
				}
			}
			break;
		case 4:
			for (S_WORD i = 0; i < IconInfo.m_height; i++)
			{
				S_BYTE *pDst = pImage+i*IconInfo.m_WidthBytes;
				const S_BYTE *pSrc = picoData+(IconInfo.m_height-i-1)*WidthBytes;
				S_WORD c = 0, j = (S_WORD)bih.biWidth;
				while (j >= 2){
					j -= 2;
					c = *pSrc++;
					*pDst++ = pPalette[(c>>4)&0X0F].rgbRed;
					*pDst++ = pPalette[(c>>4)&0X0F].rgbGreen;
					*pDst++ = pPalette[(c>>4)&0X0F].rgbBlue;
					*pDst++ = pPalette[c&0X0F].rgbRed;
					*pDst++ = pPalette[c&0X0F].rgbGreen;
					*pDst++ = pPalette[c&0X0F].rgbBlue;
				}
				if (j == 1){
					c = *pSrc++;
					*pDst++ = pPalette[(c>>4)&0X0F].rgbRed;
					*pDst++ = pPalette[(c>>4)&0X0F].rgbGreen;
					*pDst++ = pPalette[(c>>4)&0X0F].rgbBlue;
				}
			}
			break;
		case 1:
			for (S_WORD i = 0; i < IconInfo.m_height; i++)
			{
				S_BYTE *pDst = pImage+i*IconInfo.m_WidthBytes;
				const S_BYTE *pSrc = picoData+(IconInfo.m_height-i-1)*WidthBytes;
				S_WORD c = 0, j = (S_WORD)bih.biWidth;
				while (j >= 8){
					j -= 8;
					c = *pSrc++;
					*pDst++ = pPalette[(c>>7)&0X01].rgbRed;*pDst++ = pPalette[(c>>7)&0X01].rgbGreen;*pDst++ = pPalette[(c>>7)&0X01].rgbBlue;
					*pDst++ = pPalette[(c>>6)&0X01].rgbRed;*pDst++ = pPalette[(c>>6)&0X01].rgbGreen;*pDst++ = pPalette[(c>>6)&0X01].rgbBlue;
					*pDst++ = pPalette[(c>>5)&0X01].rgbRed;*pDst++ = pPalette[(c>>5)&0X01].rgbGreen;*pDst++ = pPalette[(c>>5)&0X01].rgbBlue;
					*pDst++ = pPalette[(c>>4)&0X01].rgbRed;*pDst++ = pPalette[(c>>4)&0X01].rgbGreen;*pDst++ = pPalette[(c>>4)&0X01].rgbBlue;
					*pDst++ = pPalette[(c>>3)&0X01].rgbRed;*pDst++ = pPalette[(c>>3)&0X01].rgbGreen;*pDst++ = pPalette[(c>>3)&0X01].rgbBlue;
					*pDst++ = pPalette[(c>>2)&0X01].rgbRed;*pDst++ = pPalette[(c>>2)&0X01].rgbGreen;*pDst++ = pPalette[(c>>2)&0X01].rgbBlue;
					*pDst++ = pPalette[(c>>1)&0X01].rgbRed;*pDst++ = pPalette[(c>>1)&0X01].rgbGreen;*pDst++ = pPalette[(c>>1)&0X01].rgbBlue;
					*pDst++ = pPalette[(c>>0)&0X01].rgbRed;*pDst++ = pPalette[(c>>0)&0X01].rgbGreen;*pDst++ = pPalette[(c>>0)&0X01].rgbBlue;
				}
				if (j)
				{
					S_SHORT w = (S_SHORT)j;
					c = *pSrc++;
					{*pDst++ = pPalette[(c>>7)&0X01].rgbRed;*pDst++ = pPalette[(c>>7)&0X01].rgbGreen;*pDst++ = pPalette[(c>>7)&0X01].rgbBlue;}
					if (--w > 0)
						{*pDst++ = pPalette[(c>>6)&0X01].rgbRed;*pDst++ = pPalette[(c>>6)&0X01].rgbGreen;*pDst++ = pPalette[(c>>6)&0X01].rgbBlue;}
					if (--w > 0)
						{*pDst++ = pPalette[(c>>5)&0X01].rgbRed;*pDst++ = pPalette[(c>>5)&0X01].rgbGreen;*pDst++ = pPalette[(c>>5)&0X01].rgbBlue;}
					if (--w > 0)
						{*pDst++ = pPalette[(c>>4)&0X01].rgbRed;*pDst++ = pPalette[(c>>4)&0X01].rgbGreen;*pDst++ = pPalette[(c>>4)&0X01].rgbBlue;}
					if (--w > 0)
						{*pDst++ = pPalette[(c>>3)&0X01].rgbRed;*pDst++ = pPalette[(c>>3)&0X01].rgbGreen;*pDst++ = pPalette[(c>>3)&0X01].rgbBlue;}
					if (--w > 0)
						{*pDst++ = pPalette[(c>>2)&0X01].rgbRed;*pDst++ = pPalette[(c>>2)&0X01].rgbGreen;*pDst++ = pPalette[(c>>2)&0X01].rgbBlue;}
					if (--w > 0)
						{*pDst++ = pPalette[(c>>1)&0X01].rgbRed;*pDst++ = pPalette[(c>>1)&0X01].rgbGreen;*pDst++ = pPalette[(c>>1)&0X01].rgbBlue;}
					if (--w > 0)
						{*pDst++ = pPalette[(c>>0)&0X01].rgbRed;*pDst++ = pPalette[(c>>0)&0X01].rgbGreen;*pDst++ = pPalette[(c>>0)&0X01].rgbBlue;}
				}
			}
			break;
		default:
			return NULL;
	}

	// read mask
	S_BYTE *pMask = new S_BYTE[MASKWIDTH(bih.biWidth)*bih.biWidth+4];
	if (pMask == NULL)
		return NULL;

	picoData += WidthBytes*IconInfo.m_height;
	WidthBytes = MASKWIDTH(IconInfo.m_width);
	for (S_WORD i = 0; i < IconInfo.m_height; i++)
	{
		S_BYTE *pDst = pMask+i*WidthBytes;
		const S_BYTE *pSrc = picoData+(IconInfo.m_height-i-1)*WidthBytes;
		memcpy(pDst, pSrc, WidthBytes);
	}

	pImage = new S_BYTE[strImage.size()];
	if (pImage == NULL)
	{
		delete pMask;
		return NULL;
	}
	IconInfo.m_lpMask = pMask;
	memcpy(pImage, strImage.data(), strImage.size());
	return (S_VOID *)pImage;
}


