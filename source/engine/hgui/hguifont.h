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
	C_HGUIFONT(void);
	virtual ~C_HGUIFONT();

public:
	S_BYTE SetBkMode(S_BYTE BkMode){S_BYTE old = m_BkMode;m_BkMode=BkMode;return old;}
	S_DWORD SetBkColor(S_DWORD BkColor){S_DWORD old = m_BkColor;m_BkColor=BkColor;return old;}
	S_DWORD SetTextColor(S_DWORD TextColor){S_DWORD old = m_TextColor;m_TextColor=TextColor;return old;}
	virtual int GetWidth(S_WORD font) = 0;
	virtual int GetHeight(S_WORD font) = 0;
	virtual int DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP) = 0;
	virtual int DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP) = 0;

	inline void DrawPixel(S_WORD x, S_WORD y, S_BYTE FontBit, C_HGUIBMP *pBMP){
		if (FontBit&0x01) pBMP->SetPixel(x, y, m_TextColor);
		else if (m_BkMode == HGUI_BKM_OPAQUE) pBMP->SetPixel(x, y, m_BkColor);
	}

protected:
	int m_style;
	S_BYTE m_BkMode;
	S_DWORD m_TextColor;
	S_DWORD m_BkColor;
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

class C_HGUISYM12x12:public C_HGUIFONT
{
public:
	C_HGUISYM12x12(void);
	virtual ~C_HGUISYM12x12();

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

