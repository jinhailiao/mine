/** @file
 *  @brief �ļ�������
 *  @author jinhailiao
 *  @date 2011/11/07
 *  @version 0.1
 *  1�����ļ�������װ����
 *  2����ϵͳ�������ļ���������ͨ��������У�������չ��ά��
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

/** @brief �ļ������ඨ�� */
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

/** @brief ���ļ�
 *  @param[in] ppth �ļ�·��
 *  @param[in] mode ��ģʽ
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

/** @brief �ļ��������
 *  @return true �ɹ���false ʧ��
 */
bool C_FILE::operator!(void)
{
	return (m_fp == NULL);
}

/** @brief �����Ƿ��ļ�β
 *  @return �ļ��������ط���ֵ������Ϊ0
 */
int C_FILE::FileEof(void)
{
	return feof(m_fp);
}

/** @brief �ļ��ж�ȡ
 *  @param[out] pBuffer �������
 *  @param[in] size �����С
 *  @return �ɹ�����pBuffer�����򷵻�NULL
 */
char* C_FILE::FileGets(char *pBuffer, int size)
{
	return fgets(pBuffer, size, m_fp); 
}

/** @brief �ļ���ȡ
 *  @param[out] pBuffer �������
 *  @param[in] size �����С
 *  @return ��ȡ���ֽ���
 */
size_t C_FILE::FileRead(void *pBuffer, size_t size)
{
	size_t cnt = fread(pBuffer, 1, size, m_fp);
	return cnt;
}

/** @brief �ļ�д��
 *  @param[in] pBuffer ���뻺��
 *  @param[in] size �����С
 *  @return д����ֽ���
 */
size_t C_FILE::FileWrite(const void *pBuffer, size_t size)
{
	size_t cnt = fwrite(pBuffer, size, 1, m_fp);
	return cnt * size;
}

/** @brief �ļ���λ
 *  @param[in] offset �����seek��ƫ��
 *  @param[in] seek SEEK_CUR | SEEK_END | SEEK_SET
 *  @return 0 �ɹ���!0 ʧ��
 */
int C_FILE::FileSeek(long offset, int seek)
{
	return fseek(m_fp, offset, seek);
}

/** @brief ��ȡ�ļ���ǰ��дλ��
 *  @return �ļ�ָ����ļ�ͷƫ��
 */
long C_FILE::FileTell(void)
{
	return ftell(m_fp);
}

/** @brief ����Ŀ¼
 *  @param[in] ppath Ŀ¼·��
 *  @return 0 �ɹ���!0 ʧ��
 */
int C_FILE::MakeDir(const char *ppath)
{
	char ch, PathBuf[CFG_PATH_FILE_MAX_LEN];

	strcpy(PathBuf, ppath);

	for(int i = 1; (ch=PathBuf[i])!='\0'; i++)//����path[0]�ַ����ж�
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

/** @brief �Ƴ��ļ�
 *  @param[in] ppath �ļ�·��
 *  @return 0 �ɹ���!0 ʧ��
 */
int C_FILE::RemoveFile(const char *pfile)
{
	return remove(pfile);
}

/** @brief ͬ���ļ�ϵͳ
 *  @return 0 �ɹ���!0 ʧ��
 */
int C_FILE::SyncFileSystem(void)
{
#ifndef WIN32
	sync();
#endif
	return 0;
}

/** @brief ����ָ����׺���ļ���
 *  @param[in] strPath ����Ŀ¼
 *  @param[in] strSuffix �ļ���׺
 *  @param[out] strsFileName �ļ��������
 *  @return ���ҳɹ����ļ�����
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
		sync(); // �ļ�ϵͳ���ܼ�ʱͬ�������̣�����ʱ��������⣩���ʼӴ˴ʾ�
	}
#endif
	return fclose(pfp);
}

