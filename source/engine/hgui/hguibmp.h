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
	C_HGUIBMP(S_WORD w, S_WORD h, S_WORD bpp, void *pBuff);
	virtual ~C_HGUIBMP();

	void SetBmpInfo(S_WORD w, S_WORD h, S_WORD bpp);
	void SetBmpData(void *pBuff);
	void *GetBmpData(void);

	virtual int SetPixel(S_WORD x, S_WORD y, S_DWORD color)=0;
	virtual S_DWORD GetPixel(S_WORD x, S_WORD y)=0;
	virtual int FlipPixel(S_WORD x, S_WORD y)=0;
	virtual int FlipHLine(S_WORD x, S_WORD y, S_WORD w)=0;
	virtual int FillHLine(S_WORD x, S_WORD y, S_WORD w, S_DWORD color)=0;

protected:
	S_WORD m_Width;
	S_WORD m_Height;
	S_WORD m_BPP;
	void *m_pBuff;
};

class C_HGUISCRN:public C_HGUIBMP
{
public:
	C_HGUISCRN(void);
	virtual ~C_HGUISCRN();

	virtual int SetPixel(S_WORD x, S_WORD y, S_DWORD color);
	virtual S_DWORD GetPixel(S_WORD x, S_WORD y);
	virtual int FlipPixel(S_WORD x, S_WORD y);
	virtual int FlipHLine(S_WORD x, S_WORD y, S_WORD w);
	virtual int FillHLine(S_WORD x, S_WORD y, S_WORD w, S_DWORD color);
};

extern C_HGUISCRN HGUI_LcdScrn;

#endif //__HGUI_BMP_H__

