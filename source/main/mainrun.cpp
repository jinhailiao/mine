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

enum
{
	GAME_ENGINE_NORMAL,
	GAME_ENGINE_STOP,
	GAME_ENGINE_EXIT,
	GAME_ENGINE_FINISHED,

	GAME_ENGINE_MAX		
};

static int EngineFlag = GAME_ENGINE_NORMAL;
int InitEngine(void)
{
	EngineFlag = GAME_ENGINE_NORMAL;
	C_HGUIAPP::SetCurApp(&theStartApp);
	theStartApp.SysInit(APP_RESOURCE_PATH);
	theStartApp.SetDesktopWnd(&theStartWnd);
	return 0;
}

int StopEngine(void)
{
	EngineFlag = GAME_ENGINE_STOP;
	return EngineFlag;
}

bool FinishedEngine(void)
{
	return EngineFlag == GAME_ENGINE_FINISHED;
}

int RunEngine(void)
{
	return theStartApp.GuiAppRun();
}

int EndEngine(void)
{
	return theStartApp.CloseWnd(NULL); // 释放资源
}

void MainEngine(void *pData)
{
	InitEngine();
	while (true)
	{
		if (RunEngine() != 0) // engine收到退出消息
			break;
		if (EngineFlag == GAME_ENGINE_STOP) // 外部要求引擎退出
			EndEngine(), EngineFlag = GAME_ENGINE_EXIT;
	}
	EngineFlag = GAME_ENGINE_FINISHED; // 引擎退出完毕
}


