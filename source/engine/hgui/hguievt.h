//---------------------------------------------------------------------------------
// Copyright (c) Haisoft 2006-9-1
// Copyright jinhailiao 2008-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			HGuievt.h
// Description:		
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2006-09-01	Create					Kingsea
// 2.0.0		2006-09-06  update                  Kingsea
// 3.0.0		2008-03-01	update					Kingsea
//---------------------------------------------------------------------------------

#ifndef  __HGUI_EVENT_H__
#define  __HGUI_EVENT_H__

#include "haibase.h"

enum
{
	EVT_NULL,
	EVT_CREATE,
	EVT_SYSPAINT,
	EVT_PAINT,
	EVT_TIMER,
	EVT_DESTROY,
	EVT_AUTOBACK,
	EVT_EXIT,
	EVT_QUIT,
	
	EVT_KEYDN,
	EVT_KEYUP,
	EVT_CHAR,
	EVT_SYSKEY,
	EVT_MOUSEDN,
	EVT_MOUSEUP,
	EVT_MOUSEDB,
	EVT_MOUSEMV,
	
	EVT_COMMAND,
	EVT_RESTORE,

	EVT_CTRLCOLOR,
	EVT_CTRLFOCUS,

	EVT_SETFOCUS,
	EVT_KILLFOCUS,

	EVT_USER,
	EVT_END
};

/*
 *	--- EVT_NULL ---
 */

/*
 *	--- EVT_CREAT ---
 *  EVT_CREATE is first event to the scene, the event send by system.
 *  wParam and lParam is passed by the pre scene.
 */

/*
 *	--- EVT_PAINT ---
 *	--- EVT_TIMER ---
 */

/*
 *	--- EVT_DESTROY ---
 *  wParam and lParam will defined by user.
 *  The scene and dialog must release resource when it receives EVT_DESTROY.
 *  then tells system that will GOTOSCENE or RETURNSCENE or SWITCHAPP.
 */
 
/*
 *	--- EVT_EXIT ---
 *  wParam was undefined and lParam also was undefined now.
 *  The scene and dialog must release resource when it receives EVT_EXIT.
 *  if do not exit.then do it followed under.
 *	{
 *		HGui_SetScreenFlush(SCREEN_FLUSH_ON);//beacase the system turn off this screen;
 *		exit = AskUser();
 *		if (exit)
 *		{
 *			//release resource
 *			HGui_SetScreenFlush(SCREEN_FLUSH_OFF);
 *		}
 *		else
 *		{
 *			pEvent->EventType = EVT_NULL;
 *		}
 *	}
 */
 
/*
 *	--- EVT_QUIT ---
 *  Note: user can not send EVT_QUIT event.
 */

/*
 *	--- EVT_SYSKEY ---
 *	--- EVT_KEYDN ---
 *	--- EVT_KEYUP ---
 *	--- EVT_CHAR ---
 *  wParam is the key value.
 */

/*
 *	--- EVT_MOUSEDN ---
 *	--- EVT_MOUSEUP ---
 *	--- EVT_MOUSEDB ---
 *	--- EVT_MOUSEMV ---
 *  lParam is the mouse coordinate. HiWord is the y and LoWord is the x.
 */

/*
 *	--- EVT_COMMAND ---
 *  PUSHBUTTON: LoByte of wParam is CtrlID, HiByte of wParam is BS_PUSHBTN_DN or BS_PUSHBTN_UP
 *              lParam is not defined.
 *  RADIOBUTTON: 
 *  CHECKBUTTON: LoByte of wParam is CtrlID, HiByte of wParam is BS_CHECKBTN_CHECKED of BS_CHECKBTN_NOCHECK.
 *               lParam is not defined.
 *  SELECTBUTTON: LoByte of wParam is CtrlID, HiByte of wParam is EVT_MOUSEUP or EVT_MOUSEDN or EVT_MOUSEMV.
 *                lParam is not defined.
 *  EVTAREACTRL: LoByte of wParam is CtrlID, HiByte of wParam is 
 *               EVT_MOUSEDN, EVT_MOUSEUP, EVT_MOUSEDB, EVT_MOUSEMV.
 *               lParam is the EvtCode of user defined.
 *  LISTAREACRL: LoByte of wParam is CtrlID, HiByte of wParam is 
 *               EVT_MOUSEDN, EVT_MOUSEUP, EVT_MOUSEDB.
 *               lParam is the Line Number(from 0).
 *  SCROLLBAR: LoByte of wParam is CtrlID, HiByte of wParam is 
 *             SB_BOTTOM,SB_LINEDOWN,SB_LINEUP,SB_PAGEDOWN,SB_PAGEUP,SB_THUMBPOSITION,SB_THUMBTRACK,SB_TOP.
 *             if HiByte of wParam is SB_THUMBPOSITION or SB_THUMBTRACK, then lParam is the new page start.
 *             other lParam is 0.
 *  PROGRESS: LoByte of wParam is CtrlID, HiByte of wParam is 0.
 *            LoWord of lParam is the Current Val, HiWord of lParam is 0.
 */

/*
 *	--- EVT_RESTORE ---
 */

/*
 *	--- EVT_IPMCHANGE ---
 *  wParam is the input methed ID.lParam is not defined.
 */

/*
 *	--- EVT_CTRLCOLOR ---
 *	The ctrl should send EVT_CTRLCOLOR before PAINT.
 *	wParam is ID of the ctrl, lParam is the handle of DC. 
 */

/*
 *	--- EVT_FOCUS ----
 *	wParam HiByte is HG_GETFOCUS or HG_KILLFOCUS.
 *	if HG_GETFOCUS, wParam LoByte is Pre focus ctrl.
 *	if HG_KILLFOCUS, wParam LoByte is Current focus ctrl.
 *	lParam is undefined.
 */

/*
 *	--- EVT_USER ----
 *	user defined event begin.
 */

/*
 *	--- EVT_END ---
 */ 

class C_WNDBASE;
struct  S_GUIEVT
{
	C_WNDBASE *pWnd;
	S_WORD   Evt;
	S_WORD   wParam;
	S_DWORD  lParam;

public:
	S_GUIEVT(void){pWnd=NULL, Evt=EVT_NULL, wParam=0x00, lParam=0x00;}
};

struct S_KbState
{
	bool m_CapsLock;
	bool m_ShiftLock;

public:
	S_KbState(void){m_CapsLock=false,m_ShiftLock=false;}
};

#endif // __HGUI_EVENT_H__

