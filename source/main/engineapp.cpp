/** @file
 *  @brief 
 *  @author jinhailiao
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "engineapp.h"
#include "hguicfg.h"
#include "input.h"
#include "script.h"

#define LCD_WIDTH HGUI_LCD_WIDTH
#define LCD_HEIGHT HGUI_LCD_HEIGHT
#define APP_RESOURCE_PATH "..\\..\\resource\\bmp\\"

C_MINEAPP::C_MINEAPP(void)
{
}

C_MINEAPP::~C_MINEAPP()
{
}

#define EVT_GAME_UPDATE (EVT_USER + 1)
int C_MINEAPP::GuiAppRun(void)
{
	S_GUIEVT aEvt = PeekGuiEvt();
	if (aEvt.Evt == EVT_NULL)
		aEvt.Evt = EVT_GAME_UPDATE;
	TranslateEvt(aEvt);
	DispatchGuiEvt(aEvt);

	return 0;
}

int C_MINEWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case EVT_CREATE:{
		C_LuaScript &LuaScript = C_LuaScript::GetInstance();
		LuaScript.Init();
		LuaScript.loadScritp("..\\..\\script\\mine.lua");
		LuaScript.call("GameInit");
		}break;
	case EVT_PAINT:{
		C_HGUIDC *pDC = BeginPaint();
		C_LuaScript &LuaScript = C_LuaScript::GetInstance();
		LuaScript.call("GameDraw");
		EndPaint(pDC);
		}break;
	case EVT_KEYUP:
		break;
	case EVT_MOUSEDB:
	case EVT_MOUSEDN:{
		S_WORD state = MOUSE_STATE_CLICK;
		C_INPUT &input = C_INPUT::GetInstance();
		if (msg == EVT_MOUSEDB)
			state = MOUSE_STATE_DBCLICK;
		input.SetMouseState(state, lParam);
		}break;
	case EVT_GAME_UPDATE:{
		C_LuaScript &LuaScript = C_LuaScript::GetInstance();
		LuaScript.call("GameUpdate");
		SendWndEvt(EVT_PAINT, 0, 0);
		}break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_MINELOGO::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
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
		GOTOWND(C_MINEWND, 0);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}



