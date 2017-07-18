/** @file
 *  @brief 
 *  @author jinhailiao
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "demoapp.h"
#include "hguicfg.h"
#include "hguictrl.h"

#define LCD_WIDTH HGUI_LCD_WIDTH
#define LCD_HEIGHT HGUI_LCD_HEIGHT
#define APP_RESOURCE_PATH "..\\..\\resource\\bmp\\"

C_DEMOAPP::C_DEMOAPP(void)
{
}

C_DEMOAPP::~C_DEMOAPP()
{
}

int C_DEMOAPP::GuiAppRun(void)
{
	S_GUIEVT aEvt = GetGuiEvt();
	TranslateEvt(aEvt);
	DispatchGuiEvt(aEvt);

	return 0;
}

#define DT_TIME_W		76
#define DT_TIME_H		16
#define DT_TIME_X		(LCD_WIDTH-DT_TIME_W-2)
#define DT_TIME_Y		(LCD_HEIGHT-DT_TIME_H-2)

static const char * const ShortCutName[8] = 
{
	"控件1", "控件2", "图拉伸", "图隐现", "图层", "测试", "黄花菜", "分蘖节"
};
/*
S_VOID hai_DisplayTime(SH_DC hDC, S_WORD x, S_WORD y, S_WORD w, S_WORD h)
{
	S_BYTE time[14] = {0x20, 0x20, 0x20, 0x20};

	hai_GetTime(&time[4]);
	hai_DrawButton(hDC, x, y, w, h, S_FALSE, time);
}
*/
int C_DESKTOP::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case EVT_CREATE:{
		for (S_WORD i = 0; i < 8; i++)
		{
			S_RECT Rect;
			Rect.x = 100+(i%4)*70, Rect.w = 48;
			Rect.y = 30+(i/4)*90, Rect.h = 64;
			C_ButtonEx *pButton = C_ButtonEx::NewCtrl();
			pButton->Create(ShortCutName[i], HGUI_BS_PUSHBTN|HGUI_CS_TEXT_BUTTOM, Rect, this, APP_RESOURCE_PATH"tubiao.bmp", APP_RESOURCE_PATH"tubiao2.bmp", APP_RESOURCE_PATH"tubiao2.bmp", i+1);
		}
//		C_ButtonEx *pStart = C_ButtonEx::NewCtrl();
//		pStart->Create("开始", HGUI_BS_PUSHBTN, Rect, this, NULL, NULL, NULL, 10);
//		hai_CreateButton(BS_PUSHBTN, 10, 2, (LCD_HEIGHT-16-2), 40, 16, "开始", 0, 0, 0);
		}break;
	case EVT_PAINT:{
		C_HGUIDC *pDC = BeginPaint();
		pDC->BitBlt(0, 0, LCD_WIDTH, LCD_HEIGHT, NULL, 0, 0, HGUI_WHITENESS);
		pDC->DrawBox(10, LCD_HEIGHT-30, 30, 20);
	//	hai_DisplayTime(hDC, DT_TIME_X, DT_TIME_Y, DT_TIME_W, DT_TIME_H);
	//	hai_SetTextColor(hDC, (S_COLOR)COLOR_GRAY);
	//	hai_TextOut(hDC,  70, 180, "温馨提示:F1为输入法切换按键,F2,F3,F4,F5为消息框测试按键,", 0);
	//	hai_TextOut(hDC, 124, 200, "请在各个版面试按,谢了!", 0);
		EndPaint(pDC);
		}break;
	case EVT_KEYUP:
//		GOTOWND(C_MENUWND, 0);
		break;
	case EVT_MOUSEDN:
//		InvalidateRect(NULL);
		break;
	case EVT_TIMER:
//		InvalidateRect(NULL);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

/*
static void  DTOnCommand(SP_EVT pEvent)
{
	S_BYTE CtrlID = HAI_GETLOBYTE(pEvent->wParam);
	S_BYTE Event  = HAI_GETHIBYTE(pEvent->wParam);
	switch (CtrlID)
	{
	case 1:
		if (Event == EVT_MOUSEUP)
		{
			S_BYTE *pName = (S_BYTE*)HG_DialogBox(140, 90, 180, 90, GetNameDlgProc);
			if (pName)
				GOTOSCENE(SID_CTRLDEMO1, 23, (LPARAM)pName);
		}
		break;
	case 2:
		if (Event == EVT_MOUSEUP)
			GOTOSCENE(SID_CTRLDEMO2, 50, 60);
		break;
	case 3:
		if (Event == EVT_MOUSEUP)
			GOTOSCENE(SID_PICSTRETCH, 50, 60);
		break;
	case 4:
		if (Event == EVT_MOUSEUP)
			GOTOSCENE(SID_PICALPHA, 50, 60);
		break;
	case 5:
		if (Event == EVT_MOUSEUP)
			GOTOSCENE(SID_PICPARALL, 50, 60);
		break;
	case 6:
		if (Event == EVT_MOUSEUP)
			GOTOSCENE(SID_TEST, 50, 60);
		break;
	case 7:
		break;
	case 8:
		break;
	case 10:
		if (Event == BS_PUSHBTN_UP)
			hai_MsgBox("Welcome", "欢迎体验HAISOFT的HGui系统!", MB_BTNOK|MB_RESTORE_BG);
		break;
	default:
		break;
	}
}

static void  DTOnKey(SP_EVT pEvent)
{
	SH_CTRL hFocus = hai_GetCurSceneFocusCtrl();
	S_BYTE CtrlID  = hai_GetCurSceneCtrlID(hFocus);

	switch (pEvent->wParam)
	{
	case VK_LEFT:
		if (CtrlID > 1 && CtrlID < 9)
		{
			CtrlID -= 1;
			hFocus = hai_GetCurSceneCtrl(CtrlID);
			hai_SetFocusCtrl(hFocus);
		}
		else if (CtrlID == 1)
		{
			CtrlID = 8;
			hFocus = hai_GetCurSceneCtrl(CtrlID);
			hai_SetFocusCtrl(hFocus);
		}
		break;
	case VK_RIGHT:
		if (CtrlID > 0 && CtrlID < 8)
		{
			CtrlID += 1;
			hFocus = hai_GetCurSceneCtrl(CtrlID);
			hai_SetFocusCtrl(hFocus);
		}
		else if (CtrlID == 8)
		{
			CtrlID = 1;
			hFocus = hai_GetCurSceneCtrl(CtrlID);
			hai_SetFocusCtrl(hFocus);
		}
		break;
	case VK_UP:
	case VK_DOWN:
		if (CtrlID > 4 && CtrlID < 9)
		{
			CtrlID -= 4;
			hFocus = hai_GetCurSceneCtrl(CtrlID);
			hai_SetFocusCtrl(hFocus);
		}
		else if (CtrlID > 0 && CtrlID < 5)
		{
			CtrlID += 4;
			hFocus = hai_GetCurSceneCtrl(CtrlID);
			hai_SetFocusCtrl(hFocus);
		}
		break;
	default:
		break;
	}
}
*/

int C_DEMOLOGO::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case EVT_CREATE:
		break;
	case EVT_PAINT:{
		C_HGUIDC DCmem;
		C_HGUIBMP Bitmap;

		C_HGUIDC *pDC = BeginPaint();
		Bitmap.LoadBitmap(APP_RESOURCE_PATH"hailogo.bmp");
		DCmem.CreateCompatibleDC(pDC);
		DCmem.SelectObject(&Bitmap);
		pDC->BitBlt(0, 0, LCD_WIDTH, LCD_HEIGHT, NULL, 0, 0, HGUI_WHITENESS);
		pDC->BitBlt((S_SHORT)((LCD_WIDTH-Bitmap.GetWidth())>>1), (S_SHORT)((LCD_HEIGHT-Bitmap.GetHeight())>>1), Bitmap.GetWidth(), Bitmap.GetHeight(), &DCmem, 0, 0, HGUI_SRCCOPY);
		Bitmap.DeleteObject();
		DCmem.DeleteObject();
		EndPaint(pDC);
		}break;
	case EVT_MOUSEUP:
	case EVT_KEYUP:
		GOTOWND(C_DESKTOP, 0);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}



