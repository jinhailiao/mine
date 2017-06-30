//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguibase.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "hguibase.h"
#include "hguivirkey.h"

//
//
//
#define HGUI_LCD_WIDTH		480
#define HGUI_LCD_HEIGHT		272
#define HGUI_LCD_BPP		24
#define HGUI_LCD_LINE	(HGUI_LCD_WIDTH * HGUI_LCD_BPP / 8)
#define HGUI_LCD_SIZE	(HGUI_LCD_LINE * HGUI_LCD_HEIGHT)

//
//HGUIȫ����Դ
//
char g_HGui_ResourcePath[CFG_PATH_FILE_MAX_LEN] = "./"; //��ʾ��Դ·��
char HGui_UsrBuffer[HGUI_LCD_SIZE]; //�û��Դ� [�������Ļ��]
char *HGui_fb = NULL; //��Ļ�Դ�

/** @brief ����HGUI ���� */
int HGui_SetDriver(int hguiDriverID)
{
	return 0;
}

/** @brief ��Դ·����ʼ��*/
char *HGui_ResourcePathInit(const char *pResourcePath)
{
	if (pResourcePath != NULL)
		sprintf(g_HGui_ResourcePath, "%s", pResourcePath);
	return g_HGui_ResourcePath;
}

/** @brief LCD��ʼ��*/
int HGui_LcdInit(void)
{
	return 0;
}

/** @brief Lcd���� */
char *HGui_LcdBuffer(void)
{
	return HGui_UsrBuffer;
}
unsigned short HGui_LcdWidth(void)
{
	return HGUI_LCD_WIDTH;
}
unsigned short HGui_LcdHeight(void)
{
	return HGUI_LCD_HEIGHT;
}
unsigned short HGui_LcdBpp(void)
{
	return HGUI_LCD_BPP;
}
unsigned long HGui_LcdLine(void)
{
	return ((unsigned long)HGui_LcdWidth() * HGui_LcdBpp() / 8);
}
unsigned long HGui_LcdSize(void)
{
	return ((unsigned long)HGui_LcdLine() * HGui_LcdHeight());
}

static bool flushScrnEnable = true;
/** @brief ��Ļˢ��*/
void HGui_flushScreen(S_WORD x, S_WORD y, S_WORD w, S_WORD h)
{
	if (flushScrnEnable == true)
		memcpy(HGui_fb, HGui_UsrBuffer, sizeof(HGui_UsrBuffer));//fbfresh();
}

/** @brief ��Ļˢ��ʹ��*/
bool HGui_FlushScreenEn(bool enable)
{
	bool old = flushScrnEnable;
	flushScrnEnable = enable;
	return old;
}

/** @brief �ر���Ļ*/
void HGui_CloseScreen(void)
{
}

/** @brief ����Ļ*/
void HGui_OpenScreen(void)
{
}

/** @brief ����ƿ���*/
void HGui_backlight(int on)
{
	if (on != 0)
		on = 1;
}

/** @brief ���öԱȶ�
 *  @param[in] contrast 0-99
 */
void HGui_contrast(int contrast)
{
	if (contrast < 0 || contrast > 99)
		contrast = 99;
}

/** @brief ���ñ��������
 *  @param[in] brightness 0-99
 */
void HGui_brightness(int brightness)
{
	if (brightness < 0 || brightness > 99)
		brightness = 99;
}

/** @brief ���ûҶ�
 *  @param[in] grayness 0-99
 */
void HGui_grayness(int grayness)
{
	if (grayness < 0 || grayness > 99)
		grayness = 99;
}

/** @brief ������Ļ����
 *  @param[in] direction 0-3
 */
void HGui_direction(int direction)
{
	if (direction < 0 || direction > 3)
		direction = 0;
}

static bool fUserInputEnable = true;
bool HGui_SetUserInputEnable(bool fEnable)
{
	bool old = fUserInputEnable;
	fUserInputEnable = fEnable;

	return old;
}


static S_WORD KeyBuffer;
/** @brief ���̳�ʼ��*/
int HGui_KeyboardInit(void)
{
	return 0;
}

void HGui_KeyISR(unsigned short key)
{
	if (fUserInputEnable == false)
		return;
	
	KeyBuffer = key;
}

S_WORD HGui_PollKey(S_VOID)
{
	S_WORD key = KeyBuffer;
	KeyBuffer = 0x00;

	return key;
}

/** @brief ��������ʼ�� */
int HGui_TouchscrnInit(void)
{
	return 0;
}

static S_WORD TouchEvt = 0;
static S_DWORD TouchPos = 0;
const S_WORD TouchEvtType[] = 
{
	EVT_TOUCHDN, EVT_TOUCHMV, EVT_TOUCHUP, EVT_TOUCHDB
};

void HGui_TouchISR(unsigned short Evt, unsigned short x, unsigned short y)
{
	if (fUserInputEnable == false)
		return;

	TouchEvt = TouchEvtType[Evt];
	TouchPos = HAI_COMBDWORD(x, y);
}

S_WORD HGui_PollTouch(S_DWORD &Pos)
{
	S_WORD evt = TouchEvt;

	Pos = TouchPos;
	TouchEvt = 0x00;
	TouchPos = 0x00;

	return evt;
}

/** @brief ��ѯ��Ϣ*/
S_GUIEVT HGui_PollEvt(void)
{
	S_GUIEVT aEvt;
	S_WORD key = HGui_PollKey();
	if (key != 0x00)
	{
		if (key >= VK_F1 && key <= VK_F24)
			aEvt.Evt = EVT_SYSKEY;
		else
			aEvt.Evt = EVT_KEYUP;
		aEvt.wParam = key;
	}
	else
	{
		S_DWORD Pos = 0x00;

		aEvt.Evt = HGui_PollTouch(Pos);
		if (aEvt.Evt != 0x00)
			aEvt.lParam = Pos;
	}
	return aEvt;
}


