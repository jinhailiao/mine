//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguibase.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_BASE_H__
#define __HGUI_BASE_H__

#include "hguievt.h"

//
//
//
/** @brief 设置HGUI 驱动 */
int HGui_SetDriver(int hguiDriverID);
/** @brief 资源路径初始化*/
char *HGui_ResourcePathInit(const char *pResourcePath);

/** @brief LCD初始化*/
int HGui_LcdInit(void);
/** @brief Lcd参数 */
char *HGui_LcdBuffer(void);
unsigned short HGui_LcdWidth(void);
unsigned short HGui_LcdHeight(void);
unsigned short HGui_LcdBpp(void);
unsigned long HGui_LcdLine(void);
unsigned long HGui_LcdSize(void);

/** @brief 屏幕刷新*/
void HGui_flushScreen(S_WORD x, S_WORD y, S_WORD w, S_WORD h);
/** @brief 屏幕刷新使能*/
bool HGui_FlushScreenEn(bool enable);
/** @brief 关闭屏幕*/
void HGui_CloseScreen(void);
/** @brief 打开屏幕*/
void HGui_OpenScreen(void);
/** @brief 背光灯开关*/
void HGui_backlight(int on);
/** @brief 设置对比度
 *  @param[in] contrast 0-99
 */
void HGui_contrast(int contrast);
/** @brief 设置背光灯亮度
 *  @param[in] brightness 0-99
 */
void HGui_brightness(int brightness);
/** @brief 设置灰度
 *  @param[in] grayness 0-99
 */
void HGui_grayness(int grayness);
/** @brief 设置屏幕方向
 *  @param[in] direction 0-3
 */
void HGui_direction(int direction);

/** @brief 键盘初始化*/
int HGui_KeyboardInit(void);
void HGui_KeyISR(unsigned short key);
S_WORD HGui_PollKey(S_VOID);

/** @brief 触摸屏初始化 */
int HGui_TouchscrnInit(void);

/** @brief 查询消息*/
S_GUIEVT HGui_PollEvt(void);


#endif //__HGUI_BASE_H__



