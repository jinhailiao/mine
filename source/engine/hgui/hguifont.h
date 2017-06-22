//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguifont.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-19  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_FONT_H__
#define __HGUI_FONT_H__
#include "haibase.h"
#include "hguiobj.h"
#include "hguibmp.h"

class C_HGUIFONT:public C_HGUIOBJ
{
public:
	C_HGUIFONT(void):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_FONT){m_style = 0; }
	virtual ~C_HGUIFONT(){}

public:
	virtual int GetWidth(S_WORD font) = 0;
	virtual int GetHeight(S_WORD font) = 0;
	virtual int DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP) = 0;
	virtual int DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP) = 0;

protected:
	int m_style;
};

class C_HGUIF12x12:public C_HGUIFONT
{
public:
	C_HGUIF12x12(void);
	virtual ~C_HGUIF12x12();

public:
	virtual int GetWidth(S_WORD font);
	virtual int GetHeight(S_WORD font);
	virtual int DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP);
	virtual int DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP);

private:
	int GetFontMold(S_WORD font, S_BYTE *pFontBuf);

private:
	FILE *m_fpFont;
};

class C_HGUIF16x16:public C_HGUIFONT
{
public:
	C_HGUIF16x16(void);
	virtual ~C_HGUIF16x16();

public:
	virtual int GetWidth(S_WORD font);
	virtual int GetHeight(S_WORD font);
	virtual int DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP);
	virtual int DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP);

private:
	int GetFontMold(S_WORD font, S_BYTE *pFontBuf);

private:
	FILE *m_fpFont;
};

class C_HGUIE5x8:public C_HGUIFONT
{
public:
	C_HGUIE5x8(void);
	virtual ~C_HGUIE5x8();

public:
	virtual int GetWidth(S_WORD font);
	virtual int GetHeight(S_WORD font);
	virtual int DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP);
	virtual int DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP);

private:
	int GetFontMold(S_WORD font, S_BYTE *pFontBuf);
};

class C_HGUISYM16x16:public C_HGUIFONT
{
public:
	C_HGUISYM16x16(void);
	virtual ~C_HGUISYM16x16();

public:
	virtual int GetWidth(S_WORD font);
	virtual int GetHeight(S_WORD font);
	virtual int DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP);
	virtual int DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP);

private:
	int GetFontMold(S_WORD font, S_BYTE *pFontBuf);

private:
	FILE *m_fpFont;
};

class C_HGUISYM08x16:public C_HGUIFONT
{
public:
	C_HGUISYM08x16(void);
	virtual ~C_HGUISYM08x16();

public:
	virtual int GetWidth(S_WORD font);
	virtual int GetHeight(S_WORD font);
	virtual int DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP);
	virtual int DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP);

private:
	int GetFontMold(S_WORD font, S_BYTE *pFontBuf);

private:
	FILE *m_fpFont;
};

#endif //__HGUI_FONT_H__

