/** @file
 *  @brief �������ö���
 *  @author jinhailiao
 *  @date 2011/10/15
 *  @version 0.1
 *  1����־ϵͳ���ö���
 */
#include "htraceset.h"

/** @brief �û����캯�� */
S_TraceSet::S_TraceSet(unsigned char *ptitle, int level, int way)
:m_level(level),m_way(way)
{
	m_FileSiz  = TRACE_FILE_SIZE_MIN;
	m_FileTtl  = TRACE_FILE_TOTAL_MAX;
	if (ptitle == NULL)
		memset(m_title, 0x00, sizeof(m_title));
	else
		strncpy((char *)m_title, (const char *)ptitle, 10);
	m_Reserved = 0;
}

/** @brief �������캯�� */
S_TraceSet::S_TraceSet(const S_TraceSet &rTraceSet)
:m_level(rTraceSet.m_level), m_way(rTraceSet.m_way)
{
	m_FileSiz  = rTraceSet.m_FileSiz;
	m_FileTtl  = rTraceSet.m_FileTtl;
	strncpy((char *)m_title, (const char *)rTraceSet.m_title, 10);
	m_Reserved = rTraceSet.m_Reserved;
}

S_TraceSet::~S_TraceSet()
{
}

/** @brief ���ظ�ֵ���� */
S_TraceSet & S_TraceSet::operator=(const S_TraceSet &rTraceSet)
{
	if (this != &rTraceSet)
	{
		strncpy((char *)m_title, (const char *)rTraceSet.m_title, 10);
		m_level = rTraceSet.m_level;
		m_way = rTraceSet.m_way;
		m_FileSiz = rTraceSet.m_FileSiz;
		m_FileTtl = rTraceSet.m_FileTtl;
		m_Reserved = rTraceSet.m_Reserved;
	}

	return *this;
}

/** @brief ���ü���
 *  @param[in] level ����[LEVEL_DISABLED, LEVEL_ERROR, LEVEL_WARNING, LEVEL_INFO, LEVEL_VERBOSE, LEVEL_DEBUG, LEVEL_MAX]
 *  @return true
 */
bool S_TraceSet::SetLevel(int level)
{
	if (level>=LEVEL_DISABLED && level<LEVEL_MAX)
	{
		m_level = (unsigned char)level;
		return true;
	}
	return false;
}

/** @brief �������Ŀ��
 *  @param[in] way Ŀ��[WAY_NONE, WAY_STDOUT, WAY_LOGFILE, WAY_TTYP0, WAY_TTYP1...]
 *  @return true
 */
bool S_TraceSet::SetWay(int way)
{
	if (way >= WAY_NONE && way < WAY_MAX)
	{
		m_way = (unsigned char)way;
		return true;
	}
	return false;
}

/** @brief ��������ļ���С
 *  @param[in] FileSiz �ļ���С
 *  @return true
 */
bool S_TraceSet::SetFileSize(unsigned int FileSiz)
{
	if (FileSiz < TRACE_FILE_SIZE_MIN)
		m_FileSiz = TRACE_FILE_SIZE_MIN;
	else if (FileSiz > TRACE_FILE_SIZE_MAX)
		m_FileSiz = TRACE_FILE_SIZE_MAX;
	else
		m_FileSiz = FileSiz;
	return true;
}

/** @brief �����ļ�����
 *  @param[in] FileTtl �ļ�����
 *  @return true
 */
bool S_TraceSet::SetFileTotal(unsigned char FileTtl)
{
	if (FileTtl < TRACE_FILE_TOTAL_MIN)
		m_FileTtl = TRACE_FILE_TOTAL_MIN;
	else if (FileTtl > TRACE_FILE_TOTAL_MAX)
		m_FileTtl = TRACE_FILE_TOTAL_MAX;
	else
		m_FileTtl = FileTtl;
	return true;
}

/** @brief �������ñ���
 *  @param[in] ptitle ����
 *  @return true
 */
bool S_TraceSet::SetTitle(const unsigned char *ptitle)
{
	if (ptitle == NULL)
		memset(m_title, 0x00, sizeof(m_title));
	else
		strncpy((char *)m_title, (const char *)ptitle, 10);
	return true;
}



