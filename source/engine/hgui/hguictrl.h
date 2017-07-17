//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguictrl.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-06-06  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_CTRL_H__
#define __HGUI_CTRL_H__
#include "hguiwnd.h"
#include "hstrings.h"

//ctrl comm style
//
#define HGUI_CS_TEXT_CENTER		0x00000000UL
#define HGUI_CS_TEXT_LEFT			0x00000100UL
#define HGUI_CS_TEXT_RIGHT			0x00000200UL
#define HGUI_CS_TEXT_MIDDLE		0x00000000UL
#define HGUI_CS_TEXT_TOP			0x00000400UL
#define HGUI_CS_TEXT_BUTTOM		0x00000800UL
//#define HGUI_CS_TEXT_LF_MASK		0x00000300UL
//#define HGUI_CS_TEXT_TB_MASK		0x00000C00UL
//button style
//
#define HGUI_BS_PUSHBTN			0x00000000UL
#define HGUI_BS_CHECKBTN			0x00001000UL
#define HGUI_BS_RADIOBTN			0x00002000UL
//#define HGUI_BS_BTN_TYPE_MASK	0x00007000UL
//button state
//
#define HGUI_BS_PUSHBTN_UP		0x00000000
#define HGUI_BS_PUSHBTN_DN		0x00000001
#define HGUI_BS_CHECKBTN_NOCHECK		0x00000000
#define HGUI_BS_CHECKBTN_CHECKED		0x00000001

// numedit
//
#define HGUI_NUMEDIT_STYLE_DEC		0x00000000UL
#define HGUI_NUMEDIT_STYLE_OCT		0x00000100UL
#define HGUI_NUMEDIT_STYLE_HEX		0x00000200UL
// evt
//
#define HGUI_EVT_CMD_BTN_PUSHED	0x00000000UL
#define HGUI_EVT_CMD_CTRL_UPDATE	0x00000000UL
//
//�ؼ�����
//
class C_HGUICTRL:public C_GUIWNDB
{
public:
	C_HGUICTRL(void);
	virtual ~C_HGUICTRL();

public:
	S_DWORD GetCtrlStyle(void){return m_flag;}
	S_WORD GetCtrlID(void){return m_ID;}
	S_WORD GetGroup(void){return m_group;}

protected:
	bool AddCtrl2ParentWnd(C_HGUIWND *pWnd);
	S_WORD GetLastCtrlGroup(C_HGUIWND *pWnd);
	virtual S_WORD DrawText(C_HGUIDC &dc, const string &strText);

	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

	C_HGUIDC *BeginPaint(void);
	void EndPaint(C_HGUIDC *pDC);

protected:
	S_WORD m_ID;
	S_WORD m_group;

private:
	bool m_ScrnFlushEn;
};

//
//���ֱ༭��֧�ְ˽��ơ�ʮ���ơ�ʮ������
//��ӦEVT_KEYUP��VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT����Ϣ
//�û�����EVT_CMDҪ��
//EVT_CMD��Ϣ����:wParam=�ؼ�ID,lParam=EVT_CMD_CTRL_UPDATE(��ʾ�������б仯)
//
class C_NUMEDIT:public C_HGUICTRL
{
public:
	C_NUMEDIT(void);
	virtual ~C_NUMEDIT();

public:
	static C_NUMEDIT *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID); 

protected:
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

	bool IsOctChar(S_CHAR ch){return ch>='0'&&ch<='7';}
	bool IsDecChar(S_CHAR ch){return ch>='0'&&ch<='9';}
	bool IsHexChar(S_CHAR ch){return IsDecChar(ch) || ch>='A'&&ch<='F';}

	S_CHAR IncreaseChar(S_CHAR ch);
	S_CHAR DecreaseChar(S_CHAR ch);

protected:
	static const char m_SideW = 2;
	static const char m_SideH = 1;
};

//
//����ASCII��༭��
//��ӦEVT_KEYUP��VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT����Ϣ
//�û�����EVT_CMDҪ��
//EVT_CMD��Ϣ����:wParam=�ؼ�ID,lParam=EVT_CMD_CTRL_UPDATE(��ʾ�������б仯)
//
class C_ASCEDIT:public C_HGUICTRL
{
public:
	C_ASCEDIT(void);
	virtual ~C_ASCEDIT();

public:
	static C_ASCEDIT *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID); 

protected:
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

	S_CHAR IncreaseChar(S_CHAR ch);
	S_CHAR DecreaseChar(S_CHAR ch);

protected:
	static const char m_SideW = 2;
	static const char m_SideH = 1;
};


struct S_VKBTN
{
	const S_CHAR *m_pName;
	const S_CHAR m_Val;
};
//
//������̿ؼ�
//��ӦEVT_KEYUP��VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT,VK_RETURN����Ϣ
//
class C_VKBOARD:public C_HGUICTRL
{
public:
	C_VKBOARD(void);
	virtual ~C_VKBOARD();

public:
	static C_VKBOARD *NewCtrl(void);

public:
	bool Create(S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID); 

protected:
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

protected:
	virtual int GetHighlightRect(S_RECT &rect, const C_HGUIDC *pdc);
	virtual S_WORD GetHighlightChar(void);

protected:
	int m_RowCharCnt;
	int m_RowIdx;
	int m_ColIdx;

protected:
	static const int m_VkBoardW = 139;
	static const int m_VkBoardH = 64;
	static const S_CHAR *m_VkAsciiSet[5];
	static const S_VKBTN m_VkSpecialKey[5];
};

//
//�ı��༭��
//��ӦEVT_KEYUP��VK_LEFT,VK_RIGHT����Ϣ
//�û�����EVT_CMDҪ��
//EVT_CMD��Ϣ����:wParam=�ؼ�ID,lParam=EVT_CMD_CTRL_UPDATE(��ʾ�������б仯)
//
class C_TEXTEDIT:public C_HGUICTRL
{
public:
	C_TEXTEDIT(void);
	virtual ~C_TEXTEDIT();

public:
	static C_TEXTEDIT *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID); 

protected:
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

protected:
	int m_CaretIdx;
	
protected:
	static const char m_SideW = 2;
	static const char m_SideH = 1;
};

//
//��ť�ؼ�
//��ӦEVT_KEYUP��VK_RETURN����Ϣ
//�û�����EVT_CMDҪ��
//EVT_CMD��Ϣ����:wParam=�ؼ�ID,lParam=EVT_CMD_BTN_PUSHED(��ʾ��ť������)
//
class C_BUTTON:public C_HGUICTRL
{
public:
	C_BUTTON(void);
	virtual ~C_BUTTON();

public:
	static C_BUTTON *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID); 

protected:
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_state;
};

//
//ѡ���
//��ӦEVT_KEYUP��VK_UP,VK_DOWN,VK_RETURN����Ϣ
//ѡ���ı���;���
//�û�����EVT_CMDҪ��
//EVT_CMD��Ϣ����:wParam=�ؼ�ID,lParam=EVT_CMD_CTRL_UPDATE(��ʾ�������б仯)
//lParam Ϊ����ֵΪ��ǰѡ���ȷ�ϵ���
//
class C_SELECTBOX:public C_HGUICTRL
{
public:
	C_SELECTBOX(void);
	virtual ~C_SELECTBOX();

public:
	static C_SELECTBOX *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszSelectText, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_WORD nID); 
	/** @brief ����ȱʡ��
	 * @param[in] idx ����ֵ [0-(������-1)]
	 * @return true ���óɹ�; false ����ʧ��
	 */
	bool SetDefaultItem(S_DWORD idx);

protected:
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

	int DrawFocusDirection(C_HGUIDC *pdc);

protected:
	S_DWORD m_Index;
	strings m_SelectText;
	static const char m_Select_SideW = 2;
	static const char m_Select_SideH = 2;
	static const char m_Direction_High = 16;//�̶��߶�Ϊ16��SYM16X16���Ÿ߶ȣ�
	static const char m_Direction_Width = 16;//�̶����Ϊ16��SYM16X16���Ÿ߶ȣ�
};

class C_ButtonEx:public C_HGUICTRL
{
public:
	C_ButtonEx(void);
	virtual ~C_ButtonEx();

public:
	static C_ButtonEx *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, 
		const char *pUpPic, const char *pDnPic, const char *pGrayPic, S_WORD nID); 

protected:
	virtual S_WORD DrawText(C_HGUIDC &dc, const string &strText);
	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	int PushBtnProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	int CheckBtnProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	int RadioBtnProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	int SelectBtnProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_state;
	string m_strUpPic;
	string m_strDnPic;
	string m_strGrayPic;
};

#endif //__HGUI_CTRL_H__

