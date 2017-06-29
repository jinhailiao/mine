/** @file
 *  @brief 
 *  @author jinhailiao
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "demoapp.h"

C_DEMOAPP theDemoApp;
C_DESKTOP theDesktop;

int InitEngine(void)
{
	C_DEMOAPP::SetCurApp(&theDemoApp);
	theDemoApp.SetDesktopWnd(&theDesktop);
	return 0;
}

int RunEngine(void)
{
	return theDemoApp.GuiAppRun();
}


