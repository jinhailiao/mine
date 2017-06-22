/** @file
 *  @brief 跟踪系统定义
 *  @author jinhailiao
 *  @date 2011/10/15
 *  @version 0.1
 *  1、定义日志系统跟踪类
 *  2、日志系统配置定义
 */
#include "htrace.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdarg.h>

using std::cout;
using std::ofstream;
using std::ios;

const char *C_HTRACE::m_pLogBasePath = "./"; // 日志输出径
S_TraceSet *C_HTRACE::m_pSystemSet = NULL;

//以下函数请根据需要修改
//modifybegin
char *GetLogfileName(void)
{//必须ASCII码形式的名字
	extern char *GetTraceTitle(void);
	char *pStr = GetTraceTitle();

	if (pStr) 
		return pStr;
	return "dyjc";
}
char *GetLogfileSuffix(void)
{
	return ".log";
}
char *GetLogPrivateDir(void)
{
	char *pStr;
	extern char *GetTraceTitle(void);
	static char PrivateDir[12] = {0};

	if (PrivateDir[0])
		return PrivateDir;
	else if ((pStr = GetTraceTitle()) != NULL) 
	{
		strncpy(PrivateDir, pStr, sizeof(PrivateDir)-4);
		strcat(PrivateDir, "/");
		return PrivateDir;
	}
	return "dyjc/";
}
#include "haitime.h"
char *GetBackupLogName(void)
{//格式必须为:YYYYMMDDHHmm
	static char BakLogName[14];
	memset(BakLogName, 0x00, sizeof(BakLogName));
	C_TIME::Now2Ascii12(BakLogName);
	return BakLogName;
}
void GetTimeStamp(string &tmstr)
{
	char tm[14];

	C_TIME::Now2Stamp(tm);
	tmstr.append(tm, sizeof(tm));
}

#include "haifile.h"
int MakeLogDir(const char *path)
{
	return C_FILE::MakeDir(path);
}
//modifyend

char *GetTraceTitle(void)
{
	S_TraceSet *pset = C_HTRACE::GetSystemTraceSetting();
	if (pset) return (char *)pset->GetTitle();
	return NULL;
}

// C_TRACE function begin
static const char *sTraceType[] =
{
	"", "[E]", "[W]", "[I]", "[V]", "[D]"
};

C_HTRACE::C_HTRACE(unsigned char *ptitle, int level, int way):m_curset(ptitle, level, way)
{
//	assert(level > S_TraceSet::L_DISABLED);
	m_operate = 0;
	m_context = sTraceType[m_curset.GetLevel()];
}

C_HTRACE::~C_HTRACE()
{
}

/** @brief 设置系统跟踪日志输出路径 */
void C_HTRACE::SetLogBasePath(const char *pLogBasePath)
{
	m_pLogBasePath = pLogBasePath;
}

/** @brief 获取系统跟踪日志输出路径 */
const char *C_HTRACE::GetLogBasePath(void)
{
	return m_pLogBasePath;
}

void C_HTRACE::SetSystemTraceSetting(S_TraceSet *psysset)
{
	m_pSystemSet = psysset;
}

S_TraceSet* C_HTRACE::GetSystemTraceSetting(void)
{
	return m_pSystemSet;
}

C_HTRACE & C_HTRACE::operator<<(TRACEOP *pfnop)
{
	return pfnop(*this);
}

void C_HTRACE::ToDecString(char val, string &strDigit)
{
	ToDecString((long)val, strDigit);
}

void C_HTRACE::ToDecString(unsigned char val, string &strDigit)
{
	ToDecString((unsigned long)val, strDigit);
}

void C_HTRACE::ToDecString(short val, string &strDigit)
{
	ToDecString((long)val, strDigit);
}

void C_HTRACE::ToDecString(unsigned short val, string &strDigit)
{
	ToDecString((unsigned long)val, strDigit);
}

void C_HTRACE::ToDecString(int val, string &strDigit)
{
	ToDecString((long)val, strDigit);
}

void C_HTRACE::ToDecString(unsigned int val, string &strDigit)
{
	ToDecString((unsigned long)val, strDigit);
}

C_HTRACE & C_HTRACE::operator<<(const char *pstr)
{
	m_context.append(pstr);
	return *this;
}

C_HTRACE & C_HTRACE::operator<<(const unsigned char *pstr)
{
	return operator<<((const char *)pstr);
}

C_HTRACE & C_HTRACE::operator<<(char *pstr)
{
	return operator<<((const char *)pstr);
}

C_HTRACE & C_HTRACE::operator<<(unsigned char *pstr)
{
	return operator<<((const char *)pstr);
}

C_HTRACE & C_HTRACE::operator<<(float val)
{
	return operator<<((double)val);
}

C_HTRACE & C_HTRACE::operator<<(double val)
{
	char buffer[32];
	sprintf(buffer, "%f", val);
	return operator<<(buffer);
}


/** @brief <<操作符重载 */
C_HTRACE & C_HTRACE::operator<<(const string &rstr)
{
	unsigned long op = m_operate & TRACEOP_JZMSK;

	if (op == TRACEOP_HEX || op == TRACEOP_hex)//十六进制输出串
	{
		for (string::const_iterator iter = rstr.begin(); iter < rstr.end(); ++iter)
		{
			_ToHexString(*iter, m_context, (op == TRACEOP_HEX));
			m_context.push_back(' ');
		}
	}
	else
	{
		m_context.append(rstr);//字符形式输出串
	}

	return *this;
}

C_HTRACE & C_HTRACE::operator<<(const C_BCD &rBcd)
{
	string strBcd = rBcd;
	return operator<<(strBcd.c_str());
}

C_HTRACE & C_HTRACE::operator<<(const S_DATE &rDate)
{
	char buffer[64];

	memset(buffer, 0x00, sizeof(buffer));
	C_TIME::Time2YMDhms(C_TIME::MakeTime(rDate), buffer);
	return operator<<(buffer);
}

C_HTRACE & C_HTRACE::operator<<(const S_TIMEX &rTimex)
{
	string strDigit;
	char buffer[64];

	memset(buffer, 0x00, sizeof(buffer));
	C_TIME::Time2YMDhms(rTimex.m_Second, buffer);
	ToDecString((unsigned long)rTimex.m_MS, strDigit);
	return operator<<(buffer)<<":"<<strDigit.c_str();
}

/** @brief 重载函数运算符() */
void C_HTRACE::operator()(const char *pszformat, ...)
{
	char buffer[2048];
	va_list arg;

	memset(buffer, 0x00, sizeof(buffer));
	va_start(arg, pszformat);
	vsprintf(buffer, pszformat, arg);
	va_end(arg);

	(*this)<<tag<<buffer<<endl;
}

/** @brief 行输出算子 */
C_HTRACE & endl(C_HTRACE & v)
{
	S_TraceSet *pset;

	if (C_HTRACE::m_pSystemSet && (v.m_curset.GetLevel() > C_HTRACE::m_pSystemSet->GetLevel()))
		goto end; //  当前跟踪级别高于系统设置的级别，退出

	if (C_HTRACE::m_pSystemSet)
		pset = C_HTRACE::m_pSystemSet;
	else
		pset = &v.m_curset;

	v.m_context += '\n';

	if (pset->GetWay() == S_TraceSet::WAY_NONE)
		;
	else if (pset->GetWay() == S_TraceSet::WAY_STDOUT)
		cout<<v.m_context;
	else
	{ // 以下估计在WINDOWS下编译与confuse冲突
#ifndef WIN32
		char filename[CFG_PATH_FILE_MAX_LEN];
		memset(filename, 0x00, sizeof(filename));
		v.GetOutfileName(*pset, filename);

		if (pset->GetWay() == S_TraceSet::WAY_LOGFILE)
		{
			ofstream outfile(filename, ios::app);
			if (!outfile)//handle dir
			{
				MakeLogDir(filename);
				outfile.open(filename, ios::app);
				if (!outfile)
					goto end;
			}
			outfile<<v.m_context;//write log file
			outfile.seekp(0x00, std::ios::end);//handle logfile size and total
			if (outfile.tellp() >= (int)pset->GetFileSize())
			{
				char backfile[CFG_PATH_FILE_MAX_LEN];
				memset(backfile, 0x00, sizeof(backfile));
				v.GetBackfileName(*pset, backfile);

				outfile.close();
				if (v.RenameFile(filename, backfile) == true)
					v.DelLogFile(*pset);
			}
		}
		else//for linux tty
		{
			ofstream outfile(filename, ios::out);
			if (!outfile)
				goto end;
			outfile<<v.m_context;//write
		}
#endif
	}

end:
	v.m_operate = 0;
	v.m_context = sTraceType[v.m_curset.GetLevel()];

	return v;
}

/** @brief 时钟输出算子 */
C_HTRACE & time(C_HTRACE & v)
{
	string timestr;

	GetTimeStamp(timestr);

	v.m_context.append("(");
	v.m_context.append(timestr);
	v.m_context.append(") ");

	return v;
}

/** @brief 标题输出算子 */
C_HTRACE & title(C_HTRACE & v)
{
	if (C_HTRACE::m_pSystemSet && C_HTRACE::m_pSystemSet->GetTitle())
	{
		v.m_context.append((char *)C_HTRACE::m_pSystemSet->GetTitle());
		v.m_context.append(": ");
	}
	else if (v.m_curset.GetTitle())
	{
		v.m_context.append((char *)v.m_curset.GetTitle());
		v.m_context.append(": ");
	}

	return v;
}

/** @brief 标签（title+time）输出算子 */
C_HTRACE & tag(C_HTRACE & v)
{
	if (C_HTRACE::m_pSystemSet && C_HTRACE::m_pSystemSet->GetTitle())
	{
		v.m_context.append((char *)C_HTRACE::m_pSystemSet->GetTitle());
	}
	else if (v.m_curset.GetTitle())
	{
		v.m_context.append((char *)v.m_curset.GetTitle());
	}

	return time(v);
}

/** @brief 十六进制（大写）输出算子 */
C_HTRACE & HEX(C_HTRACE & v)
{
	v.m_operate &= ~TRACEOP_JZMSK;
	v.m_operate |= TRACEOP_HEX;

	return v;
}

/** @brief 十六进制（小写）输出算子 */
C_HTRACE & hex(C_HTRACE & v)
{
	v.m_operate &= ~TRACEOP_JZMSK;
	v.m_operate |= TRACEOP_hex;

	return v;
}

/** @brief 十进制输出算子 */
C_HTRACE & dec(C_HTRACE & v)
{
	v.m_operate &= ~TRACEOP_JZMSK;
	v.m_operate |= TRACEOP_DEC;

	return v;
}

/** @brief 八进制输出算子 */
C_HTRACE & oct(C_HTRACE & v)
{
	v.m_operate &= ~TRACEOP_JZMSK;
	v.m_operate |= TRACEOP_OCT;

	return v;
}

/** @brief 二进制输出算子 */
C_HTRACE & bin(C_HTRACE & v)
{
	v.m_operate &= ~TRACEOP_JZMSK;
	v.m_operate |= TRACEOP_BIN;

	return v;
}

static const char *spHexDigit = "0123456789ABCDEF";
static const char *sphexDigit = "0123456789abcdef";
/** @brief 十六进制输出重载 */
void C_HTRACE::_ToHexString(unsigned char val, string &strDigit, bool upcase)
{
	const char *pdigit = (upcase==true? spHexDigit:sphexDigit); 

	strDigit += pdigit[val>>4];
	strDigit += pdigit[val&0x0F];
}

void C_HTRACE::ToHexString(unsigned char val, string &strDigit, bool upcase)
{
	strDigit += "0x";
	_ToHexString(val, strDigit, upcase);
}

void C_HTRACE::ToHexString(char val, string &strDigit, bool upcase)
{
	ToHexString((unsigned char)val, strDigit, upcase);
}

void C_HTRACE::ToHexString(unsigned short val, string &strDigit, bool upcase)
{
	strDigit += "0x";
	_ToHexString((unsigned char)(val>>8), strDigit, upcase);
	_ToHexString((unsigned char)(val>>0), strDigit, upcase);
}

void C_HTRACE::ToHexString(short val, string &strDigit, bool upcase)
{
	ToHexString((unsigned short)val, strDigit, upcase);
}

void C_HTRACE::ToHexString(unsigned long val, string &strDigit, bool upcase)
{
	strDigit += "0x";
	_ToHexString((unsigned char)(val>>24), strDigit, upcase);
	_ToHexString((unsigned char)(val>>16), strDigit, upcase);
	_ToHexString((unsigned char)(val>> 8), strDigit, upcase);
	_ToHexString((unsigned char)(val>> 0), strDigit, upcase);
}

void C_HTRACE::ToHexString(long val, string &strDigit, bool upcase)
{
	ToHexString((unsigned long)val, strDigit, upcase);
}

void C_HTRACE::ToHexString(unsigned int val, string &strDigit, bool upcase)
{
	if (sizeof(int) == 2)
		ToHexString((unsigned short)val, strDigit, upcase);
	else
		ToHexString((unsigned long)val, strDigit, upcase);
}

void C_HTRACE::ToHexString(int val, string &strDigit, bool upcase)
{
	ToHexString((unsigned int)val, strDigit, upcase);
}

/** @brief 十进制输出重载 */
void C_HTRACE::ToDecString(long val, string &strDigit)
{
	int power, tmp;
	bool sign = (val<0? true:false);

	if (sign == true)
	{
		val = -val;
		strDigit += '-';
	}

	for (tmp = val, power = 1; tmp >= 10; tmp /= 10)
		power *= 10;

	for (tmp = 0; power > 0; power /= 10,tmp++)
	{
		strDigit += '0' + (char)(val/power);
		val %= power;
	}
}

void C_HTRACE::ToDecString(unsigned long val, string &strDigit)
{
	int power, tmp;

	for (tmp = val, power = 1; tmp >= 10; tmp /= 10)
		power *= 10;

	for (tmp = 0; power > 0; power /= 10,tmp++)
	{
		strDigit += '0' + (char)(val/power);
		val %= power;
	}
}

/** @brief 取输出文件名
 *  @param[in] rSet 跟踪配置
 *  @param[out] Outfile 输出文件名
 *  @return 输出文件名指针
 */
char *C_HTRACE::GetOutfileName(const S_TraceSet &rSet, char *Outfile)
{
	if (rSet.GetWay() == S_TraceSet::WAY_LOGFILE)
	{
		sprintf(Outfile, "%s%s%s%s", GetLogBasePath(), GetLogPrivateDir(), GetLogfileName(), GetLogfileSuffix());
	}
	else if (rSet.GetWay()>=S_TraceSet::WAY_TTYP0 && rSet.GetWay()<S_TraceSet::WAY_MAX)
	{//for Linux tty
		const char *ttydir = "/dev/pts/";
		unsigned char tty = rSet.GetWay() - S_TraceSet::WAY_TTYP0;

		sprintf(Outfile, "%s%d", ttydir, tty);
	}

	return Outfile;
}

/** @brief 取备份文件名
 *  @param[in] rSet 跟踪配置
 *  @param[out] OutDir 输出文件名
 *  @return 输出文件名指针
 */
char *C_HTRACE::GetBackfileName(const S_TraceSet &rSet, char *OutDir)
{
	if (rSet.GetWay() == S_TraceSet::WAY_LOGFILE)
	{
		sprintf(OutDir, "%s%s%s%s", GetLogBasePath(), GetLogPrivateDir(), GetBackupLogName(), GetLogfileSuffix());
	}
	return OutDir;
}

/** @brief 文件改名
 *  @param[in] SrcFile 源文件名
 *  @param[in] DstFile 目的文件名
 *  @return true
 */
bool C_HTRACE::RenameFile(const char *SrcFile, const char *DstFile)
{
	return (rename(SrcFile, DstFile) == 0);
}

/** @brief 删除文件
 *  @param[in] rSet 跟踪配置
 *  @return true
 */
bool C_HTRACE::DelLogFile(const S_TraceSet &rSet)
{
	int MaxTimes = 0;// explicitly control delete times
	while ((MaxTimes++ < 3) && (DelOneLogFile(rSet) == false))
		;
	return true;
}

#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <dirent.h>
#endif

/** @brief 删除文件
 *  @param[in] rSet 跟踪配置
 *  @return true
 */
bool C_HTRACE::DelOneLogFile(const S_TraceSet &rSet)
{
#ifdef WIN32
	intptr_t hfind;
	char dir[CFG_PATH_FILE_MAX_LEN];
	char fil[CFG_PATH_FILE_MAX_LEN];
	char cwd[CFG_PATH_FILE_MAX_LEN];
	char filter[32] = "*";
	struct _finddata_t findinfo;

	memset(dir, 0x00, sizeof(dir));
	memset(fil, 0x00, sizeof(fil));
	memset(cwd, 0x00, sizeof(cwd));

	sprintf(dir, "%s%s", GetLogBasePath(), GetLogPrivateDir());
	strcat(filter, GetLogfileSuffix());

	_getcwd(cwd, sizeof(cwd));
	_chdir(dir);

	int LogFileCnt = 0;
	if (hfind = _findfirst(filter, &findinfo))
	{
		strcpy(fil, findinfo.name);
		LogFileCnt++;
		while (_findnext(hfind, &findinfo) == 0)
		{
			if (strcmp(findinfo.name, fil) < 0)
				strcpy(fil, findinfo.name);
			LogFileCnt++;
		}
        _findclose(hfind);
	}

	if (LogFileCnt > rSet.GetFileTotal())
		remove(fil);

	_chdir(cwd);

	return (LogFileCnt>(rSet.GetFileTotal()+1)? false:true);
#else
	char dir[CFG_PATH_FILE_MAX_LEN];
	char fil[CFG_PATH_FILE_MAX_LEN];
	int LogFileCnt = 0;
	DIR *pDIR;
	struct dirent *pdirent;

	memset(dir, 0x00, sizeof(dir));
	memset(fil, 0x00, sizeof(fil));
	sprintf(dir, "%s%s", GetLogBasePath(),  GetLogPrivateDir());

	if ((pDIR = opendir(dir)) == NULL)
		return true;//it's failed, don't repeat del

	while ((pdirent = readdir(pDIR)) != NULL)
	{
		if (strcmp(pdirent->d_name, ".")==0 || strcmp(pdirent->d_name, "..")==0)
			continue; //ignore dot and dot-dot
		if (strstr(pdirent->d_name, GetLogfileSuffix()) == NULL)
			continue; //ignore other file
		if (fil[0] == '\0')
			strcpy(fil, pdirent->d_name);
		else if (strcmp(pdirent->d_name, fil) < 0)
			strcpy(fil, pdirent->d_name);
		LogFileCnt++;
	}

	closedir(pDIR);
	
	if (LogFileCnt > rSet.GetFileTotal())
	{
		char rmfile[CFG_PATH_FILE_MAX_LEN];
		sprintf(rmfile, "%s%s", dir, fil);
		remove(rmfile);
	}
	return (LogFileCnt>(rSet.GetFileTotal()+1)? false:true);
#endif
}


C_HTRACE hai_logn((unsigned char *)"dyjc", S_TraceSet::LEVEL_DISABLED);
C_HTRACE hai_loge((unsigned char *)"dyjc", S_TraceSet::LEVEL_ERROR);
C_HTRACE hai_logw((unsigned char *)"dyjc", S_TraceSet::LEVEL_WARNING);
C_HTRACE hai_logi((unsigned char *)"dyjc", S_TraceSet::LEVEL_INFO);
C_HTRACE hai_logv((unsigned char *)"dyjc", S_TraceSet::LEVEL_VERBOSE);
C_HTRACE hai_logd((unsigned char *)"dyjc", S_TraceSet::LEVEL_DEBUG);

