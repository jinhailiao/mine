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

#define NUMEDIT_STYLE_DEC		0x00000000UL
#define NUMEDIT_STYLE_OCT		0x00000100UL
#define NUMEDIT_STYLE_HEX		0x00000200UL

#define EVT_CMD_BTN_PUSHED	0x00000000UL
#define EVT_CMD_CTRL_UPDATE	0x00000000UL
//
//�ؼ�����
//
class C_GUICTRL:public C_GUIWNDB
{
public:
	C_GUICTRL(void);
	virtual ~C_GUICTRL();

public:
	S_DWORD GetCtrlStyle(void){return m_flag;}
	S_DWORD GetCtrlID(void){return m_ID;}

protected:
	bool AddCtrl2ParentWnd(C_HGUIWND *pWnd);

	virtual int WndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD evt, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_ID;
};

//
//���ֱ༭��֧�ְ˽��ơ�ʮ���ơ�ʮ������
//��ӦEVT_KEYUP��VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT����Ϣ
//�û�����EVT_CMDҪ��
//EVT_CMD��Ϣ����:wParam=�ؼ�ID,lParam=EVT_CMD_CTRL_UPDATE(��ʾ�������б仯)
//
class C_NUMEDIT:public C_GUICTRL
{
public:
	C_NUMEDIT(void);
	virtual ~C_NUMEDIT();

public:
	static C_NUMEDIT *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_DWORD nID); 

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
class C_ASCEDIT:public C_GUICTRL
{
public:
	C_ASCEDIT(void);
	virtual ~C_ASCEDIT();

public:
	static C_ASCEDIT *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_DWORD nID); 

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
class C_VKBOARD:public C_GUICTRL
{
public:
	C_VKBOARD(void);
	virtual ~C_VKBOARD();

public:
	static C_VKBOARD *NewCtrl(void);

public:
	bool Create(S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_DWORD nID); 

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
class C_TEXTEDIT:public C_GUICTRL
{
public:
	C_TEXTEDIT(void);
	virtual ~C_TEXTEDIT();

public:
	static C_TEXTEDIT *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_DWORD nID); 

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
class C_BUTTON:public C_GUICTRL
{
public:
	C_BUTTON(void);
	virtual ~C_BUTTON();

public:
	static C_BUTTON *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_DWORD nID); 

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
class C_SELECTBOX:public C_GUICTRL
{
public:
	C_SELECTBOX(void);
	virtual ~C_SELECTBOX();

public:
	static C_SELECTBOX *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszSelectText, S_DWORD dwStyle, const S_RECT& rect, C_HGUIWND* pParentWnd, S_DWORD nID); 
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

#endif //__HGUI_CTRL_H__

