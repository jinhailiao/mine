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
#ifndef  __HGUI_CONFIG_H__
#define  __HGUI_CONFIG_H__

/**@note LCD显示行的字节数必需是4的倍数，
 * 否则模拟器会显示有问题，目前原因未知
 */
 
//#define HGUI_DEMO_APP
#define MINE_ENGINE_APP
//#define ELS_ENGINE_APP

#if defined(HGUI_DEMO_APP)

#define HGUI_LCD_BITSPERPIX		24
#define HGUI_LCD_WIDTH		480
#define HGUI_LCD_HEIGHT		272

#elif defined(MINE_ENGINE_APP)

#define HGUI_LCD_BITSPERPIX		24
#define HGUI_LCD_WIDTH		384
#define HGUI_LCD_HEIGHT		448

#elif defined(ELS_ENGINE_APP)

#define HGUI_LCD_BITSPERPIX		24
#define HGUI_LCD_WIDTH		480
#define HGUI_LCD_HEIGHT		272

#else

#error "please define APP_MACRO!"

#endif


#endif // __HGUI_CONFIG_H__


