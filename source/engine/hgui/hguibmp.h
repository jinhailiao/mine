//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguibmp.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-19  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_BMP_H__
#define __HGUI_BMP_H__
#include "haibase.h"
#include "hguiobj.h"

class C_HGUIBMP:public C_HGUIOBJ
{
public:
	C_HGUIBMP(void);
	C_HGUIBMP(S_WORD w, S_WORD h, S_WORD bpp);
	virtual ~C_HGUIBMP();

	void SetBmpInfo(S_WORD w, S_WORD h, S_WORD bpp);
	void *SetBmpData(void *pBuffer);
	S_WORD GetWidth(void);
	S_WORD GetHeight(void);
	void *GetBmpData(void);
	bool DeleteObject(void);

	virtual int SetPixel(S_WORD x, S_WORD y, S_DWORD color);
	virtual S_DWORD GetPixel(S_WORD x, S_WORD y);
	virtual int FlipPixel(S_WORD x, S_WORD y);
	virtual int FlipHLine(S_WORD x, S_WORD y, S_WORD w);
	virtual int FillHLine(S_WORD x, S_WORD y, S_WORD w, S_DWORD color);

	bool LoadBitmap(const string &strPath);

protected:
	void *SetBmpDataWithAutorelease(void *pBuffer);
	int SetPixel_1(S_WORD x, S_WORD y, S_DWORD color);
	S_DWORD GetPixel_1(S_WORD x, S_WORD y);
	int FlipPixel_1(S_WORD x, S_WORD y);
	int FlipHLine_1(S_WORD x, S_WORD y, S_WORD w);
	int FillHLine_1(S_WORD x, S_WORD y, S_WORD w, S_DWORD color);

protected:
	S_WORD m_Width;
	S_WORD m_Height;
	S_WORD m_BPP;
	C_HGUIBUFFER m_HGuiBuffer;
};

class C_HGUISCRN:public C_HGUIBMP
{
public:
	static int InitScreenBitmap(void);

public:
	C_HGUISCRN(void);
	virtual ~C_HGUISCRN();

	bool BufferNotRelease(void);

	virtual int SetPixel(S_WORD x, S_WORD y, S_DWORD color);
	virtual S_DWORD GetPixel(S_WORD x, S_WORD y);
	virtual int FlipPixel(S_WORD x, S_WORD y);
	virtual int FlipHLine(S_WORD x, S_WORD y, S_WORD w);
	virtual int FillHLine(S_WORD x, S_WORD y, S_WORD w, S_DWORD color);
};

extern C_HGUISCRN HGUI_LcdScrn;

#endif //__HGUI_BMP_H__

