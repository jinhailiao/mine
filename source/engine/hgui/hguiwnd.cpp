//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguiwnd.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "hguiwnd.h"
#include "hguiapp.h"
#include "hguibase.h"
#include "hguicart.h"
#include "hguictrl.h"
#include "hguirect.h"

/** GUI base window define
  */
C_WNDBASE::C_WNDBASE(void):C_HGUIOBJ(C_HGUIOBJ::OBJ_T_WNDB)
{
	m_WndRect.x = 0;
	m_WndRect.y = 0;
	m_WndRect.w = HGui_LcdWidth();
	m_WndRect.h = HGui_LcdHeight();
	m_InvalidRect.x = 0;
	m_InvalidRect.y = 0;
	m_InvalidRect.w = 0;
	m_InvalidRect.h = 0;

	m_flag = HGUI_WNDF_VISIBLE;

	m_pParent = NULL;
	m_pCapture = NULL;
}

C_WNDBASE::~C_WNDBASE()
{
}

int C_WNDBASE::SendWndEvt(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return WndProcess(evt, wParam, lParam);
}

int C_WNDBASE::PostWndEvt(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	S_GUIEVT aEvt;

	aEvt.pWnd = this;
	aEvt.Evt = evt;
	aEvt.wParam = wParam;
	aEvt.lParam = lParam;
	SGuiApp->PostGuiEvt(aEvt);

	return 0;
}

bool C_WNDBASE::SetVisible(bool fVisible)
{
	if (fVisible == true)
		m_flag |= HGUI_WNDF_VISIBLE;
	else
		m_flag &= ~HGUI_WNDF_VISIBLE;
	return true;
}

bool C_WNDBASE::SetFocus(bool fFocus)
{
	if (fFocus == true)
		m_flag |= HGUI_WNDF_FOCUS;
	else
		m_flag &= ~HGUI_WNDF_FOCUS;
	return true;
}

bool C_WNDBASE::SetDisabled(bool fDisable)
{
	if (fDisable == true)
		m_flag |= HGUI_WNDF_DISABLED;
	else
		m_flag &= ~HGUI_WNDF_DISABLED;
	return true;
}

int C_WNDBASE::ClientToScreen(S_RECT &rect)
{
	rect.x += m_WndRect.x, rect.y += m_WndRect.y;
	if (rect.x+rect.w > m_WndRect.x+m_WndRect.w)
		rect.w = m_WndRect.x+m_WndRect.w - rect.x;
	if (rect.y+rect.h > m_WndRect.y+m_WndRect.h)
		rect.h = m_WndRect.y+m_WndRect.h - rect.y;

	C_WNDBASE *pWnd = m_pParent;
	while (pWnd)
	{
		rect.x += pWnd->m_WndRect.x, rect.y += pWnd->m_WndRect.y;
		if (rect.x+rect.w > pWnd->m_WndRect.x+pWnd->m_WndRect.w)
			rect.w = pWnd->m_WndRect.x+pWnd->m_WndRect.w - rect.x;
		if (rect.y+rect.h > pWnd->m_WndRect.y+pWnd->m_WndRect.h)
			rect.h = pWnd->m_WndRect.y+pWnd->m_WndRect.h - rect.y;

		pWnd = pWnd->m_pParent;
	}

	return 0;
}

void C_WNDBASE::UpdateWnd(void)
{
	if (m_InvalidRect.w != 0 && m_InvalidRect.h != 0)
	{
		S_RECT rect = m_InvalidRect;
		ClientToScreen(rect);
		HGui_flushScreen(rect.x, rect.y, rect.w, rect.h);
	}
	else
	{
		HGui_flushScreen(m_WndRect.x, m_WndRect.y, m_WndRect.w, m_WndRect.h);
	}

	m_InvalidRect.x = 0;
	m_InvalidRect.y = 0;
	m_InvalidRect.w = 0;
	m_InvalidRect.h = 0;
}

void C_WNDBASE::ShowWnd(bool bShow)
{
	if (bShow == true)
		m_flag |= HGUI_WNDF_VISIBLE;
	else
		m_flag &= ~HGUI_WNDF_VISIBLE;
}

void C_WNDBASE::EnableWnd(bool bEnable)
{
	if (bEnable == true)
		m_flag |= HGUI_WNDF_DISABLED;
	else
		m_flag &= ~HGUI_WNDF_DISABLED;
}

int C_WNDBASE::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return DefWndProcess(evt, wParam, lParam);
}

int C_WNDBASE::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	switch(evt)
	{
	case EVT_KILLFOCUS:
		SetFocus(false);
		break;
	case EVT_SETFOCUS:
		SetFocus(true);
		break;
	default:
		break;
	}
	return 0;
}

/** GUI window define
  */
C_GUIWNDB::C_GUIWNDB(void)
{
	m_pCurCaret = NULL;
}

C_GUIWNDB::~C_GUIWNDB(void)
{
	delete m_pCurCaret;
}

int C_GUIWNDB::CreateCaret(S_WORD w, S_WORD h)
{
	if (m_pCurCaret == NULL)
		m_pCurCaret = new C_HGUICARET(this, w, h);
	SGuiApp->SetAppCaret(m_pCurCaret);
	return 0;
}

int C_GUIWNDB::SetCaretPos(S_WORD x, S_WORD y)
{
	if (m_pCurCaret == NULL)
		return -1;

	if (m_pCurCaret->GetCaretStatus())
		m_pCurCaret->SendWndEvt(EVT_PAINT, 0, 0);

	S_RECT rect = m_pCurCaret->GetWndRect();
	rect.x = x, rect.y = y;
	m_pCurCaret->SetWndRect(rect);

	m_pCurCaret->SendWndEvt(EVT_PAINT, 0, 0);

	return 0;
}

int C_GUIWNDB::GetCaretPos(S_WORD &x, S_WORD &y)
{
	if (m_pCurCaret == NULL)
		return -1;
	S_RECT rect = m_pCurCaret->GetWndRect();
	x = rect.x,  y = rect.y;

	return 0;
}

int C_GUIWNDB::ShowCaret(void)
{
	if (m_pCurCaret == NULL)
		return -1;
	m_pCurCaret->ShowWnd(true);
	return 0;
}

int C_GUIWNDB::HideCaret(void)
{
	if (m_pCurCaret == NULL)
		return -1;
	if (m_pCurCaret->GetCaretStatus())
		m_pCurCaret->SendWndEvt(EVT_PAINT, 0, 0);
	m_pCurCaret->ShowWnd(false);
	return 0;
}

int C_GUIWNDB::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return DefWndProcess(evt, wParam, lParam);
}

int C_GUIWNDB::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	switch (evt)
	{
	case EVT_SETFOCUS:
		m_flag |= HGUI_WNDF_FOCUS;
		SGuiApp->SetAppCaret(m_pCurCaret);
		break;
	case EVT_KILLFOCUS:{
		C_HGUICARET *pCaret = SGuiApp->GetAppCaret();
		if (pCaret != NULL)
		{
			if (pCaret->GetCaretStatus())
				pCaret->SendWndEvt(EVT_PAINT, 0x00, 0x00);
		}
		m_flag &= ~HGUI_WNDF_FOCUS;
		SGuiApp->SetAppCaret(NULL);}
		break;
	default:
		return C_WNDBASE::DefWndProcess(evt, wParam, lParam);
	}
	return 0;
}


/** User wnd define
 */
C_HGUIWND::C_HGUIWND(void)
{
	m_ObjT = OBJ_T_WND;

	m_WndRect.x = 0;
	m_WndRect.y = 0;
	m_WndRect.w = HGui_LcdWidth();
	m_WndRect.h = HGui_LcdHeight();

	m_pWndData  = NULL;
	m_pFatherWndData = NULL;
	m_ScrnFlushEn = false;
}

C_HGUIWND::~C_HGUIWND()
{
}

C_HGUIDC *C_HGUIWND::BeginPaint(void)
{
	C_HGUIDC *pdc = new C_HGUIDC(this);
	S_RECT rect = m_WndRect;

	C_HGUICARET *pAppCaret = SGuiApp->GetAppCaret();
	if (pAppCaret && pAppCaret->GetCaretStatus())
		pAppCaret->SendWndEvt(EVT_PAINT, 0, 0);
	if (m_pCurCaret && m_pCurCaret->GetCaretStatus())
		m_pCurCaret->SendWndEvt(EVT_PAINT, 0, 0);

	m_ScrnFlushEn = HGui_FlushScreenEn(false);
	pdc->SelectObject(pdc->GetStockGuiObj(HGUI_OBJ_WPEN));
	rect.x = 0, rect.y = 0;
	pdc->FillRect(rect);
	pdc->SelectObject(pdc->GetStockGuiObj(HGUI_OBJ_BPEN));

	return pdc;
}

void C_HGUIWND::EndPaint(C_HGUIDC *pDC)
{
	if (!m_CtrlQ.empty())
	{
		for (S_DWORD i = 0; i < m_CtrlQ.size(); i++)
		{
			C_HGUICTRL *pWnd = m_CtrlQ[i];
			if (pWnd->GetWndLong() & HGUI_WNDF_VISIBLE)
				pWnd->SendWndEvt(EVT_PAINT, 0x00, 0x00);
		}
	}
	
	HGui_FlushScreenEn(m_ScrnFlushEn);
	UpdateWnd();
	delete pDC;
}

void *C_HGUIWND::GetWndData(int size)
{
	if (m_pWndData == NULL)
		m_pWndData = new char[size];

	return m_pWndData;
}

void C_HGUIWND::ReleaseWndData(void)
{
	if (m_pWndData != NULL)
		delete[] m_pWndData;
	m_pWndData = NULL;
}

void *C_HGUIWND::GetFatherWndData(void)
{
	return m_pFatherWndData;
}

void C_HGUIWND::SetFatherWndData(void *pFatherWndData)
{
	m_pFatherWndData = pFatherWndData;
}

int C_HGUIWND::InvalidateRect(const S_RECT *pRect)
{
	S_GUIEVT aEvt;

	/** @brief 防止消息队列中有多个EVT_PAINT消息,
	 * 但此种机制还不完善,暂时屏蔽
	 */
//	if (m_InvalidRect.w!=0x00 && m_InvalidRect.h!=0x00)
//		return -1;

	if (pRect == NULL)
		m_InvalidRect = m_WndRect;
	else
		m_InvalidRect = *pRect;

	/** push EVT_PAINT evt
	 */
	aEvt.pWnd = this;
	aEvt.Evt = EVT_PAINT;
	aEvt.wParam = 0;
	aEvt.lParam = 0;
	SGuiApp->PostGuiEvt(aEvt);

	return 0;
}

C_HGUICTRL *C_HGUIWND::GetFocusCtrl(void)
{
	if (m_CtrlQ.empty())
		return NULL;
	for (S_DWORD i = 0; i < m_CtrlQ.size(); i++)
	{
		C_HGUICTRL *pWnd = m_CtrlQ[i];
		S_DWORD flag = pWnd->GetWndLong();
		if (flag & HGUI_WNDF_FOCUS)
			return pWnd;
	}
	return NULL;
}

C_HGUICTRL *C_HGUIWND::SetFocusCtrl(C_HGUICTRL *pFocusCtrl)
{
	C_HGUICTRL *pCurFocus = GetFocusCtrl();
	if (_SetFocusCtrl(pFocusCtrl) == true)
		InvalidateRect(NULL);
	return pCurFocus;
}

C_HGUICTRL *C_HGUIWND::GetWndCtrl(int nID)
{
	if (m_CtrlQ.empty())
		return NULL;
	for (S_DWORD i = 0; i < m_CtrlQ.size(); i++)
	{
		C_HGUICTRL *pWnd = m_CtrlQ[i];
		if (pWnd->GetCtrlID() == (S_DWORD)nID)
			return pWnd;
	}
	return NULL;
}

C_HGUICTRL *C_HGUIWND::GetHittedCtrl(S_WORD x, S_WORD y)
{
	if (m_CtrlQ.empty())
		return NULL;
	for (vector<C_HGUICTRL*>::iterator iter = m_CtrlQ.begin(); iter != m_CtrlQ.end(); iter++)
	{
		if ((*iter)->WndVisible() == false || (*iter)->WndGrayed() == true)
			continue;
		C_HGUIRECT Rect((*iter)->GetWndRect());
		if (Rect.PointInRect(x, y) == true)
			return *iter;
	}
	return NULL;
}

bool C_HGUIWND::DeleteAutoReleaseControl(void)
{
	if (m_CtrlQ.empty())
		return false;

	vector<C_HGUICTRL*> ReleaseCtrlQ;
	vector<C_HGUICTRL*> AllCtrlQ(m_CtrlQ);

	m_CtrlQ.clear();
	for (S_DWORD i = 0; i < AllCtrlQ.size(); i++)
	{
		C_HGUICTRL *pWnd = AllCtrlQ[i];
		pWnd->Release();
		if (pWnd->GetReferenceCount() == 0)
			ReleaseCtrlQ.push_back(pWnd);
		else
			m_CtrlQ.push_back(pWnd);
	}
	for (S_DWORD i = 0; i < ReleaseCtrlQ.size(); i++)
	{
		C_HGUICTRL *pWnd = ReleaseCtrlQ[i];
		if (GetCapture() == pWnd)
			SetCapture(NULL);
		delete pWnd;
	}

	return true;
}

C_HGUICTRL *C_HGUIWND::RemoveControl(int nID)
{
	if (m_CtrlQ.empty())
		return NULL;
	for (S_DWORD i = 0; i < m_CtrlQ.size(); i++)
	{
		C_HGUICTRL *pWnd = m_CtrlQ[i];
		if (pWnd->GetCtrlID() == (S_DWORD)nID)
		{
			if (GetCapture() == pWnd)
				SetCapture(NULL);
			m_CtrlQ.erase(m_CtrlQ.begin() + i); // 从链表里移除
			return pWnd;
		}
	}
	return NULL;
}

S_WORD C_HGUIWND::GetLastCtrlGroup(void)
{
	if (m_CtrlQ.empty())
		return 0x00;

	size_t i = m_CtrlQ.size();
	return m_CtrlQ[i-1]->GetGroup();
}

bool C_HGUIWND::_SetFocusCtrl(C_HGUICTRL *pFocusCtrl)
{
	C_HGUICTRL *pCurFocus = GetFocusCtrl();
	if (pCurFocus == pFocusCtrl)
		return false;

	if (pCurFocus != NULL)
		pCurFocus->SendWndEvt(EVT_KILLFOCUS, 0x00, 0x00);
	else if (WndFocus() == true)
		SendWndEvt(EVT_KILLFOCUS, 0x00, 0x00);

	if (pFocusCtrl != NULL)
		pFocusCtrl->SendWndEvt(EVT_SETFOCUS, 0x00, 0x00);
	else
		SendWndEvt(EVT_SETFOCUS, 0x00, 0x00);

	return true;
}

int C_HGUIWND::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return DefWndProcess(evt, wParam, lParam);
}

int C_HGUIWND::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	switch (evt)
	{
	case EVT_QUIT:
		SGuiApp->PopWnd();
		break;
	case EVT_CTRLFOCUS:
		_SetFocusCtrl(GetWndCtrl(wParam));
		break;
	default:
		return C_GUIWNDB::DefWndProcess(evt, wParam, lParam);
	}
	return 0;
}




