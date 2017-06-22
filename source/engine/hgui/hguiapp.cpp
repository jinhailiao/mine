//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGUI
// File:			hguiapp.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "haitime.h"
#include "hguiapp.h"
#include "hguibase.h"
#include "hguictrl.h"

C_HGUIAPP *C_HGUIAPP::m_pGuiApp = NULL;

int C_HGUIAPP::SetGuiDriver(int GuiDriverID)
{
	HGui_SetDriver(GuiDriverID);
	return 0;
}

int C_HGUIAPP::SysInit(const char *pResourcePath)
{
	HGui_ResourcePathInit(pResourcePath);
	if (HGui_LcdInit() != 0)
		return -1;
	if (HGui_KeyboardInit() != 0)
		return -1;
	if (HGui_TouchscrnInit() != 0)
		return -1;

	return 0;
}

int C_HGUIAPP::CloseScreen(void)
{
	HGui_CloseScreen();
	return 0;
}

int C_HGUIAPP::OpenScreen(void)
{
	HGui_OpenScreen();
	return 0;
}

void C_HGUIAPP::SetDesktopWnd(C_HGUIWND *pDesktopWnd)
{
	m_pCurWnd = pDesktopWnd;

 	/** push EVT_CREATE evt
	 */
	S_GUIEVT aEvt;
	aEvt.pWnd = m_pCurWnd;
	aEvt.Evt = EVT_CREATE;
	aEvt.wParam = 0;
	aEvt.lParam = 0;
	m_EvtQ.push(aEvt);
	/** push EVT_SYSPAINT evt
	 */
	aEvt.pWnd = NULL;
	aEvt.Evt = EVT_SYSPAINT;
	aEvt.wParam = 0;
	aEvt.lParam = 0;
	m_EvtQ.push(aEvt);
	/** push EVT_PAINT evt
	 */
	pDesktopWnd->InvalidateRect(NULL);
}

int C_HGUIAPP::GuiAppRun(void)
{
	S_GUIEVT aEvt;

	while (true)
	{
		aEvt = GetGuiEvt();
		TranslateEvt(aEvt);
		DispatchGuiEvt(aEvt);
	}

	return 0;
}

S_GUIEVT C_HGUIAPP::GetGuiEvt(void)
{
	S_GUIEVT aEvt;
	static S_TIME s_timeSystem = C_TIME::TimeNow();
	
	while (m_EvtQ.empty())
	{
		/**poll key
		 */
		aEvt = HGui_PollEvt();
		if (aEvt.Evt != EVT_NULL)
			m_EvtQ.push(aEvt);

		/** time tick message
		 */
		S_TIME CurTime = C_TIME::TimeNow();
		if (CurTime != s_timeSystem)
		{
			s_timeSystem = CurTime;
			if (m_pCurDlg != NULL) aEvt.pWnd = m_pCurDlg;
			else aEvt.pWnd = m_pCurWnd;
			aEvt.Evt = EVT_TIMER;
			aEvt.wParam = 0;
			aEvt.lParam = 0;
			m_EvtQ.push(aEvt);
		}

		if (m_EvtQ.empty() == true)
			SleepMS(50);
	}

	aEvt = m_EvtQ.front();
	m_EvtQ.pop();

	return aEvt;
}

int C_HGUIAPP::SendGuiEvt(S_GUIEVT GuiEvt)
{
	if (GuiEvt.pWnd == NULL)
		return -1;

	return GuiEvt.pWnd->SendWndEvt(GuiEvt.Evt, GuiEvt.wParam, GuiEvt.lParam);
}

int C_HGUIAPP::PostGuiEvt(S_GUIEVT GuiEvt)
{
	m_EvtQ.push(GuiEvt);

	return 0;
}

int C_HGUIAPP::TranslateEvt(S_GUIEVT &evt)
{
	switch(evt.Evt)
	{
	case EVT_TIMER:
		if (m_pCurCaret != NULL)
			return m_pCurCaret->SendWndEvt(evt.Evt, evt.wParam, evt.lParam);
	default:
		break;
	}
	return 0;
}

int C_HGUIAPP::DispatchGuiEvt(S_GUIEVT GuiEvt)
{
	C_WNDBASE *pWnd;

	if (GuiEvt.Evt == EVT_NULL)
		return -1;

	if (GuiEvt.pWnd != NULL)
	{
		return GuiEvt.pWnd->SendWndEvt(GuiEvt.Evt, GuiEvt.wParam, GuiEvt.lParam);
	}
	else if (m_pCurDlg != NULL)
	{
		if ((pWnd = m_pCurDlg->GetFocusCtrl()) != NULL)
			return pWnd->SendWndEvt(GuiEvt.Evt, GuiEvt.wParam, GuiEvt.lParam);
		else
			return SGuiApp->m_pCurDlg->SendWndEvt(GuiEvt.Evt, GuiEvt.wParam, GuiEvt.lParam);
	}
	else if (m_pCurWnd != NULL)
	{
		if ((pWnd = m_pCurWnd->GetFocusCtrl()) != NULL)
			return pWnd->SendWndEvt(GuiEvt.Evt, GuiEvt.wParam, GuiEvt.lParam);
		else
			return SGuiApp->m_pCurWnd->SendWndEvt(GuiEvt.Evt, GuiEvt.wParam, GuiEvt.lParam);
	}

	return -1;
}

int C_HGUIAPP::GotoWnd(C_HGUIWND *pWnd, S_DWORD UserData)
{
	/** Hide caret
	 */
	SGuiApp->SetAppCaret(NULL);

	m_WndStack.push(m_pCurWnd);//push current wnd
	m_pCurWnd = pWnd;

	EmptyEvtQ();
	/** push EVT_CREATE evt
	 */
	S_GUIEVT aEvt;
	aEvt.pWnd = m_pCurWnd;
	aEvt.Evt = EVT_CREATE;
	aEvt.wParam = 0;
	aEvt.lParam = UserData;
	m_EvtQ.push(aEvt);
	/** push EVT_PAINT evt
	 */
	m_pCurWnd->InvalidateRect(NULL);

	return 0;
}

int C_HGUIAPP::CloseWnd(C_HGUIWND *pWnd)
{
	C_HGUIWND *pwnd = (pWnd? pWnd:m_pCurWnd);
	EmptyEvtQ();
	/** push EVT_DESTROY evt
	 */
	S_GUIEVT aEvt;
	aEvt.pWnd = pwnd;
	aEvt.Evt = EVT_DESTROY;
	aEvt.wParam = 0;
	aEvt.lParam = 0;
	m_EvtQ.push(aEvt);
	/** push EVT_QUIT evt
	 */
	aEvt.pWnd = pwnd;
	aEvt.Evt = EVT_QUIT;
	aEvt.wParam = 0;
	aEvt.lParam = 0;
	m_EvtQ.push(aEvt);
	return 0;
}

int C_HGUIAPP::DeleteWnd(C_HGUIWND *pWnd)
{
	pWnd->DeleteAutoReleaseControl();
	pWnd->ReleaseWndData();
	delete pWnd;
	return 0;
}

int C_HGUIAPP::PopWnd(void)
{
	if (m_WndStack.empty())
		return -1;

	DeleteWnd(m_pCurWnd);
	m_pCurWnd = m_WndStack.top();
	m_WndStack.pop();

	EmptyEvtQ();
	/** push EVT_RESTORE evt
	 */
	S_GUIEVT aEvt;
	aEvt.pWnd = m_pCurWnd;
	aEvt.Evt = EVT_RESTORE;
	aEvt.wParam = 0;
	aEvt.lParam = 0;
	m_EvtQ.push(aEvt);
	/** push EVT_PAINT evt
	 */
	m_pCurWnd->InvalidateRect(NULL);

	/** Restore focus
	 */
	SGuiApp->SetAppCaret(NULL);
	aEvt.pWnd = m_pCurWnd->GetFocusCtrl();
	if (aEvt.pWnd == NULL)
        aEvt.pWnd = m_pCurWnd;
	aEvt.Evt = EVT_SETFOCUS;
	aEvt.wParam = 0;
	aEvt.lParam = 0;
	m_EvtQ.push(aEvt);

	return 0;
}

int C_HGUIAPP::BeginDlg(C_HGUIWND *pCurDlg)
{
	if (m_pCurDlg != NULL)
		return -1;
	EmptyEvtQ();
	m_pCurDlg = pCurDlg;
	return 0;
}

int C_HGUIAPP::EndDlg(void)
{
	m_pCurDlg = NULL;

	EmptyEvtQ();
	/** push EVT_RESTORE evt
	 */
	S_GUIEVT aEvt;
	aEvt.pWnd = m_pCurWnd;
	aEvt.Evt = EVT_RESTORE;
	aEvt.wParam = 0;
	aEvt.lParam = 0;
	m_EvtQ.push(aEvt);
	/** push EVT_PAINT evt
	 */
	m_pCurWnd->InvalidateRect(NULL);
	return 0;
}


int C_HGUIAPP::EmptyEvtQ(void)
{
	while (!m_EvtQ.empty())
		m_EvtQ.pop();
	return 0;
}

int C_HGUIAPP::EmptyWndStack(void)
{
	S_GUIEVT aEvt;

	if (m_pCurDlg != NULL)//wait Evtbox close
		return -1;
	if (m_WndStack.empty())
		return -1;

	//Query wnd auto back
	aEvt.pWnd = m_pCurWnd;
	aEvt.Evt = EVT_AUTOBACK;
	if (SendGuiEvt(aEvt) != 0)//current wnd is not auto back still
		return -1;

	//clean evtQ
	EmptyEvtQ();
	
	while (!m_WndStack.empty())
	{
		/** restore user data first
		 */
		aEvt.pWnd = m_pCurWnd;
		aEvt.Evt = EVT_RESTORE;
		SendGuiEvt(aEvt);
		/** destroy wnd
		 */
		aEvt.pWnd = m_pCurWnd;
		aEvt.Evt = EVT_DESTROY;
		SendGuiEvt(aEvt);

		DeleteWnd(m_pCurWnd);
		m_pCurWnd = m_WndStack.top();
		m_WndStack.pop();
	}
	/** restore user data first
	 */
	aEvt.pWnd = m_pCurWnd;
	aEvt.Evt = EVT_RESTORE;
	SendGuiEvt(aEvt);
	/** paint wnd
	 */
	m_pCurWnd->InvalidateRect(NULL);
	/** restore caret
	 */
	SGuiApp->SetAppCaret(NULL);
	aEvt.pWnd = m_pCurWnd->GetFocusCtrl();
	if (aEvt.pWnd == NULL)
		aEvt.pWnd = m_pCurWnd;
	aEvt.Evt = EVT_SETFOCUS;
	aEvt.wParam = 0;
	aEvt.lParam = 0;
	m_EvtQ.push(aEvt);

	return 0;
}


