/** @file
 *  @brief 文件操作集
 *  @author jinhailiao
 *  @date 2011/11/07
 *  @version 0.1
 *  1、将文件操作封装成类
 *  2、本系统中所有文件操作必须通过此类进行，方便扩展和维护
 */
#include "haibase.h"
#include "haifile.h"

#ifdef WIN32
#include <io.h>
#include <direct.h>

#define access	_access
#define mkdir(path, mode)	_mkdir(path)
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <errno.h>
#include <dirent.h>
#endif

C_FILE::C_FILE(void)
{
	m_fp = NULL;
}

/** @brief 文件操作类定义 */
C_FILE::C_FILE(const char *ppath, const char *mode)
{
	m_fp = NULL;
	OpenFile(ppath, mode);
}

C_FILE::~C_FILE()
{
	if (m_fp != NULL)
		_CloseFile(m_fp);
	m_fp = NULL;
}

/** @brief 打开文件
 *  @param[in] ppth 文件路径
 *  @param[in] mode 打开模式
 */
bool C_FILE::OpenFile(const char *ppath, const char *mode)
{
	if (m_fp != NULL)
		_CloseFile(m_fp);

	m_fp = _OpenFile(ppath, mode);
	if (m_fp == NULL)
	{
		if (MakeDir(ppath) != 0)
			printf("MakeDir for %s failed\n",ppath);
		else
			m_fp = _OpenFile(ppath, mode);
	}
	return m_fp != NULL;
}

/** @brief 文件对象就绪
 *  @return true 成功；false 失败
 */
bool C_FILE::operator!(void)
{
	return (m_fp == NULL);
}

/** @brief 测试是否到文件尾
 *  @return 文件结束返回非零值，否则为0
 */
int C_FILE::FileEof(void)
{
	return feof(m_fp);
}

/** @brief 文件行读取
 *  @param[out] pBuffer 输出缓冲
 *  @param[in] size 缓冲大小
 *  @return 成功返回pBuffer，否则返回NULL
 */
char* C_FILE::FileGets(char *pBuffer, int size)
{
	return fgets(pBuffer, size, m_fp); 
}

/** @brief 文件读取
 *  @param[out] pBuffer 输出缓冲
 *  @param[in] size 缓冲大小
 *  @return 读取的字节数
 */
size_t C_FILE::FileRead(void *pBuffer, size_t size)
{
	size_t cnt = fread(pBuffer, 1, size, m_fp);
	return cnt;
}

/** @brief 文件写入
 *  @param[in] pBuffer 输入缓冲
 *  @param[in] size 缓冲大小
 *  @return 写入的字节数
 */
size_t C_FILE::FileWrite(const void *pBuffer, size_t size)
{
	size_t cnt = fwrite(pBuffer, size, 1, m_fp);
	return cnt * size;
}

/** @brief 文件定位
 *  @param[in] offset 相对于seek的偏移
 *  @param[in] seek SEEK_CUR | SEEK_END | SEEK_SET
 *  @return 0 成功；!0 失败
 */
int C_FILE::FileSeek(long offset, int seek)
{
	return fseek(m_fp, offset, seek);
}

/** @brief 获取文件当前读写位置
 *  @return 文件指针距文件头偏移
 */
long C_FILE::FileTell(void)
{
	return ftell(m_fp);
}

/** @brief 创建目录
 *  @param[in] ppath 目录路径
 *  @return 0 成功；!0 失败
 */
int C_FILE::MakeDir(const char *ppath)
{
	char ch, PathBuf[CFG_PATH_FILE_MAX_LEN];

	strcpy(PathBuf, ppath);

	for(int i = 1; (ch=PathBuf[i])!='\0'; i++)//跳过path[0]字符的判定
	{
		if (ch=='\\' || ch=='/')
		{
			PathBuf[i] = '\0';
			if (access(PathBuf, 0) != 0)
			{
				if (mkdir(PathBuf,00777) != 0)
					return -1;
			}
			PathBuf[i] = ch;
		}
	}
	return 0;
}

/** @brief 移除文件
 *  @param[in] ppath 文件路径
 *  @return 0 成功；!0 失败
 */
int C_FILE::RemoveFile(const char *pfile)
{
	return remove(pfile);
}

/** @brief 同步文件系统
 *  @return 0 成功；!0 失败
 */
int C_FILE::SyncFileSystem(void)
{
#ifndef WIN32
	sync();
#endif
	return 0;
}

/** @brief 查找指定后缀的文件集
 *  @param[in] strPath 工作目录
 *  @param[in] strSuffix 文件后缀
 *  @param[out] strsFileName 文件输出缓冲
 *  @return 查找成功的文件个数
 */
S_WORD C_FILE::FindFileWithSuffix(const string &strPath, const string &strSuffix, strings &strsFileName)
{
#ifdef WIN32
	intptr_t hfind;
	char cwd[CFG_PATH_FILE_MAX_LEN];
	char filter[CFG_PATH_FILE_MAX_LEN] = "*";
	struct _finddata_t findinfo;
	string strMyPath(strPath);

	if (strMyPath.empty() == true)
		strMyPath.append(".");

	memset(cwd, 0x00, sizeof(cwd));
	if (strSuffix.empty() == false)
		strcat(filter, strSuffix.c_str());

	_getcwd(cwd, sizeof(cwd));
	_chdir(strMyPath.c_str());

	if (hfind = _findfirst(filter, &findinfo))
	{
		strsFileName.push_back(findinfo.name);
		while (_findnext(hfind, &findinfo) == 0)
			strsFileName.push_back(findinfo.name);
		_findclose(hfind);
	}

	_chdir(cwd);

#else

	DIR *pDIR;
	struct dirent *pdirent;
	string strMyPath(strPath);

	if (strMyPath.empty() == true)
		strMyPath.append(".");

	if ((pDIR = opendir(strMyPath.c_str())) == NULL)
		return 0x00;//it's failed, don't repeat del

	while ((pdirent = readdir(pDIR)) != NULL)
	{
		if (strcmp(pdirent->d_name, ".")==0 || strcmp(pdirent->d_name, "..")==0)
			continue; //ignore dot and dot-dot
		if (strSuffix.empty() == false)
		{
			if (strstr(pdirent->d_name, strSuffix.c_str()) == NULL)
				continue; //ignore other file
		}
		strsFileName.push_back(pdirent->d_name);
	}

	closedir(pDIR);
	
#endif
	return (S_WORD)strsFileName.size();
}

FILE *C_FILE::_OpenFile(const char *ppath, const char *mode)
{
	if (ppath == NULL || *ppath == '\0' || mode == NULL)
		return NULL;

	FILE* fp = fopen(ppath, mode);
	if (fp)
	{
#ifndef WIN32
		int fd = fileno(fp);
		flock(fd,LOCK_EX);
		fcntl(fd,O_FSYNC);
		if (fd > 20)
			printf("WARNING:fd for file %s is %d\n",ppath, fd);
#endif
	}

	return fp;
}

int C_FILE::_CloseFile(FILE *pfp)
{
#ifndef WIN32
	if (pfp)
	{
		int fd = fileno(pfp);
		fsync(fd);
		flock(fd,LOCK_UN);
		sync(); // 文件系统不能及时同步到磁盘（掉电时会出现问题），故加此词句
	}
#endif
	return fclose(pfp);
}

