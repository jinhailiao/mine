//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguiwnd.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_WND_H__
#define __HGUI_WND_H__
#include "haibase.h"
#include "hguiobj.h"
#include "hguievt.h"
#include "hguidc.h"

#define HGUI_WNDF_VISIBLE		0x00000001UL
#define HGUI_WNDF_DISABLED		0x00000002UL
#define HGUI_WNDF_FOCUS			0x00000004UL
#define HGUI_WNDF_GRAYED			HGUI_WNDF_DISABLED
#define HGUI_WNDF_GROUP			0x00000008UL

class C_WNDBASE:public C_HGUIOBJ
{
public:
	C_WNDBASE(void);
	virtual ~C_WNDBASE();

public:
	int SendWndEvt(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	int PostWndEvt(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	S_RECT GetWndRect(void){return m_WndRect;}
	void SetWndRect(const S_RECT &rRect){m_WndRect = rRect;}
	S_DWORD SetWndLong(S_DWORD flag){S_DWORD old = m_flag; m_flag = flag; return old;}
	S_DWORD GetWndLong(void){return m_flag;}

	void ShowWnd(bool bShow = true);
	void EnableWnd(bool bEnable = true);

	bool InvalidRect(void){return m_InvalidRect.w && m_InvalidRect.h;}
	bool WndVisible(void){return (m_flag&HGUI_WNDF_VISIBLE) == HGUI_WNDF_VISIBLE;}
	bool WndFocus(void){return (m_flag&HGUI_WNDF_FOCUS) == HGUI_WNDF_FOCUS;}
	bool WndDisabled(void){return (m_flag&HGUI_WNDF_DISABLED) == HGUI_WNDF_DISABLED;}
	bool WndGrayed(void){return WndDisabled();}

	virtual bool SetVisible(bool fVisible);
	virtual bool SetFocus(bool fFocus);
	virtual bool SetDisabled(bool fDisable);

	bool SetGrayed(bool fGrayed){return SetDisabled(fGrayed);}
	C_WNDBASE *SetCapture(C_WNDBASE *pWnd){C_WNDBASE *pOld=m_pCapture;m_pCapture=pWnd;return pOld;}
	C_WNDBASE *GetCapture(void){return m_pCapture;}
	int ClientToScreen(S_RECT &rect);
	void UpdateWnd(void);

protected:
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_flag; // byte 0 is wndflag
	S_RECT m_WndRect;
	S_RECT m_InvalidRect;
	C_WNDBASE *m_pParent;
	C_WNDBASE *m_pCapture;
};

class C_HGUICARET;
class C_GUIWNDB:public C_WNDBASE
{
public:
	C_GUIWNDB(void);
	virtual ~C_GUIWNDB();

public:
	int SetWndText(const string &rWndText){m_WndText = rWndText;return 0;}
	const string &GetWndText(void){return m_WndText;}

	int CreateCaret(S_WORD w, S_WORD h);
	int SetCaretPos(S_WORD x, S_WORD y);
	int GetCaretPos(S_WORD &x, S_WORD &y);
	int ShowCaret(void);
	int HideCaret(void);

protected:
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

protected:
	string m_WndText;
	C_HGUICARET *m_pCurCaret;
};

class C_HGUICTRL;
class C_HGUIWND:public C_GUIWNDB
{
public:
	C_HGUIWND(void);
	virtual ~C_HGUIWND();

	C_HGUIDC *BeginPaint(void);
	void EndPaint(C_HGUIDC *pDC);

	/** @brief 用户数据区申请
	  *  @param[in] 数据区大小,单位字节
	  *  @return 数据区指针
	  *  @remarks 用来代替malloc和new，便于查找内存泄漏
	  */
	void *GetWndData(int size);
	void ReleaseWndData(void);
	void *GetFatherWndData(void);
	void SetFatherWndData(void *pFatherWndData);

	int InvalidateRect(const S_RECT *pRect);
	S_WORD GetFocusCtrlID(void);
	C_HGUICTRL *GetFocusCtrl(void);
	C_HGUICTRL *SetFocusCtrl(C_HGUICTRL *pFocusCtrl);
	C_HGUICTRL *GetWndCtrl(int nID);
	C_HGUICTRL *GetHittedCtrl(S_WORD x, S_WORD y);

	bool AddControl(C_HGUICTRL *pCtrl){m_CtrlQ.push_back(pCtrl);return true;}
	bool DeleteAutoReleaseControl(void);
	C_HGUICTRL *RemoveControl(int nID);
	S_WORD GetLastCtrlGroup(void);

protected:
	bool _SetFocusCtrl(C_HGUICTRL *pFocusCtrl);
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

protected:
	char *m_pWndData;
	void *m_pFatherWndData;
	vector<C_HGUICTRL*> m_CtrlQ;

private:
	bool m_ScrnFlushEn;
};

#endif //__HGUI_WND_H__

