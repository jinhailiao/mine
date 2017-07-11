//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGUI
// File:			hguiapp.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_APP_H__
#define __HGUI_APP_H__
#include "haibase.h"
#include "hguiobj.h"
#include "hguiwnd.h"
#include "hguievt.h"
#include "hguicart.h"

typedef stack< C_HGUIWND* > C_WNDSTACK;
typedef queue< S_GUIEVT > C_EVTQ;

class C_HGUIAPP:public C_HGUIOBJ
{
public:
	C_HGUIAPP(void):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_APP){m_pCurWnd = NULL;m_pCurDlg = NULL;m_pCurCaret = NULL;}
	virtual ~C_HGUIAPP(){}

public:
	static void SetCurApp(C_HGUIAPP *pGuiApp){m_pGuiApp = pGuiApp;}
	static C_HGUIAPP *GetCurApp(void){return m_pGuiApp;}

public:
	int SetGuiDriver(int GuiDriverID);
	int SysInit(const char *pResourcePath);
	int CloseScreen(void);
	int OpenScreen(void);

	int SendGuiEvt(S_GUIEVT GuiEvt);
	int PostGuiEvt(S_GUIEVT GuiEvt);
	int DispatchGuiEvt(S_GUIEVT GuiEvt);
	void SetDesktopWnd(C_HGUIWND *pDesktopWnd);
	S_GUIEVT GetGuiEvt(void);

	int GotoWnd(C_HGUIWND *pWnd, S_DWORD UserData = 0);
	int CloseWnd(C_HGUIWND *pWnd);
	int DeleteWnd(C_HGUIWND *pWnd);
	int PopWnd(void);

	int BeginDlg(C_HGUIWND *pCurDlg);
	int EndDlg(void);

	C_HGUICARET *GetAppCaret(void){return m_pCurCaret;}
	void SetAppCaret(C_HGUICARET *pCaret){m_pCurCaret = pCaret;}

	virtual int GuiAppRun(void);
	virtual int TranslateEvt(S_GUIEVT &evt);

protected:
	int EmptyEvtQ(void);
	int EmptyWndStack(void);
	C_HGUIWND *GetCurrentWnd(void);

protected:
	static C_HGUIAPP *m_pGuiApp;
	
protected:
	C_HGUICARET *m_pCurCaret;
	C_HGUIWND *m_pCurDlg;
	C_HGUIWND *m_pCurWnd;
	C_WNDSTACK m_WndStack;
	C_EVTQ m_EvtQ;
};


#define SGuiApp	C_HGUIAPP::GetCurApp()
#define GOTOWND(WndT, UsrData)	 do{C_HGUIWND *pWnd = new WndT; SGuiApp->GotoWnd(pWnd, (S_DWORD)(UsrData));}while(0)
#define GOTOWNDEX(WndT, UsrData, UserPointer)	 do{C_HGUIWND *pWnd = new WndT; pWnd->SetFatherWndData(UserPointer); SGuiApp->GotoWnd(pWnd, (S_DWORD)(UsrData));}while(0)
#define RETURNWND()	do{SGuiApp->CloseWnd(NULL);}while(0)

#endif //__HGUI_APP_H__

