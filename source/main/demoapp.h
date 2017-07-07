/** @file
 *  @brief 
 *  @author jinhailiao
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __DEMO_APP_H__
#define __DEMO_APP_H__
#include "hguiapp.h"

class C_DEMOAPP:public C_HGUIAPP
{
public:
	C_DEMOAPP(void);
	virtual ~C_DEMOAPP();

	virtual int GuiAppRun(void);
};

class C_DESKTOP:public C_HGUIWND
{
public:
	C_DESKTOP(void){}

	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

class C_LOGO:public C_HGUIWND
{
public:
	C_LOGO(void){}

	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

#endif//__DEMO_APP_H__

