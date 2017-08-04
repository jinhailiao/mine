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
	case EVT_CREATE:{
		S_WORD TextW = 0, TextH = 0;
 		HandleTextData(TextW, TextH);
		m_WndRect.h = TextH + 40;
		m_WndRect.w = TextW + 20;

		if ((GetWndLong()&MB_BTN_MASK) != MB_BTN_NULL)
			m_WndRect.h += 20;

		if (m_WndRect.h > (HGUI_LCD_HEIGHT*2/3))
			m_WndRect.h = HGUI_LCD_HEIGHT*2/3;
		if (m_WndRect.w > (HGUI_LCD_WIDTH*2/3))
			m_WndRect.w = HGUI_LCD_WIDTH*2/3;

		m_WndRect.x = (HGUI_LCD_WIDTH - m_WndRect.w) / 2;
		m_WndRect.y = (HGUI_LCD_WIDTH - m_WndRect.h) / 2;
		CreateButton();
		}break;
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

int C_MSGBOX::HandleTextData(S_WORD &TextW, S_WORD &TextH)
{
	TextW = 0, TextH = 0;
	if (m_WndText.empty() == true)
		return 0;

	m_TextData.split(m_WndText, '\n');

	C_HGUIDC dc(NULL);
	for (size_t i = 0; i < m_TextData.size(); i++)
	{
		string &strText = m_TextData[i];
		if (*(strText.end()-1) == '\r') 	// 清除 \r
			strText.erase(strText.end()-1);
	
		TextH += dc.GetFontHeight(' ') + 2;
		int w = dc.GetStringExtent(strText.c_str());
		if (w > TextW) TextW = w;
	}

	return (int)m_TextData.size();
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
	S_WORD TextY = 30;
	for (size_t i = 0; i < m_TextData.size(); i++)
	{
		S_WORD TextX = 2;
		string &strText = m_TextData[i];
		if ((GetWndLong()&MB_TEXT_MASK) == MB_TEXT_CENTER)
		{
			int TextW = pdc->GetStringExtent(strText.c_str());
			if (TextW < m_WndRect.w) TextX = (m_WndRect.w - TextW) / 2;
		}
		pdc->DrawString(TextX, TextY, strText.c_str());
		TextY += pdc->GetFontHeight(' ') + 2;
	}
	return 0;
}

int C_MSGBOX::CreateButton(void)
{
	if ((GetWndLong()&MB_BTN_MASK) == MB_BTN_OK)
	{
		C_BUTTON *pbtnOK = C_BUTTON::NewCtrl();
		S_RECT rect;
		rect.w = 32; rect.h = 16;
		rect.x = (m_WndRect.w - rect.w) / 2;
		rect.y = m_WndRect.h - 24;
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
		rect.y = m_WndRect.h - 24;
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

C_EditBoxEx::C_EditBoxEx(const S_CHAR *ptitle, const S_CHAR *pDefault, S_CHAR *pStringData)
:C_MSGBOX(ptitle, pDefault, MB_BTN_OKCANCEL|MB_TITLE_CENTER)	
{
	m_pStringData = pStringData;
}

C_EditBoxEx::~C_EditBoxEx()
{
}

#define EDIT_BOX_CHAR_MAX		20
int C_EditBoxEx::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case EVT_CREATE:{
		S_WORD TextW = EDIT_BOX_CHAR_MAX*6+20, TextH = 16;
		m_WndRect.h = TextH + 40;
		m_WndRect.w = TextW + 20;

		if ((GetWndLong()&MB_BTN_MASK) != MB_BTN_NULL)
			m_WndRect.h += 20;

		if (m_WndRect.h > (HGUI_LCD_HEIGHT*2/3))
			m_WndRect.h = HGUI_LCD_HEIGHT*2/3;
		if (m_WndRect.w > (HGUI_LCD_WIDTH*2/3))
			m_WndRect.w = HGUI_LCD_WIDTH*2/3;

		m_WndRect.x = (HGUI_LCD_WIDTH - m_WndRect.w) / 2;
		m_WndRect.y = (HGUI_LCD_WIDTH - m_WndRect.h) / 2;
		CreateButton();
		CreateEditBox();
		}break;
	case EVT_PAINT:{
		C_HGUIDC *pdc = BeginPaint();
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawHLine(1, 16, m_WndRect.w-2);
		DrawTitle(pdc);
		EndPaint(pdc);
		}	
		break;
	case EVT_KEYUP:
		if ((GetWndLong()&MB_BTN_MASK) == MB_BTN_OKCANCEL)
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
			C_HGUICTRL *pCtrl = GetWndCtrl(Edit_Box_ID);
			string strName = pCtrl->GetWndText();
			replace(strName.begin(), strName.end(), ' ', '\0');
			strcpy(m_pStringData, strName.c_str());

			EndDlg();
			return MB_RTN_YESOK;
		}
		else if (wParam == Btn_Cancel_ID && lParam == HGUI_EVT_CMD_BTN_PUSHED)//Cancel
		{
			EndDlg();
			return MB_RTN_CANCEL;
		}
		break;
	case EVT_DESTROY:
		DestroyButton();
		DestroyEditBox();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_EditBoxEx::CreateEditBox(void)
{
	string strInfo(m_WndText);
	S_RECT RectEd = {10, 24, 140, 16};

	if (strInfo.size() > EDIT_BOX_CHAR_MAX)
		strInfo.erase(strInfo.begin()+EDIT_BOX_CHAR_MAX, strInfo.end());
	if (strInfo.size() < EDIT_BOX_CHAR_MAX)
		strInfo.append((size_t)(EDIT_BOX_CHAR_MAX-strInfo.size()), (char)' ');

	C_TEXTEDIT *pEdit = C_TEXTEDIT::NewCtrl();
	pEdit->Create(strInfo.c_str(), 0x00, RectEd, this, Edit_Box_ID);
	SetFocusCtrl(pEdit);
	return 0;
}

int C_EditBoxEx::DestroyEditBox(void)
{
	delete (RemoveControl(Edit_Box_ID));
	return 0;
}


int HGui_MsgBox(const S_CHAR *ptitle, const S_CHAR *ptext, S_DWORD dwFlag)
{
	C_MSGBOX MsgBox(ptitle, ptext, dwFlag);
	return MsgBox.DoModel();
}

int HGui_EditBox(const S_CHAR *ptitle, const S_CHAR *pDefault, S_CHAR *pStringData)
{
	C_EditBoxEx EditBox(ptitle, pDefault, pStringData);
	return EditBox.DoModel();
}


