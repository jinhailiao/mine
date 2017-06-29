/** @file
 *  @brief 
 *  @author jinhailiao
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "demoapp.h"

#define LCD_WIDTH 480
#define LCD_HEIGHT 272
#define APP_RESOURCE_PATH ".//"

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

int C_DESKTOP::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
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
	case EVT_KEYUP:
//		GOTOWND(C_MENUWND, 0);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}



