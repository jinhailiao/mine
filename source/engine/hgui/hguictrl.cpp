//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguictrl.cpp
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-06-06  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "hguictrl.h"
#include "hguirect.h"

#define HGUI_CS_TEXT_LF_MASK		0x00000300UL
#define HGUI_CS_TEXT_TB_MASK		0x00000C00UL
#define HGUI_BS_BTN_TYPE_MASK	0x00007000UL

//
//控件父类
//
C_HGUICTRL::C_HGUICTRL(void)
{
	m_ID = 0;
	m_group = 0;
}

C_HGUICTRL::~C_HGUICTRL()
{
}

bool C_HGUICTRL::AddCtrl2ParentWnd(C_HGUIWND *pWnd)
{
	if (pWnd == NULL)
		return false;
	return pWnd->AddControl(this);
}

S_WORD C_HGUICTRL::GetLastCtrlGroup(C_HGUIWND *pWnd)
{
	if (pWnd == NULL)
		return 0x00;
	return pWnd->GetLastCtrlGroup();
}

S_WORD C_HGUICTRL::DrawText(C_HGUIDC &dc, const string &strText)
{
	return 0;
}

int C_HGUICTRL::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_GUIWNDB::WndProcess(evt, wParam, lParam);
}

int C_HGUICTRL::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_GUIWNDB::DefWndProcess(evt, wParam, lParam);
}

//
//数字编辑框，支持八进制、十进制、十六进制
//响应EVT_KEYUP（VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT）消息
//
C_NUMEDIT::C_NUMEDIT(void)
{
}
C_NUMEDIT::~C_NUMEDIT()
{
}

C_NUMEDIT *C_NUMEDIT::NewCtrl(void)
{
	C_NUMEDIT *pCtrl = new C_NUMEDIT;
	if (pCtrl == NULL)
		return NULL;

	pCtrl->AutoRelease();
	pCtrl->Retain();
	return pCtrl;
}

bool C_NUMEDIT::Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndText = pszCaption;
	m_WndRect = rect;
	m_pParent = pParentWnd;
	m_flag = dwStyle | HGUI_WNDF_VISIBLE;
	m_ID = nID;

	for (size_t i = 0; i < m_WndText.size(); i++)
	{
		if (m_WndText[i] >= 'a' && m_WndText[i] <= 'f')
			m_WndText[i] -= 'a' - 'A';
		if ((GetCtrlStyle()&HGUI_NUMEDIT_STYLE_OCT)==HGUI_NUMEDIT_STYLE_OCT)
		{
			if (!IsOctChar(m_WndText[i])) m_WndText[i] = '0';
		}
		else if ((GetCtrlStyle()&HGUI_NUMEDIT_STYLE_HEX)==HGUI_NUMEDIT_STYLE_HEX)
		{
			if (!IsHexChar(m_WndText[i])) m_WndText[i] = '0';
		}
		else
		{
			if (!IsDecChar(m_WndText[i])) m_WndText[i] = '0';
		}
	}
	if (m_WndText.empty())
		m_WndText.push_back('0');

	SendWndEvt(EVT_CREATE, 0x00, 0x00);
	return true;
}

int C_NUMEDIT::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (evt)
	{
	case EVT_CREATE:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		CreateCaret(pdc->GetFontWidth('0'), 1);
		SetCaretPos(m_SideW+0*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
		ShowCaret();
		delete pdc;
		}
		break;
	case EVT_PAINT:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
		delete pdc;
		}
		break;
	case EVT_KEYUP:{
		S_WORD x, y;
		C_HGUIDC *pdc = new C_HGUIDC(this);
		GetCaretPos(x, y);
		x = (x - 2) / pdc->GetFontWidth('0');
		if (wParam == VK_UP)
		{
			m_WndText[x] = IncreaseChar(m_WndText[x]);
			pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
			m_pParent->SendWndEvt(EVT_COMMAND, (S_WORD)m_ID, HGUI_EVT_CMD_CTRL_UPDATE);
		}
		else if(wParam == VK_DOWN)
		{
			m_WndText[x] = DecreaseChar(m_WndText[x]);
			pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
			m_pParent->SendWndEvt(EVT_COMMAND, (S_WORD)m_ID, HGUI_EVT_CMD_CTRL_UPDATE);
		}
		else if (wParam == VK_LEFT)
		{
			if (x == 0)
				ok = m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
			else
				SetCaretPos(m_SideW + (x-1) * pdc->GetFontWidth('0'), y);
		}
		else if (wParam == VK_RIGHT)
		{
			if (x == m_WndText.size() - 1)
				ok = m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
			else
				SetCaretPos(m_SideW + (x+1) * pdc->GetFontWidth('0'), y);
		}
		else
		{
			ok = m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
		}
		delete pdc;}
		break;
	default:
		return C_HGUICTRL::WndProcess(evt, wParam, lParam);
	}
	return ok;
}

int C_NUMEDIT::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_HGUICTRL::DefWndProcess(evt, wParam, lParam);
}

S_CHAR C_NUMEDIT::IncreaseChar(S_CHAR ch)
{
	if ((GetCtrlStyle()&HGUI_NUMEDIT_STYLE_OCT)==HGUI_NUMEDIT_STYLE_OCT)
	{
		return (ch >= '7')? '0':(ch+1);
	}
	else if ((GetCtrlStyle()&HGUI_NUMEDIT_STYLE_HEX)==HGUI_NUMEDIT_STYLE_HEX)
	{
		if (ch == '9') return 'A';
		else if (ch == 'F' || ch == 'f') return '0';
		else return ch+1;
	}

	return (ch >= '9')? '0':(ch+1);
}

S_CHAR C_NUMEDIT::DecreaseChar(S_CHAR ch)
{
	if ((GetCtrlStyle()&HGUI_NUMEDIT_STYLE_OCT)==HGUI_NUMEDIT_STYLE_OCT)
	{
		return (ch == '0')? '7':(ch-1);
	}
	else if ((GetCtrlStyle()&HGUI_NUMEDIT_STYLE_HEX)==HGUI_NUMEDIT_STYLE_HEX)
	{
		if (ch == '0') return 'F';
		else if (ch == 'A' || ch == 'a') return '9';
		else return ch-1;
	}

	return (ch == '0')? '9':(ch-1);
}


//
//简易ASCII码编辑框
//响应EVT_KEYUP（VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT）消息
//
C_ASCEDIT::C_ASCEDIT(void)
{
}
C_ASCEDIT::~C_ASCEDIT()
{
}

C_ASCEDIT *C_ASCEDIT::NewCtrl(void)
{
	C_ASCEDIT *pCtrl = new C_ASCEDIT;
	if (pCtrl == NULL)
		return NULL;

	pCtrl->AutoRelease();
	pCtrl->Retain();
	return pCtrl;
}

bool C_ASCEDIT::Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndText = pszCaption;
	m_WndRect = rect;
	m_pParent = pParentWnd;
	m_flag = dwStyle | HGUI_WNDF_VISIBLE;
	m_ID = nID;

	for (size_t i = 0; i < m_WndText.size(); i++)
	{
		if (!(m_WndText[i] >= ' ' && m_WndText[i] <= '~'))
			m_WndText[i] = ' ';
	}
	if (m_WndText.empty())
		m_WndText.push_back('0');

	SendWndEvt(EVT_CREATE, 0x00, 0x00);
	return true;
}

int C_ASCEDIT::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (evt)
	{
	case EVT_CREATE:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		CreateCaret(pdc->GetFontWidth('0'), 1);
		SetCaretPos(m_SideW+0*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
		ShowCaret();
		delete pdc;
		}
		break;
	case EVT_PAINT:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
		delete pdc;
		}
		break;
	case EVT_KEYUP:{
		S_WORD x, y;
		C_HGUIDC *pdc = new C_HGUIDC(this);
		GetCaretPos(x, y);
		x = (x - 2) / pdc->GetFontWidth('0');
		if (wParam == VK_UP)
		{
			m_WndText[x] = IncreaseChar(m_WndText[x]);
			pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
			m_pParent->SendWndEvt(EVT_COMMAND, (S_WORD)m_ID, HGUI_EVT_CMD_CTRL_UPDATE);
		}
		else if(wParam == VK_DOWN)
		{
			m_WndText[x] = DecreaseChar(m_WndText[x]);
			pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
			m_pParent->SendWndEvt(EVT_COMMAND, (S_WORD)m_ID, HGUI_EVT_CMD_CTRL_UPDATE);
		}
		else if (wParam == VK_LEFT)
		{
			if (x == 0)
				ok = m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
			else
				SetCaretPos(m_SideW + (x-1) * pdc->GetFontWidth('0'), y);
		}
		else if (wParam == VK_RIGHT)
		{
			if (x == m_WndText.size() - 1)
				ok = m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
			else
				SetCaretPos(m_SideW + (x+1) * pdc->GetFontWidth('0'), y);
		}
		else
		{
			ok = m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
		}
		delete pdc;}
		break;
	default:
		return C_HGUICTRL::WndProcess(evt, wParam, lParam);
	}
	return ok;
}

int C_ASCEDIT::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_HGUICTRL::DefWndProcess(evt, wParam, lParam);
}

S_CHAR C_ASCEDIT::IncreaseChar(S_CHAR ch)
{
	return (ch >= '~')? ' ':(ch+1);
}

S_CHAR C_ASCEDIT::DecreaseChar(S_CHAR ch)
{
	return (ch <= ' ')? '~':(ch-1);
}


//
//虚拟键盘控件
//响应EVT_KEYUP（VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT,VK_RETURN）消息
//
const S_CHAR *C_VKBOARD::m_VkAsciiSet[5] =
{
	"ABCDEFGHIJKLMNOPQRS",
	"TUVWXYZabcdefghijkl",
	"mnopqrstuvwxyz@#$%&",
	"1234567890 <>[](){}",
	"*^~_+-=|!:;,.?'\"\\ /"
};
const S_VKBTN C_VKBOARD::m_VkSpecialKey[5] =
{
	{"ESC", VK_ESCAPE}, {"Del", VK_BACK}, {"Ent", VK_RETURN}, {"Tab", VK_TAB}, {"F1 ", VK_F1}
};

C_VKBOARD::C_VKBOARD(void)
{
	m_RowCharCnt = 0;
	m_RowIdx = 0;
	m_ColIdx = 0;
}

C_VKBOARD::~C_VKBOARD()
{
}

C_VKBOARD *C_VKBOARD::NewCtrl(void)
{
	C_VKBOARD *pCtrl = new C_VKBOARD;
	if (pCtrl == NULL)
		return NULL;

	pCtrl->AutoRelease();
	pCtrl->Retain();
	return pCtrl;
}

bool C_VKBOARD::Create(S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndRect = rect;
	m_WndRect.w = m_VkBoardW;
	m_WndRect.h = m_VkBoardH;
	m_pParent = pParentWnd;
	m_flag = dwStyle | HGUI_WNDF_VISIBLE;
	m_ID = nID;

	SendWndEvt(EVT_CREATE, 0x00, 0x00);
	return true;
}

int C_VKBOARD::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (evt)
	{
	case EVT_CREATE:
		m_RowIdx = 0;
		m_ColIdx = 0;
		m_RowCharCnt = (int)strlen(m_VkAsciiSet[0]);
		break;
	case EVT_PAINT:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		for (int i = 0; i < 5; i++)
			pdc->DrawString(2, 2+i*pdc->GetFontHeight('0'), m_VkAsciiSet[i]);
		for (int i = 0; i < 5; i++)
			pdc->DrawString(2+3+m_RowCharCnt*pdc->GetFontWidth('0'), 2+i*pdc->GetFontHeight('0'), m_VkSpecialKey[i].m_pName);
		pdc->DrawVLine(2+1+m_RowCharCnt*pdc->GetFontWidth('0'), 1+0*pdc->GetFontHeight('0'), 3+5*pdc->GetFontHeight('0'));
		for (int i = 1; i < 5; i++)
			pdc->DrawHLine(1+3+m_RowCharCnt*pdc->GetFontWidth('0'), 2+i*pdc->GetFontHeight('0'), 2+3*pdc->GetFontWidth('0'));
		if ((GetWndLong()&HGUI_WNDF_FOCUS) == HGUI_WNDF_FOCUS)
		{
			GetHighlightRect(WndRect, pdc);
			pdc->ReverseRect(WndRect);
		}
		delete pdc;
		}
		break;
	case EVT_KEYUP:
		if (wParam == VK_DOWN || wParam == VK_UP || wParam == VK_LEFT || wParam == VK_RIGHT)
		{
			S_RECT Rect;
			C_HGUIDC *pdc = new C_HGUIDC(this);
			if ((GetWndLong()&HGUI_WNDF_FOCUS) == HGUI_WNDF_FOCUS)
			{
				GetHighlightRect(Rect, pdc);
				pdc->ReverseRect(Rect);
			}
			if (wParam == VK_DOWN)
				m_ColIdx = (m_ColIdx>=4)?0:m_ColIdx+1;
			else if (wParam == VK_UP)
				m_ColIdx = (m_ColIdx<=0)?4:m_ColIdx-1;
			else if (wParam == VK_LEFT)
				m_RowIdx = (m_RowIdx<=0)?m_RowCharCnt:m_RowIdx-1;
			else if (wParam == VK_RIGHT)
				m_RowIdx = (m_RowIdx>=m_RowCharCnt)?0:m_RowIdx+1;
			if ((GetWndLong()&HGUI_WNDF_FOCUS) == HGUI_WNDF_FOCUS)
			{
				GetHighlightRect(Rect, pdc);
				pdc->ReverseRect(Rect);
			}
			delete pdc;
		}
		else if (wParam == VK_RETURN)
		{
			ok = m_pParent->SendWndEvt(EVT_CHAR, GetHighlightChar(), lParam);
		}
		else
		{
			ok = m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
		}
		break;
	default:
		return C_HGUICTRL::WndProcess(evt, wParam, lParam);
	}
	return ok;
}

int C_VKBOARD::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_HGUICTRL::DefWndProcess(evt, wParam, lParam);
}

int C_VKBOARD::GetHighlightRect(S_RECT &rect, const C_HGUIDC *pdc)
{
	if (m_RowIdx == (m_RowCharCnt + 1) - 1)
	{
		rect.x = 2+3+m_RowCharCnt*pdc->GetFontWidth('0');
		rect.y = 3+m_ColIdx*pdc->GetFontHeight('0');
		rect.w = 3*pdc->GetFontWidth('0');
		rect.h = pdc->GetFontHeight('0')-1;
	}
	else
	{
		rect.x = 2+m_RowIdx*pdc->GetFontWidth('0');
		rect.y = 2+m_ColIdx*pdc->GetFontHeight('0');
		rect.w = pdc->GetFontWidth('0');
		rect.h = pdc->GetFontHeight('0');
	}
	return 0;
}

S_WORD C_VKBOARD::GetHighlightChar(void)
{
	if (m_RowIdx == (m_RowCharCnt + 1) - 1)
		return (S_WORD)m_VkSpecialKey[m_ColIdx].m_Val;
	return (S_WORD)m_VkAsciiSet[m_ColIdx][m_RowIdx];
}

//
//文本编辑框
//响应EVT_KEYUP（VK_LEFT,VK_RIGHT）消息
//
C_TEXTEDIT::C_TEXTEDIT(void)
{
	m_CaretIdx = 0;
}

C_TEXTEDIT::~C_TEXTEDIT()
{
}

C_TEXTEDIT *C_TEXTEDIT::NewCtrl(void)
{
	C_TEXTEDIT *pCtrl = new C_TEXTEDIT;
	if (pCtrl == NULL)
		return NULL;

	pCtrl->AutoRelease();
	pCtrl->Retain();
	return pCtrl;
}

bool C_TEXTEDIT::Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndText = pszCaption;
	m_WndRect = rect;
	m_pParent = pParentWnd;
	m_flag = dwStyle | HGUI_WNDF_VISIBLE;
	m_ID = nID;

	if (m_WndText.empty())
		m_WndText.append("  ");
	SendWndEvt(EVT_CREATE, 0x00, 0x00);
	return true;
}

int C_TEXTEDIT::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (evt)
	{
	case EVT_CREATE:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		m_CaretIdx = 0;
		CreateCaret(pdc->GetFontWidth('0'), 1);
		SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
		ShowCaret();
		delete pdc;
		}
		break;
	case EVT_PAINT:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
		delete pdc;
		}
		break;
	case EVT_KEYUP:
		if (wParam == VK_LEFT)
		{
			C_HGUIDC *pdc = new C_HGUIDC(this);
			if (m_CaretIdx != 0)
			{
				m_CaretIdx -= 1;
				SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
			}
			delete pdc;
		}
		else if (wParam == VK_RIGHT)
		{
			C_HGUIDC *pdc = new C_HGUIDC(this);
			if (m_CaretIdx < (int)m_WndText.size())
			{
				m_CaretIdx += 1;
				SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
			}
			delete pdc;
		}
		else
		{
			ok = m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
		}
		break;
	case EVT_CHAR:
		if (wParam == VK_BACK)
		{
			C_HGUIDC *pdc = new C_HGUIDC(this);
			if (m_CaretIdx != 0)
			{
				size_t i = m_CaretIdx - 1;
				for (; i < m_WndText.size()-1; i++)
					m_WndText[i] = m_WndText[i+1];
				m_WndText[i] = ' ';
				pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
				m_CaretIdx -= 1;
				SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
				m_pParent->SendWndEvt(EVT_COMMAND, (S_WORD)m_ID, HGUI_EVT_CMD_CTRL_UPDATE);
			}
			delete pdc;
		}
		else if (wParam == VK_RETURN || wParam == VK_ESCAPE || wParam == VK_F1)
		{
			ok = m_pParent->SendWndEvt(EVT_CHAR, wParam, lParam);
		}
		else
		{
			if (wParam == VK_TAB) wParam = ' ';
			C_HGUIDC *pdc = new C_HGUIDC(this);
			if (m_CaretIdx < (int)m_WndText.size() )
			{
				m_WndText[m_CaretIdx] = (S_CHAR)wParam;
				pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
				m_CaretIdx += 1;
				SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
				m_pParent->SendWndEvt(EVT_COMMAND, (S_WORD)m_ID, HGUI_EVT_CMD_CTRL_UPDATE);
			}
			delete pdc;
		}
		break;
	default:
		return C_HGUICTRL::WndProcess(evt, wParam, lParam);
	}
	return ok;
}

int C_TEXTEDIT::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_HGUICTRL::DefWndProcess(evt, wParam, lParam);
}


//
//按钮控件
//响应EVT_KEYUP（VK_RETURN）消息
//
C_BUTTON::C_BUTTON(void)
{
	m_state = 0;
}

C_BUTTON::~C_BUTTON()
{
}

C_BUTTON *C_BUTTON::NewCtrl(void)
{
	C_BUTTON *pCtrl = new C_BUTTON;
	if (pCtrl == NULL)
		return NULL;

	pCtrl->AutoRelease();
	pCtrl->Retain();
	return pCtrl;
}

bool C_BUTTON::Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndText = pszCaption;
	m_WndRect = rect;
	m_pParent = pParentWnd;
	m_flag = dwStyle | HGUI_WNDF_VISIBLE;
	m_ID = nID;

	SendWndEvt(EVT_CREATE, 0x00, 0x00);
	return true;
}

int C_BUTTON::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (evt)
	{
	case EVT_CREATE:
		break;
	case EVT_PAINT:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->SelectObject(pdc->GetStockGuiObj(HGUI_OBJ_WPEN));
		pdc->FillRect(WndRect);
		pdc->SelectObject(pdc->GetStockGuiObj(HGUI_OBJ_BPEN));
		pdc->DrawRect(WndRect);
		pdc->DrawString(4, 2, m_WndText.c_str());
		if ((GetWndLong()&HGUI_WNDF_FOCUS) == HGUI_WNDF_FOCUS)
		{
			WndRect.x = 1, WndRect.y = 1;
			WndRect.w = WndRect.w - 2, WndRect.h = WndRect.h - 2;
			pdc->ReverseRect(WndRect);
		}
		delete pdc;
		}
		break;
	case EVT_KEYUP:
		if (wParam == VK_RETURN)
		{
			ok = m_pParent->SendWndEvt(EVT_COMMAND, (S_WORD)m_ID, HGUI_EVT_CMD_BTN_PUSHED);
		}
		else
		{
			ok = m_pParent->SendWndEvt(evt, wParam, lParam);
		}
		break;
	default:
		return DefWndProcess(evt, wParam, lParam);
	}
	return ok;
}

int C_BUTTON::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_HGUICTRL::DefWndProcess(evt, wParam, lParam);
}

//
//选择框
//响应EVT_KEYUP（VK_UP,VK_DOWN,VK_RETURN）消息
//选择文本用;间隔
//
C_SELECTBOX::C_SELECTBOX(void)
{
	m_Index = 0;
}
C_SELECTBOX::~C_SELECTBOX()
{
}

C_SELECTBOX *C_SELECTBOX::NewCtrl(void)
{
	C_SELECTBOX *pCtrl = new C_SELECTBOX;
	if (pCtrl == NULL)
		return NULL;

	pCtrl->AutoRelease();
	pCtrl->Retain();
	return pCtrl;
}

bool C_SELECTBOX::Create(const S_CHAR *pszSelectText, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID)
{
	m_WndRect = rect;
	m_WndRect.h = m_Direction_High;//固定高度为16（SYM16X16符号高度）
	m_pParent = pParentWnd;
	m_flag = dwStyle | HGUI_WNDF_VISIBLE;
	m_ID = nID;
	m_Index = 0;

	//字符串长度对齐
	size_t Length = 0;
	strings SelectText;
	SelectText.split(pszSelectText, ';');
	for (S_WORD i = 0; i < SelectText.size(); i++)
	{
		if (Length < SelectText[i].size())
			Length = SelectText[i].size();
	}
	for (S_WORD i = 0; i < SelectText.size(); i++)
	{
		string OneText(SelectText[i]);
		if (OneText.size() < Length)
			OneText.append(Length-OneText.size(), ' ');
		m_SelectText.push_back(OneText);
	}
	
	if (m_SelectText.size() < 2)//最少需要两项供选择
		return false;

	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	SendWndEvt(EVT_CREATE, 0x00, 0x00);
	return true;
}

/** @brief 设置缺省项
 * @param[in] idx 索引值 [0 - (项总数-1)]
 * @return true 设置成功; false 设置失败
 */
bool C_SELECTBOX::SetDefaultItem(S_DWORD idx)
{
	if (idx < m_SelectText.size())
	{
		m_Index = idx;
		m_WndText = m_SelectText[m_Index];
		return true;
	}
	return false;
}

int C_SELECTBOX::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (evt)
	{
	case EVT_CREATE:
		m_WndText = m_SelectText[m_Index];
		break;
	case EVT_PAINT:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawString(m_Select_SideW, m_Select_SideH, m_SelectText[m_Index].c_str());
		DrawFocusDirection(pdc);
		delete pdc;
		}
		break;
	case EVT_COMMAND:
		if (wParam ==0x00) SetDefaultItem(lParam);
		break;
	case EVT_KEYUP:{
		C_HGUIDC *pdc = new C_HGUIDC(this);
		if (wParam == VK_UP)
		{
			if (m_Index > 0)
			{
				m_Index -= 1;
				pdc->DrawString(m_Select_SideW, m_Select_SideH, m_SelectText[m_Index].c_str());
				DrawFocusDirection(pdc);
				m_WndText = m_SelectText[m_Index];
				m_pParent->SendWndEvt(EVT_COMMAND, (S_WORD)m_ID, HGUI_EVT_CMD_CTRL_UPDATE);
			}
		}
		else if(wParam == VK_DOWN)
		{
			if (m_Index < m_SelectText.size() - 1)
			{
				m_Index += 1;
				pdc->DrawString(m_Select_SideW, m_Select_SideH, m_SelectText[m_Index].c_str());
				DrawFocusDirection(pdc);
				m_WndText = m_SelectText[m_Index];
				m_pParent->SendWndEvt(EVT_COMMAND, (S_WORD)m_ID, HGUI_EVT_CMD_CTRL_UPDATE);
			}
		}
		else
		{
			ok = m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
		}
		delete pdc;
		}
		break;
	default:
		return C_HGUICTRL::WndProcess(evt, wParam, lParam);
	}
	return ok;
}

int C_SELECTBOX::DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return C_HGUICTRL::DefWndProcess(evt, wParam, lParam);
}

int C_SELECTBOX::DrawFocusDirection(C_HGUIDC *pdc)
{
	S_WORD x = 0;
	static const char * const FocusDirection[] = {"P","Q","R","S"};
	
	if (m_WndRect.w > m_Direction_Width)
		x = m_WndRect.w - m_Direction_Width;

	pdc->SelectObject(pdc->GetStockGuiObj(HGUI_OBJ_SYM16x16));
	if ((GetWndLong()&HGUI_WNDF_FOCUS) == HGUI_WNDF_FOCUS)
	{
		if (m_Index == 0)
			pdc->DrawString(x, 0, FocusDirection[2]);
		else if(m_Index >= m_SelectText.size() - 1)
			pdc->DrawString(x, 0, FocusDirection[1]);
		else
			pdc->DrawString(x, 0, FocusDirection[0]);
	}
	else
	{
		pdc->DrawString(x, 0, FocusDirection[3]);
	}
	return 0;
}

C_ButtonEx::C_ButtonEx(void)
{
	m_state = 0;
}

C_ButtonEx::~C_ButtonEx()
{
}

C_ButtonEx *C_ButtonEx::NewCtrl(void)
{
	C_ButtonEx *pCtrl = new C_ButtonEx;
	if (pCtrl == NULL)
		return NULL;

	pCtrl->AutoRelease();
	pCtrl->Retain();
	return pCtrl;
}

bool C_ButtonEx::Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, 
	const char *pUpPic, const char *pDnPic, const char *pGrayPic, S_WORD nID)
{
	S_WORD group = GetLastCtrlGroup(pParentWnd);
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	if (pszCaption != NULL)
		m_WndText = pszCaption;
	m_WndRect = rect;
	m_pParent = pParentWnd;
	m_flag = dwStyle | HGUI_WNDF_VISIBLE;
	m_ID = nID;

	// handle pic
	if (pUpPic != NULL)
		m_strUpPic.append(pUpPic);
	if (pDnPic != NULL)
		m_strDnPic.append(pDnPic);
	if (pGrayPic != NULL)
		m_strGrayPic.append(pGrayPic);

	// handle group
	m_group = group;
	if (dwStyle & HGUI_WNDF_GROUP)
		m_group += 1;

	SendWndEvt(EVT_CREATE, 0x00, 0x00);
	return true;
}

S_WORD C_ButtonEx::DrawText(C_HGUIDC &dc, const string &strText)
{
#define CTRL_CHECKBTN_FLAG_W	12
	if (strText.empty() == true)
		return 0;

	S_WORD x = 0, y = 0;
	S_BYTE bkmode = dc.GetBkMode();
	S_DWORD textcolor = dc.GetTextColor();
	S_WORD h = dc.GetFontHeight(' ');
	S_WORD w = (S_WORD)dc.GetStringExtent(strText.c_str());
	switch (m_flag & HGUI_BS_BTN_TYPE_MASK)
	{
	case HGUI_BS_PUSHBTN:
		if ((m_flag & HGUI_BS_BTN_TYPE_MASK) == HGUI_BS_PUSHBTN)
			textcolor = (WndGrayed()?  HGUI_COLOR_GRAY15:HGUI_COLOR_GRAY00);
		if (h > m_WndRect.h)
			y = m_WndRect.y;
		else
		{
			if ((m_flag & HGUI_CS_TEXT_TB_MASK) == HGUI_CS_TEXT_TOP)
				y = m_WndRect.y;
			else if ((m_flag & HGUI_CS_TEXT_TB_MASK) == HGUI_CS_TEXT_BUTTOM)
				y = (m_WndRect.y + m_WndRect.h) - h;
			else
				y = m_WndRect.y + (m_WndRect.h - h)/2;
		}
		if (w > m_WndRect.w)
			x = m_WndRect.x;
		else
		{
			if ((m_flag & HGUI_CS_TEXT_LF_MASK) == HGUI_CS_TEXT_LEFT)
				x = m_WndRect.x;
			else if ((m_flag & HGUI_CS_TEXT_LF_MASK) == HGUI_CS_TEXT_RIGHT)
				x = (m_WndRect.x + m_WndRect.w) - w;
			else
				x = m_WndRect.x + (m_WndRect.w - w)/2;
		}
		break;
	case HGUI_BS_CHECKBTN:
	case HGUI_BS_RADIOBTN:
		if (h > m_WndRect.h)
			y = m_WndRect.y;
		else
			y = m_WndRect.y + (m_WndRect.y - h)/2;
		if (w > m_WndRect.w)
			x = m_WndRect.x;
		else
			x = m_WndRect.x + CTRL_CHECKBTN_FLAG_W+6;
		break;
	default:
		break;
	}

	bkmode = dc.SetBkMode(bkmode);
	textcolor = dc.SetTextColor(textcolor);

	S_WORD size = (S_WORD)dc.DrawString(x, y, strText.c_str());
	dc.SetBkMode(bkmode);
	dc.SetTextColor(textcolor);
	return size;
}

int C_ButtonEx::WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	switch (m_flag & HGUI_BS_BTN_TYPE_MASK)
	{
	case HGUI_BS_PUSHBTN:
		return PushBtnProcess(evt, wParam, lParam);
	case HGUI_BS_CHECKBTN:
		return CheckBtnProcess(evt, wParam, lParam);
	case HGUI_BS_RADIOBTN:
		return RadioBtnProcess(evt, wParam, lParam);
	}
	return DefWndProcess(evt, wParam, lParam);
}

int C_ButtonEx::PushBtnProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	switch (evt)
	{
	case EVT_CREATE:
		break;
	case EVT_PAINT:
		{
			if (WndVisible() == false)
				break;
			C_HGUIBMP bitmap;
			C_HGUIDC dc(m_pParent);
//			m_pParent->SendWndEvt(EVT_CTRLCOLOR, m_ID, (S_DWORD)&dc);
			if (WndGrayed() == true)
			{
				if (m_strGrayPic.empty() == false)
				{
					bitmap.LoadBitmap(m_strGrayPic);
					dc.DrawBitmap(m_WndRect.x, m_WndRect.y, &bitmap);
				}
				else
				{
//					dc.DrawBox(m_WndRect.x, m_WndRect.y, m_WndRect.w, m_WndRect.h, BLOCK_3D_UP, DKGRAY_BRUSH);
				}
			}
			else
			{
				if (m_state == HGUI_BS_PUSHBTN_UP)
				{
					if (m_strUpPic.empty() == false)
					{
						bitmap.LoadBitmap(m_strUpPic);
						dc.DrawBitmap(m_WndRect.x, m_WndRect.y, &bitmap);
					}
					else
					{
//						dc.DrawBox(m_WndRect.x, m_WndRect.y, m_WndRect.w, m_WndRect.h, BLOCK_3D_UP, GRAY_BRUSH);
					}
				}
				else if (m_state == HGUI_BS_PUSHBTN_DN)
				{
					if (m_strDnPic.empty() == false)
					{
						bitmap.LoadBitmap(m_strDnPic);
						dc.DrawBitmap(m_WndRect.x, m_WndRect.y, &bitmap);
					}
					else
					{
//						dc.DrawBox(m_WndRect.x, m_WndRect.y, m_WndRect.w, m_WndRect.h, BLOCK_3D_DN, GRAY_BRUSH);
					}
				}
			}
			DrawText(dc, m_WndText);
			UpdateWnd();
		}
		break;	
	case EVT_TIMER:
		break;	
	case EVT_KEYDN:
		if (wParam == ' ' || wParam == VK_RETURN)
		{
			m_state = HGUI_BS_PUSHBTN_DN;
			m_pParent->SendWndEvt(EVT_CTRLFOCUS, m_ID, 0);
			PushBtnProcess(EVT_PAINT, 0, 0);
			m_pParent->SendWndEvt(EVT_COMMAND, m_ID, HGUI_BS_PUSHBTN_DN);
		}
		else
			m_pParent->SendWndEvt(EVT_KEYDN, wParam, lParam);
		break;
	case EVT_KEYUP:
		if (wParam == ' ' || wParam == VK_RETURN)
		{
			m_state = HGUI_BS_PUSHBTN_UP;
			PushBtnProcess(EVT_PAINT, 0, 0);
			m_pParent->SendWndEvt(EVT_COMMAND, m_ID, m_state);
		}
		else
			m_pParent->SendWndEvt(EVT_KEYUP, wParam, lParam);
		break;
	case EVT_CHAR:
		break;
	case EVT_SYSKEY:
		break;
	case EVT_DESTROY:
		break;
	case EVT_MOUSEDN:
		m_state = HGUI_BS_PUSHBTN_DN;
		m_pParent->SetCapture(this);
		m_pParent->SendWndEvt(EVT_CTRLFOCUS, m_ID, 0);
		PushBtnProcess(EVT_PAINT, 0, 0);
		m_pParent->SendWndEvt(EVT_COMMAND, m_ID, m_state);
		break;
	case EVT_MOUSEUP:{
		S_WORD x = HAI_GETLOWORD(lParam);
		S_WORD y = HAI_GETHIWORD(lParam);
		C_HGUIRECT Rect(m_WndRect);
		m_state = HGUI_BS_PUSHBTN_UP;
		PushBtnProcess(EVT_PAINT, 0, 0);
		m_pParent->SetCapture(NULL);
		if (Rect.PointInRect(x, y) == true)
			m_pParent->SendWndEvt(EVT_COMMAND, m_ID, m_state);
		}break;
	case EVT_MOUSEDB:
		m_pParent->SetCapture(NULL);
		break;
	case EVT_MOUSEMV:
		break;
	case EVT_RESTORE:
		break;
	case EVT_COMMAND:
		break;
	default:
		return DefWndProcess(evt, wParam, lParam);
	}
	return 0;
}

int C_ButtonEx::CheckBtnProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return DefWndProcess(evt, wParam, lParam);
}

int C_ButtonEx::RadioBtnProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam)
{
	return DefWndProcess(evt, wParam, lParam);
}




