/** @file
 *  @brief 引擎基本配置
 *  @author jinhailiao
 *  @date 2017/06/16
 *  @version 0.1
 */
#ifndef __HAI_CONFIG_H__
#define __HAI_CONFIG_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32

#include <windows.h>
#include <io.h>
#include <direct.h>

#else

#include <unistd.h> 
#include <dirent.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/io.h>
#include <sys/sem.h>
#include <sys/param.h>
#include <fcntl.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <termios.h>
#include <assert.h>

#endif


#ifdef WIN32

#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#define F_OK		0
#define access	_access
#define mkdir(path, mode)	_mkdir(path)

#endif

#ifdef WIN32
#define PID_T	int
#define getpid()	0xFFFF
#define socklen_t	int
#else
#define PID_T	pid_t
#endif


#ifndef WIN32

#undef __STL_USE_EXCEPTIONS
#undef __EXCEPTIONS
#ifdef __THROW_BAD_ALLOC
#undef __THROW_BAD_ALLOC
#endif
#ifndef __THROW_BAD_ALLOC
	#define __THROW_BAD_ALLOC throw bad_alloc()
#endif
#define ASSERT(exp) {if (!(exp)) printf("***********Assert failed.file=%s,line=%d,exp=%s\n",__FILE__,__LINE__,#exp);}

#else

#include <assert.h>
#define ASSERT assert

#endif

#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <list>
#include <bitset>
#include <numeric>
#include <iostream>
#include <map>
using namespace std;


/** 常用宏定义 */
#define CFG_PATH_FILE_MAX_LEN	260

#define __HGUI_BEGIN_NAMESPACE namespace hgui {
#define __HGUI_END_NAMESPACE }

#define __MINE_BEGIN_NAMESPACE namespace mine {
#define __MINE_END_NAMESPACE }


#endif // __HAI_CONFIG_H__
 
