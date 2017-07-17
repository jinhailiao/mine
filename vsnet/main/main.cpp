// main.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "main.h"
#include <process.h> /* _beginthread, _endthread */
#include "hguicfg.h"
#include "hguidef.h"

#define MAX_LOADSTRING 100

#define DEVICE_LCD_WIDTH		HGUI_LCD_WIDTH
#define DEVICE_LCD_HEIGHT		HGUI_LCD_HEIGHT
#define DEVICE_LCD_BPP			HGUI_LCD_BITSPERPIX

#define SIMULATOR_WND_WIDTH		(DEVICE_LCD_WIDTH+6)
#define SIMULATOR_WND_HEIGHT	(DEVICE_LCD_HEIGHT+28)

#define	ID_TIMER_PAINT_SCREEN	100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

//BITMAP bmpBitmap;
BITMAPINFO bmpBitmapInfo;
char rgbUserBuffer[BITMAP_WIDTHBYTES(DEVICE_LCD_WIDTH, DEVICE_LCD_BPP)*DEVICE_LCD_HEIGHT];
extern char *HGui_fb;

extern void HGui_KeyISR(unsigned short key, bool fDown);
extern void HGui_MouseISR(unsigned short Evt, unsigned short x, unsigned short y);

extern int InitEngine(void);
extern int RunEngine(void);
extern void MainEngine(void *pData);


// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	HGui_fb = rgbUserBuffer;
	memset(rgbUserBuffer, 0x00, sizeof(rgbUserBuffer));

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MAIN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	bmpBitmapInfo.bmiHeader.biSize = 40;
	bmpBitmapInfo.bmiHeader.biWidth = DEVICE_LCD_WIDTH;
	bmpBitmapInfo.bmiHeader.biHeight = DEVICE_LCD_HEIGHT; 
	bmpBitmapInfo.bmiHeader.biPlanes = 1; 
	bmpBitmapInfo.bmiHeader.biBitCount = DEVICE_LCD_BPP;
	bmpBitmapInfo.bmiHeader.biCompression = BI_RGB;
	bmpBitmapInfo.bmiHeader.biSizeImage = sizeof(rgbUserBuffer);
	bmpBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	bmpBitmapInfo.bmiHeader.biClrUsed = 0;
	bmpBitmapInfo.bmiHeader.biClrImportant = 0;
	
	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MAIN);

	_beginthread(MainEngine, 0, NULL);

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
#if 0
	InitEngine();
	while (1)
    {
        if (! PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Get current time tick.
 //           QueryPerformanceCounter(&nNow);

            // If it's the time to draw next frame, draw it, else sleep a while.
//            if (nNow.QuadPart - nLast.QuadPart > m_nAnimationInterval.QuadPart)
//            {
//                nLast.QuadPart = nNow.QuadPart;
//                CCDirector::sharedDirector()->mainLoop();
//            }
//            else
//            {
//                Sleep(0);
//            }
			RunEngine();
            continue;
        }

        if (WM_QUIT == msg.message)
        {
            // Quit message loop.
            break;
        }

        // Deal with windows message.
        if (! TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
#endif
	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释: 
//
//    仅当希望在已添加到 Windows 95 的
//    “RegisterClassEx”函数之前此代码与 Win32 系统兼容时，
//    才需要此函数及其用法。调用此函数
//    十分重要，这样应用程序就可以获得关联的
//   “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MAIN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)NULL;//IDC_MAIN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HANDLE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	RECT rc;
	int  x, y;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = GetDesktopWindow();
	GetClientRect(hWnd, &rc);
	x = (rc.right - rc.left) - SIMULATOR_WND_WIDTH;
	y = (rc.bottom - rc.top) - SIMULATOR_WND_HEIGHT;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	x /= 2;
	y /= 2;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		x, y, SIMULATOR_WND_WIDTH, SIMULATOR_WND_HEIGHT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	DWORD   dwStyle   =   GetWindowLong(hWnd,   GWL_STYLE);   
	dwStyle   &=   ~(WS_SIZEBOX);   
	dwStyle   &=   ~(WS_MAXIMIZEBOX);   
//	dwStyle   &=   ~(WS_MINIMIZEBOX);   
	SetWindowLong(hWnd,   GWL_STYLE,   dwStyle); 

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, unsigned, WORD, LONG)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_CREATE:
		SetTimer(hWnd, ID_TIMER_PAINT_SCREEN, 25, NULL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:{
		hdc = BeginPaint(hWnd, &ps);
		HDC hdcmem = CreateCompatibleDC(hdc);
		HBITMAP HBitmap = CreateDIBSection(hdc, &bmpBitmapInfo,DIB_PAL_COLORS,NULL, NULL, 0);
		SetBitmapBits(HBitmap, sizeof(rgbUserBuffer), rgbUserBuffer);
		HGDIOBJ hOldObj = SelectObject(hdcmem, HBitmap);
		BOOL ok = BitBlt(hdc, 0, 0, DEVICE_LCD_WIDTH, DEVICE_LCD_HEIGHT, hdcmem, 0, 0, SRCCOPY);
		SelectObject(hdcmem, hOldObj);
		DeleteObject(HBitmap);
		DeleteDC(hdcmem);
		EndPaint(hWnd, &ps);
		}break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_KEYDOWN:
		HGui_KeyISR((unsigned short)wParam, true);
		break;
	case WM_KEYUP:
		HGui_KeyISR((unsigned short)wParam, false);
		break;
	case WM_LBUTTONDOWN:
		HGui_MouseISR(0, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		HGui_MouseISR(1, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		HGui_MouseISR(2, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONDBLCLK:
		HGui_MouseISR(3, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		KillTimer(hWnd, ID_TIMER_PAINT_SCREEN);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
