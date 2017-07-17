/** @file
 *  @brief 
 *  @author jinhailiao
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __MINE_ENGINE_APP_H__
#define __MINE_ENGINE_APP_H__
#include "hguiapp.h"

class C_MINEAPP:public C_HGUIAPP
{
public:
	C_MINEAPP(void);
	virtual ~C_MINEAPP();

	virtual int GuiAppRun(void);
};

class C_MINEWND:public C_HGUIWND
{
public:
	C_MINEWND(void){}

	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

class C_MINELOGO:public C_HGUIWND
{
public:
	C_MINELOGO(void){}

	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

#endif//__MINE_ENGINE_APP_H__

