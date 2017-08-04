//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguidlog.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-06-06  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_DLOG_H__
#define __HGUI_DLOG_H__
#include "hguiwnd.h"
#include "hstrings.h"

#define EVT_TIME_INTERVAL_MS		1000

class C_HGUIDLG:public C_HGUIWND
{
public:
	C_HGUIDLG(void);
	virtual ~C_HGUIDLG();

public:
	int DoModel(void);

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

	int EndDlg(void);

private:
	bool m_DlgRun;
};

#define MB_TITLE_MASK			0x80000000UL
#define MB_TITLE_CENTER		0x00000000UL
#define MB_TITLE_LEFT			0x80000000UL

#define MB_TEXT_MASK			0x40000000UL
#define MB_TEXT_CENTER			0x40000000UL
#define MB_TEXT_LEFT			0x00000000UL

#define MB_BTN_MASK			0x03000000UL
#define MB_BTN_NULL				0x00000000UL
#define MB_BTN_OK				0x02000000UL
#define MB_BTN_OKCANCEL		0x03000000UL

#define MB_BTN_FOCUS_MASK		0x08000000UL
#define MB_BTN_FOCUS_YESOK	0x00000000UL
#define MB_BTN_FOCUS_NOCAN	0x08000000UL

#define MB_RTN_NULL			0
#define MB_RTN_YESOK			1
#define MB_RTN_NO				2
#define MB_RTN_CANCEL			MB_RTN_NO
#define MB_RTN_ERROR			-1

#define MSGBOX_DELAY_MAXMS	(30*1000)

class C_MSGBOX:public C_HGUIDLG
{
public:
	C_MSGBOX(const S_CHAR *ptitle, const S_CHAR *ptext, S_DWORD dwFlag);
	virtual ~C_MSGBOX();

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int HandleTextData(S_WORD &TextW, S_WORD &TextH);
	int DrawTitle(C_HGUIDC *pdc);
	int DrawText(C_HGUIDC *pdc);
	int CreateButton(void);
	int DestroyButton(void);
	
protected:
	string m_Title;
	strings m_TextData;
	S_DWORD m_DelayMS;
	static const S_WORD Btn_OK_ID = 1;
	static const S_WORD Btn_Cancel_ID = 2;
};

class C_EditBoxEx:public C_MSGBOX
{
public:
	C_EditBoxEx(const S_CHAR *ptitle, const S_CHAR *pDefault, S_CHAR *pStringData);
	virtual ~C_EditBoxEx();

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

	int CreateEditBox(void);
	int DestroyEditBox(void);

protected:
	S_CHAR *m_pStringData;
	static const S_WORD Edit_Box_ID = 3;
};

int HGui_MsgBox(const S_CHAR *ptitle, const S_CHAR *ptext, S_DWORD dwFlag);
int HGui_EditBox(const S_CHAR *ptitle, const S_CHAR *pDefault, S_CHAR *pStringData);


#endif //__HGUI_DLOG_H__

