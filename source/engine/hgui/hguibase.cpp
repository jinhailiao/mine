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
//HGUI全局资源
//
char g_HGui_ResourcePath[CFG_PATH_FILE_MAX_LEN] = "./"; //显示资源路径
char HGui_UsrBuffer[HGUI_LCD_SIZE]; //用户显存 [用最大屏幕的]
char *HGui_fb = NULL; //屏幕显存

/** @brief 设置HGUI 驱动 */
int HGui_SetDriver(int hguiDriverID)
{
	return 0;
}

/** @brief 资源路径初始化*/
char *HGui_ResourcePathInit(const char *pResourcePath)
{
	if (pResourcePath != NULL)
		sprintf(g_HGui_ResourcePath, "%s", pResourcePath);
	return g_HGui_ResourcePath;
}

/** @brief LCD初始化*/
int HGui_LcdInit(void)
{
	return 0;
}

/** @brief Lcd参数 */
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
/** @brief 屏幕刷新*/
void HGui_flushScreen(S_WORD x, S_WORD y, S_WORD w, S_WORD h)
{
	if (flushScrnEnable == true)
		memcpy(HGui_fb, HGui_UsrBuffer, sizeof(HGui_UsrBuffer));//fbfresh();
}

/** @brief 屏幕刷新使能*/
bool HGui_FlushScreenEn(bool enable)
{
	bool old = flushScrnEnable;
	flushScrnEnable = enable;
	return old;
}

/** @brief 关闭屏幕*/
void HGui_CloseScreen(void)
{
}

/** @brief 打开屏幕*/
void HGui_OpenScreen(void)
{
}

/** @brief 背光灯开关*/
void HGui_backlight(int on)
{
	if (on != 0)
		on = 1;
}

/** @brief 设置对比度
 *  @param[in] contrast 0-99
 */
void HGui_contrast(int contrast)
{
	if (contrast < 0 || contrast > 99)
		contrast = 99;
}

/** @brief 设置背光灯亮度
 *  @param[in] brightness 0-99
 */
void HGui_brightness(int brightness)
{
	if (brightness < 0 || brightness > 99)
		brightness = 99;
}

/** @brief 设置灰度
 *  @param[in] grayness 0-99
 */
void HGui_grayness(int grayness)
{
	if (grayness < 0 || grayness > 99)
		grayness = 99;
}

/** @brief 设置屏幕方向
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
/** @brief 键盘初始化*/
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

/** @brief 触摸屏初始化 */
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

/** @brief 查询消息*/
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


