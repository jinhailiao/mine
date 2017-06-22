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
/** @brief ����HGUI ���� */
int HGui_SetDriver(int hguiDriverID);
/** @brief ��Դ·����ʼ��*/
char *HGui_ResourcePathInit(const char *pResourcePath);

/** @brief LCD��ʼ��*/
int HGui_LcdInit(void);
/** @brief Lcd���� */
char *HGui_LcdBuffer(void);
unsigned short HGui_LcdWidth(void);
unsigned short HGui_LcdHeight(void);
unsigned short HGui_LcdBpp(void);
unsigned long HGui_LcdLine(void);
unsigned long HGui_LcdSize(void);

/** @brief ��Ļˢ��*/
void HGui_flushScreen(S_WORD x, S_WORD y, S_WORD w, S_WORD h);
/** @brief ��Ļˢ��ʹ��*/
bool HGui_FlushScreenEn(bool enable);
/** @brief �ر���Ļ*/
void HGui_CloseScreen(void);
/** @brief ����Ļ*/
void HGui_OpenScreen(void);
/** @brief ����ƿ���*/
void HGui_backlight(int on);
/** @brief ���öԱȶ�
 *  @param[in] contrast 0-99
 */
void HGui_contrast(int contrast);
/** @brief ���ñ��������
 *  @param[in] brightness 0-99
 */
void HGui_brightness(int brightness);
/** @brief ���ûҶ�
 *  @param[in] grayness 0-99
 */
void HGui_grayness(int grayness);
/** @brief ������Ļ����
 *  @param[in] direction 0-3
 */
void HGui_direction(int direction);

/** @brief ���̳�ʼ��*/
int HGui_KeyboardInit(void);
void HGui_KeyISR(unsigned short key);
S_WORD HGui_PollKey(S_VOID);

/** @brief ��������ʼ�� */
int HGui_TouchscrnInit(void);

/** @brief ��ѯ��Ϣ*/
S_GUIEVT HGui_PollEvt(void);


#endif //__HGUI_BASE_H__



