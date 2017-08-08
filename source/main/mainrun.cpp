/** @file
 *  @brief 
 *  @author jinhailiao
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "demoapp.h"
#include "engineapp.h"

#if defined(HGUI_DEMO_APP)

C_DEMOAPP theStartApp;
C_DEMOLOGO theStartWnd;

#elif defined(MINE_ENGINE_APP)

C_MINEAPP theStartApp;
C_MINELOGO theStartWnd;

#elif defined(ELS_ENGINE_APP)

C_MINEAPP theStartApp;
C_MINELOGO theStartWnd;

#endif

int InitEngine(void)
{
	C_HGUIAPP::SetCurApp(&theStartApp);
	theStartApp.SysInit(APP_RESOURCE_PATH);
	theStartApp.SetDesktopWnd(&theStartWnd);
	return 0;
}

int RunEngine(void)
{
	return theStartApp.GuiAppRun();
}

void MainEngine(void *pData)
{
	InitEngine();
	for (;;)
		RunEngine();
}


