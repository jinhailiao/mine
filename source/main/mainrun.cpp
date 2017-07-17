/** @file
 *  @brief 
 *  @author jinhailiao
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "demoapp.h"
#include "engineapp.h"

#if defined(HGUI_DEMO_APP)

C_DEMOAPP theDemoApp;
C_DESKTOP theDesktop;

#elif defined(MINE_ENGINE_APP)

C_MINEAPP theDemoApp;
C_MINEWND theDesktop;

#elif defined(ELS_ENGINE_APP)

C_DEMOAPP theDemoApp;
C_DESKTOP theDesktop;

#endif

int InitEngine(void)
{
	C_HGUIAPP::SetCurApp(&theDemoApp);
	theDemoApp.SysInit("..\\..\\resource\\");
	theDemoApp.SetDesktopWnd(&theDesktop);
	return 0;
}

int RunEngine(void)
{
	return theDemoApp.GuiAppRun();
}

void MainEngine(void *pData)
{
	InitEngine();
	for (;;)
		RunEngine();
}


