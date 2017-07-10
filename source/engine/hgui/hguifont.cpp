//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguifont.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "hguidef.h"
#include "hguifont.h"

#define FONT12X12	"font1212.bin"
#define FONT16X16	"font1616.bin"
#define SYM16X16	"sym1616.bin"
#define SYM08X16	"sym0816.bin"

extern char g_HGui_ResourcePath[CFG_PATH_FILE_MAX_LEN];

//
C_HGUIFONT::C_HGUIFONT(void):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_FONT)
{
	m_style = 0;
	m_BkMode = HGUI_BKM_OPAQUE;
	m_TextColor = HGUI_COLOR_BLACK;
	m_BkColor = HGUI_COLOR_WHITE;
}
C_HGUIFONT::~C_HGUIFONT()
{
}

/** @brief Font 12x12
 */
C_HGUIF12x12::C_HGUIF12x12(void)
{
	string strFontFile(g_HGui_ResourcePath);
	strFontFile.append("font//").append(FONT12X12);
	m_fpFont = fopen(strFontFile.c_str(), "rb");
}

C_HGUIF12x12::~C_HGUIF12x12()
{
	if (m_fpFont)
		fclose(m_fpFont);
}

int C_HGUIF12x12::GetWidth(S_WORD font)
{
	if (font < 256)
		return 6;
	return 12;
}

int C_HGUIF12x12::GetHeight(S_WORD font)
{
	return 12;
}

int C_HGUIF12x12::DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP)
{
	S_BYTE font, mod[12];
	memset(mod, 0x00, sizeof(mod));
	GetFontMold(ch, mod);
	
	for (int i = 0; i < 12; ++i)
	{
		font = mod[i];
		DrawPixel(x + 0, y + i, font>>7, pBMP);
		DrawPixel(x + 1, y + i, font>>6, pBMP);
		DrawPixel(x + 2, y + i, font>>5, pBMP);
		DrawPixel(x + 3, y + i, font>>4, pBMP);
		DrawPixel(x + 4, y + i, font>>3, pBMP);
		DrawPixel(x + 5, y + i, font>>2, pBMP);
/*
		pBMP->SetPixel(x + 0, y + i, font>>7);
		pBMP->SetPixel(x + 1, y + i, font>>6);
		pBMP->SetPixel(x + 2, y + i, font>>5);
		pBMP->SetPixel(x + 3, y + i, font>>4);
		pBMP->SetPixel(x + 4, y + i, font>>3);
		pBMP->SetPixel(x + 5, y + i, font>>2);*/
	}

	return 0;
}

int C_HGUIF12x12::DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP)
{
	S_BYTE font, mod[24];
	memset(mod, 0x00, sizeof(mod));
	GetFontMold(ch, mod);
	
	for (int i = 0; i < 12; ++i)
	{
		font = mod[i*2+0];
		DrawPixel(x + 0, y + i, font>>7, pBMP);
		DrawPixel(x + 1, y + i, font>>6, pBMP);
		DrawPixel(x + 2, y + i, font>>5, pBMP);
		DrawPixel(x + 3, y + i, font>>4, pBMP);
		DrawPixel(x + 4, y + i, font>>3, pBMP);
		DrawPixel(x + 5, y + i, font>>2, pBMP);
		DrawPixel(x + 6, y + i, font>>1, pBMP);
		DrawPixel(x + 7, y + i, font>>0, pBMP);
/*
		pBMP->SetPixel(x + 0, y + i, font>>7);
		pBMP->SetPixel(x + 1, y + i, font>>6);
		pBMP->SetPixel(x + 2, y + i, font>>5);
		pBMP->SetPixel(x + 3, y + i, font>>4);
		pBMP->SetPixel(x + 4, y + i, font>>3);
		pBMP->SetPixel(x + 5, y + i, font>>2);
		pBMP->SetPixel(x + 6, y + i, font>>1);
		pBMP->SetPixel(x + 7, y + i, font>>0);*/
		font = mod[i*2+1];
		DrawPixel(x + 8, y + i, font>>7, pBMP);
		DrawPixel(x + 9, y + i, font>>6, pBMP);
		DrawPixel(x + 10, y + i, font>>5, pBMP);
		DrawPixel(x + 11, y + i, font>>4, pBMP);
/*		pBMP->SetPixel(x + 8, y + i, font>>7);
		pBMP->SetPixel(x + 9, y + i, font>>6);
		pBMP->SetPixel(x + 10, y + i, font>>5);
		pBMP->SetPixel(x + 11, y + i, font>>4);*/
	}

	return 0;
}

int C_HGUIF12x12::GetFontMold(S_WORD font, S_BYTE *pFontBuf)
{
	if (m_fpFont == NULL)
		return -1;
	if (font >= 0x20 && font <= 0x7F)
	{
		fseek(m_fpFont, (font-0x20) * 12, SEEK_SET);
		fread(pFontBuf, 12, 1, m_fpFont);

		return 12;
	}
	else if (font >= 0xA1A1)
	{//1152字节为英文
		fseek(m_fpFont, 1152 + (long)((((font>>8)&0xFF)-0xA1)*94+((font&0xFF)-0xA1)) * 24, SEEK_SET);
		fread(pFontBuf, 24, 1, m_fpFont);

		return 24;
	}

	return 0;
}

/** @brief Font 16x16
 */
C_HGUIF16x16::C_HGUIF16x16(void)
{
	string strFontFile(g_HGui_ResourcePath);
	strFontFile.append("font//").append(FONT16X16);
	m_fpFont = fopen(strFontFile.c_str(), "rb");
}

C_HGUIF16x16::~C_HGUIF16x16()
{
	if (m_fpFont)
		fclose(m_fpFont);
}

int C_HGUIF16x16::GetWidth(S_WORD font)
{
	if (font < 256)
		return 8;
	return 16;
}

int C_HGUIF16x16::GetHeight(S_WORD font)
{
	return 16;
}

int C_HGUIF16x16::DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP)
{
	S_BYTE font, mod[16];
	memset(mod, 0x00, sizeof(mod));
	GetFontMold(ch, mod);
	
	for (int i = 0; i < 16; ++i)
	{
		font = mod[i];
		DrawPixel(x + 0, y + i, font>>7, pBMP);
		DrawPixel(x + 1, y + i, font>>6, pBMP);
		DrawPixel(x + 2, y + i, font>>5, pBMP);
		DrawPixel(x + 3, y + i, font>>4, pBMP);
		DrawPixel(x + 4, y + i, font>>3, pBMP);
		DrawPixel(x + 5, y + i, font>>2, pBMP);
		DrawPixel(x + 6, y + i, font>>1, pBMP);
		DrawPixel(x + 7, y + i, font>>0, pBMP);

/*		pBMP->SetPixel(x + 0, y + i, font>>7);
		pBMP->SetPixel(x + 1, y + i, font>>6);
		pBMP->SetPixel(x + 2, y + i, font>>5);
		pBMP->SetPixel(x + 3, y + i, font>>4);
		pBMP->SetPixel(x + 4, y + i, font>>3);
		pBMP->SetPixel(x + 5, y + i, font>>2);
		pBMP->SetPixel(x + 6, y + i, font>>1);
		pBMP->SetPixel(x + 7, y + i, font>>0);*/
	}

	return 0;
}

int C_HGUIF16x16::DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP)
{
	S_BYTE font, mod[32];
	memset(mod, 0x00, sizeof(mod));
	GetFontMold(ch, mod);
	
	for (int i = 0; i < 16; ++i)
	{
		font = mod[i*2+0];
		DrawPixel(x + 0, y + i, font>>7, pBMP);
		DrawPixel(x + 1, y + i, font>>6, pBMP);
		DrawPixel(x + 2, y + i, font>>5, pBMP);
		DrawPixel(x + 3, y + i, font>>4, pBMP);
		DrawPixel(x + 4, y + i, font>>3, pBMP);
		DrawPixel(x + 5, y + i, font>>2, pBMP);
		DrawPixel(x + 6, y + i, font>>1, pBMP);
		DrawPixel(x + 7, y + i, font>>0, pBMP);
/*		pBMP->SetPixel(x + 0, y + i, font>>7);
		pBMP->SetPixel(x + 1, y + i, font>>6);
		pBMP->SetPixel(x + 2, y + i, font>>5);
		pBMP->SetPixel(x + 3, y + i, font>>4);
		pBMP->SetPixel(x + 4, y + i, font>>3);
		pBMP->SetPixel(x + 5, y + i, font>>2);
		pBMP->SetPixel(x + 6, y + i, font>>1);
		pBMP->SetPixel(x + 7, y + i, font>>0);*/
		font = mod[i*2+1];
		DrawPixel(x + 8, y + i, font>>7, pBMP);
		DrawPixel(x + 9, y + i, font>>6, pBMP);
		DrawPixel(x + 10, y + i, font>>5, pBMP);
		DrawPixel(x + 11, y + i, font>>4, pBMP);
		DrawPixel(x + 12, y + i, font>>3, pBMP);
		DrawPixel(x + 13, y + i, font>>2, pBMP);
		DrawPixel(x + 14, y + i, font>>1, pBMP);
		DrawPixel(x + 15, y + i, font>>0, pBMP);
/*		pBMP->SetPixel(x + 8, y + i, font>>7);
		pBMP->SetPixel(x + 9, y + i, font>>6);
		pBMP->SetPixel(x + 10, y + i, font>>5);
		pBMP->SetPixel(x + 11, y + i, font>>4);
		pBMP->SetPixel(x + 12, y + i, font>>3);
		pBMP->SetPixel(x + 13, y + i, font>>2);
		pBMP->SetPixel(x + 14, y + i, font>>1);
		pBMP->SetPixel(x + 15, y + i, font>>0);*/
	}

	return 0;
}

int C_HGUIF16x16::GetFontMold(S_WORD font, S_BYTE *pFontBuf)
{
	if (m_fpFont == NULL)
		return -1;
	if (font >= 0x20 && font <= 0x7F)
	{
		fseek(m_fpFont, (font-0x20) * 16, SEEK_SET);
		fread(pFontBuf, 16, 1, m_fpFont);

		return 16;
	}
	else if (font >= 0xA1A1)
	{//1536字节为英文
		fseek(m_fpFont, 1536 + (long)((((font>>8)&0xFF)-0xA1)*94+((font&0xFF)-0xA1)) * 32, SEEK_SET);
		fread(pFontBuf, 32, 1, m_fpFont);

		return 32;
	}

	return 0;
}

/** @brief ENGLISH 5x8
 *  @note 本文件为5×8点阵西文字库文件,字的横向8点构成一字节,左边点在字节的高位,
 *  字符点阵四角按左上角→右上角→左下角→右下角取字,
 */
const S_BYTE HGUI_Eng5x8Mold[] = 
{
/*    CHAR_20 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*!   CHAR_21 */
0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00,
/*"   CHAR_22 */
0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00,
/*#   CHAR_23 */
0x50,0x50,0xF8,0x50,0xF8,0x50,0x50,0x00,
/*$   CHAR_24 */
0x20,0x78,0xC0,0x70,0x28,0xF0,0x20,0x00,
/*%   CHAR_25 */
0xC0,0xC8,0x10,0x20,0x40,0x98,0x18,0x00,
/*&   CHAR_26 */
0x40,0xA0,0xA0,0x40,0xA8,0x90,0x68,0x00,
/*'   CHAR_27 */
0x30,0x20,0x40,0x00,0x00,0x00,0x00,0x00,
/*(   CHAR_28 */
0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00,
/*)   CHAR_29 */
0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00,
/**   CHAR_2A */
0x20,0xA8,0x70,0x20,0x70,0xA8,0x20,0x00,
/*+   CHAR_2B */
0x20,0x20,0x20,0xF8,0x20,0x20,0x20,0x00,
/*,   CHAR_2C */
0x00,0x00,0x00,0x00,0x60,0x40,0x80,0x00,
/*-   CHAR_2D */
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,
/*.   CHAR_2E */
0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,
/*/   CHAR_2F */
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,
/*0   CHAR_30 */
0x70,0x88,0x98,0xA8,0xC8,0x88,0x70,0x00,
/*1   CHAR_31 */
0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00,
/*2   CHAR_32 */
0x70,0x88,0x08,0x30,0x40,0x80,0xF8,0x00,
/*3   CHAR_33 */
0xF8,0x08,0x10,0x30,0x08,0x88,0x70,0x00,
/*4   CHAR_34 */
0x10,0x30,0x50,0x90,0xF8,0x10,0x10,0x00,
/*5   CHAR_35 */
0xF8,0x80,0xF0,0x08,0x08,0x88,0x70,0x00,
/*6   CHAR_36 */
0x38,0x40,0x80,0xF0,0x88,0x88,0x70,0x00,
/*7   CHAR_37 */
0xF8,0x08,0x10,0x20,0x40,0x40,0x40,0x00,
/*8   CHAR_38 */
0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00,
/*9   CHAR_39 */
0x70,0x88,0x88,0x78,0x08,0x10,0xE0,0x00,
/*:   CHAR_3A */
0x00,0x60,0x60,0x00,0x60,0x60,0x00,0x00,
/*;   CHAR_3B */
0x00,0x60,0x60,0x00,0x60,0x60,0xC0,0x00,
/*<   CHAR_3C */
0x10,0x20,0x40,0x80,0x40,0x20,0x10,0x00,
/*=   CHAR_3D */
0x00,0x00,0xF8,0x00,0xF8,0x00,0x00,0x00,
/*>   CHAR_3E */
0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00,
/*?   CHAR_3F */
0x70,0x88,0x10,0x20,0x20,0x00,0x20,0x00,
/*@   CHAR_40 */
0x70,0x88,0xB8,0xA8,0xB8,0x80,0x78,0x00,
/*A   CHAR_41 */
0x20,0x50,0x88,0x88,0xF8,0x88,0x88,0x00,
/*B   CHAR_42 */
0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0,0x00,
/*C   CHAR_43 */
0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00,
/*D   CHAR_44 */
0xF0,0x88,0x88,0x88,0x88,0x88,0xF0,0x00,
/*E   CHAR_45 */
0xF8,0x80,0x80,0xF0,0x80,0x80,0xF8,0x00,
/*F   CHAR_46 */
0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0x00,
/*G   CHAR_47 */
0x70,0x88,0x80,0x80,0xB8,0x88,0x78,0x00,
/*H   CHAR_48 */
0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,
/*I   CHAR_49 */
0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00,
/*J   CHAR_4A */
0x38,0x10,0x10,0x10,0x10,0x90,0x60,0x00,
/*K   CHAR_4B */
0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88,0x00,
/*L   CHAR_4C */
0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00,
/*M   CHAR_4D */
0x88,0xD8,0xA8,0xA8,0x88,0x88,0x88,0x00,
/*N   CHAR_4E */
0x88,0x88,0xC8,0xA8,0x98,0x88,0x88,0x00,
/*O   CHAR_4F */
0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00,
/*P   CHAR_50 */
0xF0,0x88,0x88,0xF0,0x80,0x80,0x80,0x00,
/*Q   CHAR_51 */
0x70,0x88,0x88,0x88,0xA8,0x90,0x68,0x00,
/*R   CHAR_52 */
0xF0,0x88,0x88,0xF0,0xA0,0x90,0x88,0x00,
/*S   CHAR_53 */
0x70,0x88,0x80,0x70,0x08,0x88,0x70,0x00,
/*T   CHAR_54 */
0xF8,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
/*U   CHAR_55 */
0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,
/*V   CHAR_56 */
0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00,
/*W   CHAR_57 */
0x88,0x88,0x88,0xA8,0xA8,0xD8,0x88,0x00,
/*X   CHAR_58 */
0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00,
/*Y   CHAR_59 */
0x88,0x88,0x50,0x20,0x20,0x20,0x20,0x00,
/*Z   CHAR_5A */
0xF8,0x08,0x10,0x20,0x40,0x80,0xF8,0x00,
/*[   CHAR_5B */
0xF0,0xC0,0xC0,0xC0,0xC0,0xC0,0xF0,0x00,
/*\   CHAR_5C */
0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,
/*]   CHAR_5D */
0x78,0x18,0x18,0x18,0x18,0x18,0x78,0x00,
/*^   CHAR_5E */
0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00,
/*_   CHAR_5F */
0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,
/*`   CHAR_60 */
0x20,0x10,0x08,0x00,0x00,0x00,0x00,0x00,
/*a   CHAR_61 */
0x00,0x00,0xE0,0x10,0x70,0x90,0x68,0x00,
/*b   CHAR_62 */
0x80,0x80,0xB0,0xC8,0x88,0xC8,0xB0,0x00,
/*c   CHAR_63 */
0x00,0x00,0x70,0x88,0x80,0x80,0x70,0x00,
/*d   CHAR_64 */
0x08,0x08,0x68,0x98,0x88,0x98,0x68,0x00,
/*e   CHAR_65 */
0x00,0x00,0x70,0x88,0xF0,0x80,0x70,0x00,
/*f   CHAR_66 */
0x30,0x48,0x40,0xF0,0x40,0x40,0x40,0x00,
/*g   CHAR_67 */
0x00,0x00,0x70,0x88,0x88,0x78,0x08,0xF0,
/*h   CHAR_68 */
0x80,0x80,0xB0,0xC8,0x88,0x88,0x88,0x00,
/*i   CHAR_69 */
0x20,0x00,0x00,0x20,0x20,0x20,0x20,0x00,
/*j   CHAR_6A */
0x10,0x00,0x00,0x30,0x10,0x10,0x10,0x60,
/*k   CHAR_6B */
0x80,0x80,0x90,0xA0,0xC0,0xA0,0x98,0x00,
/*l   CHAR_6C */
0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00,
/*m   CHAR_6D */
0x00,0x00,0x50,0xA8,0xA8,0xA8,0xA8,0x00,
/*n   CHAR_6E */
0x00,0x00,0xB0,0x48,0x48,0x48,0x48,0x00,
/*o   CHAR_6F */
0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00,
/*p   CHAR_70 */
0x00,0x00,0xF0,0x88,0x88,0xF0,0x80,0x80,
/*q   CHAR_71 */
0x00,0x00,0x78,0x88,0x88,0x78,0x08,0x08,
/*r   CHAR_72 */
0x00,0x00,0xB0,0x48,0x40,0x40,0x40,0x00,
/*s   CHAR_73 */
0x00,0x00,0x78,0x80,0x70,0x08,0xF0,0x00,
/*t   CHAR_74 */
0x40,0x40,0xF8,0x40,0x40,0x48,0x30,0x00,
/*u   CHAR_75 */
0x00,0x00,0x90,0x90,0x90,0x90,0x68,0x00,
/*v   CHAR_76 */
0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00,
/*w   CHAR_77 */
0x00,0x00,0xA8,0xA8,0xA8,0xA8,0x50,0x00,
/*x   CHAR_78 */
0x00,0x00,0x88,0x50,0x20,0x50,0x88,0x00,
/*y   CHAR_79 */
0x00,0x00,0x88,0x88,0x98,0x68,0x08,0xF0,
/*z   CHAR_7A */
0x00,0x00,0xF8,0x10,0x20,0x40,0xF8,0x00,
/*{   CHAR_7B */
0x20,0x40,0x40,0x80,0x40,0x40,0x20,0x00,
/*|   CHAR_7C */
0x20,0x20,0x20,0x00,0x20,0x20,0x20,0x00,
/*}   CHAR_7D */
0x20,0x10,0x10,0x08,0x10,0x10,0x20,0x00,
/*~   CHAR_7E */
0x00,0x00,0x40,0xA8,0x10,0x00,0x00,0x00,
/*   CHAR_7F */
0xA8,0x50,0xA8,0x50,0xA8,0x50,0xA8,0x00
};

C_HGUIE5x8::C_HGUIE5x8(void)
{
}

C_HGUIE5x8::~C_HGUIE5x8()
{
}

int C_HGUIE5x8::GetWidth(S_WORD font)
{
	if (font < 256)
		return 6;
	return 10;
}

int C_HGUIE5x8::GetHeight(S_WORD font)
{
	return 8;
}

int C_HGUIE5x8::DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP)
{
	S_BYTE font, mod[8];
	memset(mod, 0x00, sizeof(mod));
	GetFontMold(ch, mod);
	
	for (int i = 0; i < 8; ++i)
	{
		font = mod[i];
		DrawPixel(x + 0, y + i, font>>7, pBMP);
		DrawPixel(x + 1, y + i, font>>6, pBMP);
		DrawPixel(x + 2, y + i, font>>5, pBMP);
		DrawPixel(x + 3, y + i, font>>4, pBMP);
		DrawPixel(x + 4, y + i, font>>3, pBMP);
/*		DrawPixel(x + 5, y + i, font>>2, pBMP);
		DrawPixel(x + 6, y + i, font>>1, pBMP);
		DrawPixel(x + 7, y + i, font>>0, pBMP);
		pBMP->SetPixel(x + 0, y + i, font>>7);
		pBMP->SetPixel(x + 1, y + i, font>>6);
		pBMP->SetPixel(x + 2, y + i, font>>5);
		pBMP->SetPixel(x + 3, y + i, font>>4);
		pBMP->SetPixel(x + 4, y + i, font>>3);*/
	}

	return 0;
}

int C_HGUIE5x8::DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP)
{
	DrawChar(x+0, y, '?', pBMP);
	DrawChar(x+5, y, '?', pBMP);
	return 0;
}

int C_HGUIE5x8::GetFontMold(S_WORD font, S_BYTE *pFontBuf)
{
	if (font >= 0x20 && font <= 0x7F)
	{
		memcpy(pFontBuf, HGUI_Eng5x8Mold + (font-0x20) * 8, 8);
		return 8;
	}
	return 0;
}

/** @brief symbol 16x16
 */
C_HGUISYM16x16::C_HGUISYM16x16(void)
{
	string strFontFile(g_HGui_ResourcePath);
	strFontFile.append("font//").append(SYM16X16);
	m_fpFont = fopen(strFontFile.c_str(), "rb");
}

C_HGUISYM16x16::~C_HGUISYM16x16()
{
	if (m_fpFont)
		fclose(m_fpFont);
}

int C_HGUISYM16x16::GetWidth(S_WORD font)
{
	return 16;
}

int C_HGUISYM16x16::GetHeight(S_WORD font)
{
	return 16;
}

int C_HGUISYM16x16::DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP)
{
	S_BYTE font, mod[32];
	memset(mod, 0x00, sizeof(mod));
	GetFontMold(ch, mod);
	
	for (int i = 0; i < 16; ++i)
	{
		font = mod[i*2+0];
		DrawPixel(x + 0, y + i, font>>7, pBMP);
		DrawPixel(x + 1, y + i, font>>6, pBMP);
		DrawPixel(x + 2, y + i, font>>5, pBMP);
		DrawPixel(x + 3, y + i, font>>4, pBMP);
		DrawPixel(x + 4, y + i, font>>3, pBMP);
		DrawPixel(x + 5, y + i, font>>2, pBMP);
		DrawPixel(x + 6, y + i, font>>1, pBMP);
		DrawPixel(x + 7, y + i, font>>0, pBMP);
/*		pBMP->SetPixel(x + 0, y + i, font>>7);
		pBMP->SetPixel(x + 1, y + i, font>>6);
		pBMP->SetPixel(x + 2, y + i, font>>5);
		pBMP->SetPixel(x + 3, y + i, font>>4);
		pBMP->SetPixel(x + 4, y + i, font>>3);
		pBMP->SetPixel(x + 5, y + i, font>>2);
		pBMP->SetPixel(x + 6, y + i, font>>1);
		pBMP->SetPixel(x + 7, y + i, font>>0);*/
		font = mod[i*2+1];
		DrawPixel(x + 8, y + i, font>>7, pBMP);
		DrawPixel(x + 9, y + i, font>>6, pBMP);
		DrawPixel(x + 10, y + i, font>>5, pBMP);
		DrawPixel(x + 11, y + i, font>>4, pBMP);
		DrawPixel(x + 12, y + i, font>>3, pBMP);
		DrawPixel(x + 13, y + i, font>>2, pBMP);
		DrawPixel(x + 14, y + i, font>>1, pBMP);
		DrawPixel(x + 15, y + i, font>>0, pBMP);
/*		pBMP->SetPixel(x + 8, y + i, font>>7);
		pBMP->SetPixel(x + 9, y + i, font>>6);
		pBMP->SetPixel(x + 10, y + i, font>>5);
		pBMP->SetPixel(x + 11, y + i, font>>4);
		pBMP->SetPixel(x + 12, y + i, font>>3);
		pBMP->SetPixel(x + 13, y + i, font>>2);
		pBMP->SetPixel(x + 14, y + i, font>>1);
		pBMP->SetPixel(x + 15, y + i, font>>0);*/
	}

	return 0;
}

int C_HGUISYM16x16::DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP)
{
	return DrawChar(x, y, (char)ch, pBMP);
}

int C_HGUISYM16x16::GetFontMold(S_WORD font, S_BYTE *pFontBuf)
{
	if (m_fpFont == NULL)
		return -1;
	if (font >= 0x20 && font < 0x7F)
	{
		fseek(m_fpFont, (font-0x20) * 32, SEEK_SET);
		fread(pFontBuf, 32, 1, m_fpFont);

		return 32;
	}
	return 0;
}

/** @brief symbol 08x16
 */
C_HGUISYM08x16::C_HGUISYM08x16(void)
{
	string strFontFile(g_HGui_ResourcePath);
	strFontFile.append("font//").append(SYM08X16);
	m_fpFont = fopen(strFontFile.c_str(), "rb");
}

C_HGUISYM08x16::~C_HGUISYM08x16()
{
	if (m_fpFont)
		fclose(m_fpFont);
}

int C_HGUISYM08x16::GetWidth(S_WORD font)
{
	return 8;
}

int C_HGUISYM08x16::GetHeight(S_WORD font)
{
	return 16;
}

int C_HGUISYM08x16::DrawChar(S_WORD x, S_WORD y, char ch, C_HGUIBMP *pBMP)
{
	S_BYTE font, mod[16];
	memset(mod, 0x00, sizeof(mod));
	GetFontMold(ch, mod);
	
	for (int i = 0; i < 16; ++i)
	{
		font = mod[i];
		DrawPixel(x + 0, y + i, font>>7, pBMP);
		DrawPixel(x + 1, y + i, font>>6, pBMP);
		DrawPixel(x + 2, y + i, font>>5, pBMP);
		DrawPixel(x + 3, y + i, font>>4, pBMP);
		DrawPixel(x + 4, y + i, font>>3, pBMP);
		DrawPixel(x + 5, y + i, font>>2, pBMP);
		DrawPixel(x + 6, y + i, font>>1, pBMP);
		DrawPixel(x + 7, y + i, font>>0, pBMP);
/*		pBMP->SetPixel(x + 0, y + i, font>>7);
		pBMP->SetPixel(x + 1, y + i, font>>6);
		pBMP->SetPixel(x + 2, y + i, font>>5);
		pBMP->SetPixel(x + 3, y + i, font>>4);
		pBMP->SetPixel(x + 4, y + i, font>>3);
		pBMP->SetPixel(x + 5, y + i, font>>2);
		pBMP->SetPixel(x + 6, y + i, font>>1);
		pBMP->SetPixel(x + 7, y + i, font>>0);*/
	}

	return 0;
}

int C_HGUISYM08x16::DrawFont(S_WORD x, S_WORD y, S_WORD ch, C_HGUIBMP *pBMP)
{
	return DrawChar(x, y, (char)ch, pBMP);
}

int C_HGUISYM08x16::GetFontMold(S_WORD font, S_BYTE *pFontBuf)
{
	if (m_fpFont == NULL)
		return -1;
	if (font >= 0x20 && font < 0x7F)
	{
		fseek(m_fpFont, (font-0x20) * 16, SEEK_SET);
		fread(pFontBuf, 16, 1, m_fpFont);

		return 16;
	}
	return 0;
}



