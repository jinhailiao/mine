//---------------------------------------------------------------------------------
// Copyright (c) Haisoft 2006-9-1
// Copyright jinhailiao 2008-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			HGuidef.h
// Description:		
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2006-09-01	Create					Kingsea
// 2.0.0		2006-09-06  update                  Kingsea
// 3.0.0		2008-03-01	update					Kingsea
//---------------------------------------------------------------------------------
#ifndef  __HGUI_DEFINE_H__
#define  __HGUI_DEFINE_H__
#include "hguicfg.h"

#ifdef HGUI_16BITS_COLOR_REVERSION
#define  HGUI_COLOR_GRAY00	0xF
#define  HGUI_COLOR_GRAY01	0xE
#define  HGUI_COLOR_GRAY02	0xD
#define  HGUI_COLOR_GRAY03	0xC
#define  HGUI_COLOR_GRAY04	0xB
#define  HGUI_COLOR_GRAY05	0xA
#define  HGUI_COLOR_GRAY06	0x9
#define  HGUI_COLOR_GRAY07	0x8
#define  HGUI_COLOR_GRAY08	0x7
#define  HGUI_COLOR_GRAY09	0x6
#define  HGUI_COLOR_GRAY10	0x5
#define  HGUI_COLOR_GRAY11	0x4
#define  HGUI_COLOR_GRAY12	0x3
#define  HGUI_COLOR_GRAY13	0x2
#define  HGUI_COLOR_GRAY14	0x1
#define  HGUI_COLOR_GRAY15	0x0
#else
#define  HGUI_COLOR_GRAY00	0x0
#define  HGUI_COLOR_GRAY01	0x1
#define  HGUI_COLOR_GRAY02	0x2
#define  HGUI_COLOR_GRAY03	0x3
#define  HGUI_COLOR_GRAY04	0x4
#define  HGUI_COLOR_GRAY05	0x5
#define  HGUI_COLOR_GRAY06	0x6
#define  HGUI_COLOR_GRAY07	0x7
#define  HGUI_COLOR_GRAY08	0x8
#define  HGUI_COLOR_GRAY09	0x9
#define  HGUI_COLOR_GRAY10	0xA
#define  HGUI_COLOR_GRAY11	0xB
#define  HGUI_COLOR_GRAY12	0xC
#define  HGUI_COLOR_GRAY13	0xD
#define  HGUI_COLOR_GRAY14	0xE
#define  HGUI_COLOR_GRAY15	0xF
#endif

#if (HGUI_LCD_BITSPERPIX == 4)

#define  HGUI_COLOR_WHITE		HGUI_COLOR_GRAY15
#define  HGUI_COLOR_LIGHT		HGUI_COLOR_GRAY12
#define  HGUI_COLOR_LIGRAY		HGUI_COLOR_GRAY10
#define  HGUI_COLOR_GRAY		HGUI_COLOR_GRAY08
#define  HGUI_COLOR_LIDARK		HGUI_COLOR_GRAY06
#define  HGUI_COLOR_DARK		HGUI_COLOR_GRAY03
#define  HGUI_COLOR_BLACK		HGUI_COLOR_GRAY00

#elif (HGUI_LCD_BITSPERPIX == 8)

#define  HGUI_COLOR_WHITE	0xFF
#define  HGUI_COLOR_LIGHT	0xD2
#define  HGUI_COLOR_LIGRAY	0xA8
#define  HGUI_COLOR_GRAY		0x7E
#define  HGUI_COLOR_LIDARK	0x54
#define  HGUI_COLOR_DARK		0x2A
#define  HGUI_COLOR_BLACK	0x00

#elif (HGUI_LCD_BITSPERPIX == 16)

#define  HGUI_COLOR_WHITE	0xFFFF
#define  HGUI_COLOR_LIGHT	0xCE59
#define  HGUI_COLOR_LIGRAY	0xA514
#define  HGUI_COLOR_GRAY	0x7BCF
#define  HGUI_COLOR_LIDARK	0x528A
#define  HGUI_COLOR_DARK	0x2945
#define  HGUI_COLOR_BLACK	0x0000

#elif (HGUI_LCD_BITSPERPIX == 24)

#define  HGUI_COLOR_WHITE	0xFFFFFF
#define  HGUI_COLOR_LIGHT	0xD2D2D2
#define  HGUI_COLOR_LIGRAY	0xA8A8A8
#define  HGUI_COLOR_GRAY		0x7E7E7E
#define  HGUI_COLOR_LIDARK	0x545454
#define  HGUI_COLOR_DARK		0x2A2A2A
#define  HGUI_COLOR_BLACK	0x000000

#else
#error Unsupport color depth!
#endif

#define  HGUI_BKM_OPAQUE			0
#define  HGUI_BKM_TRANSPARENT	1

#define BITMAP_WIDTHBYTES(w, bits) (((w*bits+31)>>5)<<2)

#endif // __HGUI_DEFINE_H__

