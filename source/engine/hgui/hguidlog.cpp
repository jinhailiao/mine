//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguidlog.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-06-06  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "hguiapp.h"
#include "hguidlog.h"
#include "hguictrl.h"

//
//
//
C_HGUIDLG::C_HGUIDLG(void)
{
	m_DlgRun = true;
}

C_HGUIDLG::~C_HGUIDLG()
{
}

int C_HGUIDLG::DoModel(void)
{
	int ok = MB_RTN_NULL;
	if (SGuiApp->BeginDlg(this) != 0)
		return MB_RTN_ERROR;

	C_HGUICARET *pCaret = SGuiApp->GetAppCaret();
	if (pCaret && pCaret->GetCaretStatus())
		pCaret->SendWndEvt(EVT_PAINT, 0, 0);
	SGuiApp->SetAppCaret(NULL);

	SendWndEvt(EVT_CREATE, 0x00, 0x00);
//	SendWndMsg(GM_PAINT, 0x00, 0x00);
	InvalidateRect(NULL);
	S_GUIEVT aEvt;

	while (m_DlgRun)
	{
		aEvt = SGuiApp->GetGuiEvt();
		SGuiApp->TranslateEvt(aEvt);
		ok = SGuiApp->DispatchGuiEvt(aEvt);
	}

	SGuiApp->EndDlg();
	SGuiApp->SetAppCaret(pCaret);
	return ok;
}

int C_HGUIDLG::EndDlg(void)
{
	SendWndEvt(EVT_DESTROY, 0x00, 0x00);
	DeleteAutoReleaseControl();
	ReleaseWndData();
	m_DlgRun = false;
	return 0;
}

int C_HGUIDLG::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return DefWndProcess(evt, wParam, lParam);
}

int C_HGUIDLG::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_HGUIWND::DefWndProcess(evt, wParam, lParam);
}


//
//
//
C_MSGBOX::C_MSGBOX(const S_CHAR *ptitle, const S_CHAR *ptext, S_DWORD dwFlag)
{
	m_Title = ptitle;
	m_WndText = ptext;
	m_pParent = NULL;
	m_flag = dwFlag | HGUI_WNDF_VISIBLE;
	m_DelayMS = 0;
}

C_MSGBOX::~C_MSGBOX()
{
}

int C_MSGBOX::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case EVT_CREATE:
		m_WndRect.x = 20;
		m_WndRect.w = 120;
		if ((GetWndLong()&MB_BTN_MASK) == MB_BTN_NULL)
			m_WndRect.y = 60, m_WndRect.h = 60;
		else
			m_WndRect.y = 50, m_WndRect.h = 80;
		CreateButton();
		break;
	case EVT_PAINT:{
		C_HGUIDC *pdc = BeginPaint();
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawHLine(1, 16, m_WndRect.w-2);
		DrawTitle(pdc);
		DrawText(pdc);
		EndPaint(pdc);
		}	
		break;
	case EVT_KEYUP:
		if ((GetWndLong()&MB_BTN_MASK) == MB_BTN_NULL)
		{
			EndDlg();
			return MB_RTN_NULL;
		}
		else if ((GetWndLong()&MB_BTN_MASK) == MB_BTN_OKCANCEL)
		{
			if (wParam == VK_LEFT)
				SetFocusCtrl(GetWndCtrl(Btn_OK_ID));
			else if (wParam == VK_RIGHT)
				SetFocusCtrl(GetWndCtrl(Btn_Cancel_ID));
		}
		break;
	case EVT_COMMAND:
		if (wParam == Btn_OK_ID && lParam == HGUI_EVT_CMD_BTN_PUSHED)//OK
		{
			EndDlg();
			return MB_RTN_YESOK;
		}
		else if (wParam == Btn_Cancel_ID && lParam == HGUI_EVT_CMD_BTN_PUSHED)//Cancel
		{
			EndDlg();
			return MB_RTN_CANCEL;
		}
		break;
	case EVT_TIMER:
		m_DelayMS += EVT_TIME_INTERVAL_MS;
		if (m_DelayMS > MSGBOX_DELAY_MAXMS)
		{
			EndDlg();
			return MB_RTN_NULL;
		}
		break;
	case EVT_DESTROY:
		DestroyButton();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_MSGBOX::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_HGUIDLG::DefWndProcess(evt, wParam, lParam);
}

int C_MSGBOX::DrawTitle(C_HGUIDC *pdc)
{
	int TitleX = 2;
	if ((GetWndLong()&MB_TITLE_MASK) == MB_TITLE_CENTER)
	{
		int TitleW = pdc->GetStringExtent(m_Title.c_str());
		if (TitleW < m_WndRect.w) TitleX = (m_WndRect.w - TitleW) / 2;
	}
	return pdc->DrawString(TitleX, 2, m_Title.c_str());
}

int C_MSGBOX::DrawText(C_HGUIDC *pdc)
{
	int TextX = 2;
	if ((GetWndLong()&MB_TEXT_MASK) == MB_TEXT_CENTER)
	{
		int TextW = pdc->GetStringExtent(m_WndText.c_str());
		if (TextW < m_WndRect.w) TextX = (m_WndRect.w - TextW) / 2;
	}
	return pdc->DrawString(TextX, 30, m_WndText.c_str());
}

int C_MSGBOX::CreateButton(void)
{
	if ((GetWndLong()&MB_BTN_MASK) == MB_BTN_OK)
	{
		C_BUTTON *pbtnOK = C_BUTTON::NewCtrl();
		S_RECT rect;
		rect.w = 32; rect.h = 16;
		rect.x = (m_WndRect.w - rect.w) / 2;
		rect.y = 60;
		pbtnOK->Create("确定", 0x00, rect, this, Btn_OK_ID);
		SetFocusCtrl(pbtnOK);
	}
	else if ((GetWndLong()&MB_BTN_MASK) == MB_BTN_OKCANCEL)
	{
		C_BUTTON *pbtnOK = C_BUTTON::NewCtrl();
		C_BUTTON *pbtnCancel = C_BUTTON::NewCtrl();
		S_RECT rect;
		rect.w = 32; rect.h = 16;
		rect.x = (m_WndRect.w - (rect.w + rect.w + 10)) / 2;
		rect.y = 60;
		pbtnOK->Create("确定", 0x00, rect, this, Btn_OK_ID);
		rect.x += rect.w + 10;
		pbtnCancel->Create("取消", 0x00, rect, this, Btn_Cancel_ID);
		if ((GetWndLong()&MB_BTN_FOCUS_MASK) == MB_BTN_FOCUS_MASK)
			SetFocusCtrl(pbtnCancel);
		else
			SetFocusCtrl(pbtnOK);
	}
	return 0;
}

int C_MSGBOX::DestroyButton(void)
{
	if ((GetWndLong()&MB_BTN_MASK) == MB_BTN_OK)
	{
		delete (RemoveControl(Btn_OK_ID));
	}
	else if ((GetWndLong()&MB_BTN_MASK) == MB_BTN_OKCANCEL)
	{
		delete (RemoveControl(Btn_OK_ID));
		delete (RemoveControl(Btn_Cancel_ID));
	}
	return 0;
}


int HGui_MsgBox(const S_CHAR *ptitle, const S_CHAR *ptext, S_DWORD dwFlag)
{
	C_MSGBOX MsgBox(ptitle, ptext, dwFlag);
	return MsgBox.DoModel();
}



