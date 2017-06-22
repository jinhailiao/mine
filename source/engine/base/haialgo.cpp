/** @file
 *  @brief 基础算法集合
 *  @author jinhailiao
 *  @date 2012/02/14
 *  @version 0.1
 */
#include "haialgo.h"
#include "hstrings.h"
#include "haifile.h"

/** @brief 进程退出信号描述*/
static const char *g_LinuxProcessExitSignalDescript[] =
{
	"normal",
	"signal SIGHUP, Hangup.",
	"signal SIGINT, Interrupt",
	"signal SIGQUIT, Quit",
	"signal SIGILL, Illegal instruction",
	"signal SIGTRAP, Trace/breakpoint trap",
	"signal SIGABRT, Aborted",
	"signal SIGEMT, Emulation trap",
	"signal SIGFPE, Arithmetic exception",
	"signal SIGKILL, Killed",
	"signal SIGBUS, Bus error",
	"signal SIGSEGV, Segmentation fault",
	"signal SIGSYS, Bad system call",
	"signal SIGPIPE, Broken pipe",
	"signal SIGTERM, Terminated",
	"signal SIGSTOP, Stopped (signal)",
	"signal SIGTSTP, Stopped (user)",
	"signal SIGCONT, Continued",
	"signal SIGTTIN, Stopped (tty input)",
	"signal SIGTTOU, Stopped (tty output)",
	"signal SIGXCPU, CPU time limit exceeded",
	"signal SIGXFSZ, File size limit exceeded"
};

/** @brief 进程退出信号描述*/
const char *LinuxProcessExitSignalDescript(int signal)
{
	int Cnt = HAI_MEMBER_OF(g_LinuxProcessExitSignalDescript);
	if (signal >= 0 && signal < Cnt)
		return g_LinuxProcessExitSignalDescript[signal];
	return "unknow";
}
/** @brief LINUX API error info */
const char *LinuxApiErrorInfo(char *pBuffer)
{
	sprintf(pBuffer, "errno[%d]=%s", errno, strerror(errno));
	return pBuffer;
}

/** @fn C_IniBase::MakeKey
 *  @brief 取指定名称
 *  @param[in] val 指定值
 *  @param[in] pCfgItem 索引名称数组指针
 *  @param[in] size 数组大小
 *  @return "unknow"或Key名称//NULL 无效指针；!NULL 数组项指针
 */
const char *CfgItem_MakeKey(const S_CFGITEM *pCfgItem, S_WORD size, int val)
{
	for (S_WORD i = 0; i < size; i++)
	{
		if (pCfgItem[i].m_val == val)
			return pCfgItem[i].m_pkey;
	}
//	return NULL;
	return "unknow";
}

/** @fn C_IniBase::MakeValue
 *  @brief 取索引号
 *  @param[in] pkey 指定名称
 *  @param[in] pCfgItem 索引名称数组指针
 *  @param[in] size 数组大小
 *  @return >=0 有效值 -1 无效值
 */
int CfgItem_MakeValue(const S_CFGITEM *pCfgItem, S_WORD size, const char *pkey)
{
	for (S_WORD i = 0; i < size; i++)
	{
		if (strcasecmp(pCfgItem[i].m_pkey, pkey) == 0)
			return pCfgItem[i].m_val;
	}
	return -1;
}

/** @brief 配置项方法集
 */
C_CfgItem::C_CfgItem(const S_CFGITEM *pCfgItem, S_WORD size)
{
	m_pCfgItem = pCfgItem;
	m_Size = size;
}

C_CfgItem::~C_CfgItem()
{
}

/** @fn MakeKey
 *  @brief 取项目名称
 *  @param[in] val 项目值
 *  @return "unknow"或Key名称//NULL 无效指针；!NULL 数组项指针
 */
const char *C_CfgItem::MakeKey(int val)
{
	return CfgItem_MakeKey(m_pCfgItem, m_Size, val);
}

/** @fn MakeValue
 *  @brief 取项目值
 *  @param[in] pkey 项目名称
 *  @return >=0 有效值 -1 无效值
 */
int C_CfgItem::MakeValue(const char *pkey)
{
	return CfgItem_MakeValue(m_pCfgItem, m_Size, pkey);
}

/** @fn MakeIndex
 *  @brief 取项目索引
 *  @param[in] val 项目值
 *  @return >=0 有效值 -1 无效值
 */
int C_CfgItem::MakeIndex(int val)
{
	for (S_WORD i = 0; i < m_Size; i++)
	{
		if (m_pCfgItem[i].m_val == val)
			return (int)i;
	}
	return -1;
}

/** @fn MakeIndex
 *  @brief 取项目索引
 *  @param[in] pkey 项目名称
 *  @return >=0 有效值 -1 无效值
 */
int C_CfgItem::MakeIndex(const char *pkey)
{
	for (S_WORD i = 0; i < m_Size; i++)
	{
		if (strcasecmp(m_pCfgItem[i].m_pkey, pkey) == 0)
			return (int)i;
	}
	return -1;
}

/** @fn MakeKey
 *  @brief 取项目名称
 *  @param[in] idx 项目索引
 *  @return NULL或Key名称
 */
const char *C_CfgItem::MakeKeyWithIdx(S_WORD idx)
{
	if (idx >= m_Size)
		return NULL;
	return m_pCfgItem[idx].m_pkey;
}

/** @fn MakeValue
 *  @brief 取项目值
 *  @param[in] idx 项目索引
 *  @return >=0 有效值 -1 无效值
 */
int C_CfgItem::MakeValueWithIdx(S_WORD idx)
{
	if (idx >= m_Size)
		return -1;
	return m_pCfgItem[idx].m_val;
}

/** @fn SerializeItemKey
 *  @brief 串行化项目名称
 *  @param[out] strItemKey 输出缓冲
 *  @param[in] delimit 间隔符
 *  @return 串行化后的数据长度
 */
S_WORD C_CfgItem::SerializeItemKey(string &strItemKey, char delimit)
{
	if (m_Size > 0)
		strItemKey.append(m_pCfgItem[0].m_pkey);

	for (S_WORD i = 1; i < m_Size; i++)
		strItemKey.append((size_t)1, delimit).append(m_pCfgItem[i].m_pkey);

	return (S_WORD)strItemKey.size();
}


/** @brief BCD码算法
 *  @note
 *  1、最大支持8位整数，4位小数
 *  2、所有BCD缓冲中，BCD码按小端存放
 */
const S_DWORD C_BCD::POWER[8] = {1,10,100,1000,10000,100000,1000000,10000000};
C_BCD::C_BCD(void)
{
	memset(this, 0x00, sizeof(*this));
	m_ValidBcd = true;
}

C_BCD::C_BCD(double val)
{
	memset(this, 0x00, sizeof(*this));
	m_ValidBcd = true;
	m_Integer = (S_DWORD)val;
	m_Float = (S_WORD)((val-m_Integer) * 10000);
}

C_BCD::C_BCD(S_DWORD Integer, S_WORD Float)
{
	memset(this, 0x00, sizeof(*this));
	if (Float <= 9999)
		m_ValidBcd = true, m_Integer = Integer, m_Float = Float;
	else
		m_ValidBcd = false;
}

C_BCD::C_BCD(const string &strBcdAscii)
{
	strings strsBcd;
	memset(this, 0x00, sizeof(*this));
	m_ValidBcd = false;

	if (strsBcd.split(strBcdAscii, '.') >= 1)
	{
		if (strsBcd.size() == 1)
			strsBcd.push_back("0000");
		if (strsBcd[1].size() < 4)//补足四位小数
			strsBcd[1].append(4-strsBcd[1].size(), '0');
		if (ValidBcd(strsBcd[0]) && ValidBcd(strsBcd[1]))
		{
			m_ValidBcd = true;
			m_Integer = Convert(strsBcd[0]);
			m_Float = (S_WORD)Convert(strsBcd[1]);
		}
	}
}

C_BCD::C_BCD(const S_BYTE *pbcdInteger4B, const S_BYTE *pbcdFloat2B)
{
	memset(this, 0x00, sizeof(*this));
	m_ValidBcd = false;

	if (ValidBcd(pbcdInteger4B, 4) && ValidBcd(pbcdFloat2B, 2))
	{
		S_DWORD dwInter;
		memcpy(&dwInter, pbcdInteger4B, 4);
		m_Integer = Convert(dwInter);

		S_WORD wFloat;
		memcpy(&wFloat, pbcdFloat2B, 2);
		m_Float = (S_WORD)Convert(wFloat);

		m_ValidBcd = true;
	}
}

C_BCD::C_BCD(const S_BYTE *pbcdBuffer, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat)
{
	memset(this, 0x00, sizeof(*this));
	m_BitSizeInteger = BitSizeInteger, m_BitSizeFloat = BitSizeFloat;
	if (ValidBcd(pbcdBuffer, (BitSizeInteger + BitSizeFloat + 1) / 2))
		m_ValidBcd = true;
	else
		m_ValidBcd = false;
		
	if (m_ValidBcd == true && BitSizeInteger <= 8 && BitSizeFloat <= 4)
	{
		INT64U dwBCD = 0;
		memcpy(&dwBCD, pbcdBuffer, (BitSizeInteger + BitSizeFloat + 1) / 2);
		dwBCD <<= (4 - BitSizeFloat) * 4;
		S_BYTE *pdwBCD = (S_BYTE *)&dwBCD;

		S_DWORD dwInter;
		memcpy(&dwInter, pdwBCD + 2, 4);
		m_Integer = Convert(dwInter);

		S_WORD wFloat;
		memcpy(&wFloat, pdwBCD, 2);
		m_Float = (S_WORD)Convert(wFloat);
	}
}

C_BCD::C_BCD(const C_BCD &r)
{
	if (this != &r)
	{
		m_ValidBcd = r.m_ValidBcd;
		m_Integer = r.m_Integer, m_Float = r.m_Float;
		m_BitSizeInteger = r.m_BitSizeInteger, m_BitSizeFloat = r.m_BitSizeFloat;
		memset(m_bcdBuffer, 0x00, sizeof(m_bcdBuffer));
	}
}

C_BCD::~C_BCD()
{
}

/** @brief 取BCD数据
 *  @return 返回格式为前4位小数后8位整数，小端方式
 */
const S_BYTE *C_BCD::GetBcdData(void)const
{
	m_bcdBuffer[0] = hai_cbcd((S_BYTE)(m_Float%100));
	m_bcdBuffer[1] = hai_cbcd((S_BYTE)(m_Float/100));

	S_DWORD integer = m_Integer;
	m_bcdBuffer[2] = hai_cbcd((S_BYTE)(integer%100)); integer /= 100;
	m_bcdBuffer[3] = hai_cbcd((S_BYTE)(integer%100)); integer /= 100;
	m_bcdBuffer[4] = hai_cbcd((S_BYTE)(integer%100)); integer /= 100;
	m_bcdBuffer[5] = hai_cbcd((S_BYTE)(integer%100)); integer /= 100;
	return m_bcdBuffer;
}

/** @brief 取BCD数据
 *  @param[out] Integer 16进制整数
 *  @param[out] Float 16进制小数
 *  @return void
 */
void C_BCD::GetBcdData(S_DWORD &Integer, S_WORD &Float)const
{
	Integer = m_Integer, Float = m_Float;
}

bool C_BCD::operator!(void)
{
	return m_ValidBcd == false;
}

C_BCD &C_BCD::operator=(const C_BCD &r)
{
	if (this != &r)
	{
		m_ValidBcd = r.m_ValidBcd;
		m_Integer = r.m_Integer, m_Float = r.m_Float;
		m_BitSizeInteger = r.m_BitSizeInteger, m_BitSizeFloat = r.m_BitSizeFloat;
		memcpy(m_bcdBuffer, r.m_bcdBuffer, sizeof(m_bcdBuffer));
	}
	return *this;
}

C_BCD &C_BCD::operator+=(const C_BCD &r)
{
	Correct(m_Integer+r.m_Integer, m_Float+r.m_Float);
	return *this;
}

C_BCD &C_BCD::operator+=(S_DWORD val)
{
	Correct(m_Integer+val, m_Float+0);
	return *this;
}

C_BCD &C_BCD::operator-=(const C_BCD &r)
{
	S_DWORD Integer = m_Integer, Float = m_Float;
	if (Float < r.m_Float)
	{
		Integer -= 1;
		Float += 10000;
	}
	Correct(Integer-r.m_Integer, Float-r.m_Float);
	return *this;
}

C_BCD &C_BCD::operator-=(S_DWORD val)
{
	Correct(m_Integer-val, m_Float-0);
	return *this;
}

C_BCD &C_BCD::operator*=(const C_BCD &r)
{// a.b * c.d = a*c.d + b*c + b*d
	C_BCD bcd_1(r);
	C_BCD bcd_2(0, m_Float); 
	bcd_1 *= m_Integer;
	bcd_2 *= r.m_Integer;
	S_DWORD FloatData = ((S_DWORD)m_Float * r.m_Float) / 10000;
	Correct(bcd_1.m_Integer+bcd_2.m_Integer, bcd_1.m_Float+bcd_2.m_Float+FloatData);
	return *this;
}

C_BCD &C_BCD::operator*=(S_DWORD val)
{
	Correct(m_Integer*val, m_Float*val);
	return *this;
}

C_BCD &C_BCD::operator/=(const C_BCD &r)
{// a.b / c.d = (a.b * 10000 * 10000) / (c.d * 10000) 
	INT64U LocalInteger = (INT64U)m_Integer * 10000 + m_Float; // 转成整数
	INT64U rInteger = (INT64U)r.m_Integer * 10000 + r.m_Float; // 转成整数
	LocalInteger *= 10000; //扩大10000倍
	if (rInteger != 0x00)
	{// ???TODO:
		LocalInteger /= rInteger; // 除法
		Correct((S_DWORD)(LocalInteger/10000), (S_DWORD)(LocalInteger%10000));
	}
	return *this;
}

C_BCD &C_BCD::operator/=(S_DWORD val)
{
	C_BCD r(val, 0x00);
	return operator/=(r);
}

C_BCD::operator double()const
{
	return m_Integer + m_Float * 0.0001;
}

C_BCD::operator string()const
{
	string strBcdAscii;
	size_t BitSizeInteger = 6, BitSizeFloat = 2;

	if (m_BitSizeInteger != 0) BitSizeInteger = m_BitSizeInteger;
	if (m_BitSizeFloat != 0) BitSizeFloat = m_BitSizeFloat;

	if (m_ValidBcd == true)
		PrintAscii(strBcdAscii);
	else
		strBcdAscii.append(BitSizeInteger, '-').append((size_t)1, (char)'.').append(BitSizeFloat, '-');

	return strBcdAscii;
}

bool C_BCD::ValidBcd(const S_BYTE *pbcdBuffer, S_WORD ByteSize)
{
	if (pbcdBuffer == NULL)
		return false;
	for (S_WORD i = 0; i < ByteSize; i++)
	{
		if ((pbcdBuffer[i]&0xF0) > 0x90 || (pbcdBuffer[i]&0x0F) > 0x09)
			return false;
	}
	return true;
}

bool C_BCD::ValidBcd(const string &strBcdAscii)
{
	for (S_WORD i = 0; i < strBcdAscii.size(); i++)
	{
		if (strBcdAscii[i] > '9' || strBcdAscii[i] < '0')
			return false;
	}
	return true;
}

S_DWORD C_BCD::Convert(S_DWORD dwBCD)
{
	S_DWORD sum = 0;
	for (int i = 0; i < 8; i++)
		sum += ((dwBCD >> (i * 4)) & 0x0F) * POWER[i];
	return sum;
}

S_DWORD C_BCD::Convert(const string &strBcdAscii)
{
	S_DWORD sum = 0;
	string strMyBcd(strBcdAscii);
	reverse_copy(strBcdAscii.begin(), strBcdAscii.end(), strMyBcd.begin());

	for (S_WORD i = 0; i < 8 && i < strMyBcd.size(); i++)
		sum += (strMyBcd[i] - '0') * POWER[i];
	return sum;
}

void C_BCD::Correct(S_DWORD Integer, S_DWORD Float)
{
	S_DWORD carry = Float / 10000;

	m_Float   = (S_WORD)(Float % 10000);
	m_Integer = Integer + carry;
}

bool C_BCD::PrintAscii(string &strBcdAscii)const
{
	S_BYTE buffer[6];
	const S_BYTE *p = GetBcdData();
	reverse_copy(p, p+6, buffer);
	for (int i = 0; i < 6; i++)
	{
		strBcdAscii.push_back(hai_Hex2Char((buffer[i]>>4)&0x0F));
		strBcdAscii.push_back(hai_Hex2Char((buffer[i]>>0)&0x0F));
		if (i == 3) strBcdAscii.push_back('.');
	}
	string::size_type start = strBcdAscii.find_first_not_of('0');
	if (start != string::npos)
	{
		if (strBcdAscii[start] == '.')
			start -= 1;
		strBcdAscii.erase(0, start);//去除前面多余的0
	}
	string::size_type tail = strBcdAscii.find_last_not_of('0');
	if (tail != string::npos)
	{
		if (strBcdAscii[tail] == '.')
			tail += 1;
		strBcdAscii.erase(tail+1, string::npos);//去除后面多余的0
	}

	return true;
}

//把默认的8.4格式转换为 6.4、 6.2、 4.2、2.4?1.3、3.3、3.1等
bool C_BCD::Format(S_DWORD Integerbit, S_WORD Floatbit,string &formatDataStr)
{
	const S_BYTE *pbcd =  GetBcdData();
	formatDataStr.clear();
		
	if (Integerbit == 8 && Floatbit == 2)
	{
		//float
		formatDataStr.push_back(pbcd[1]); 	

		//integer
		formatDataStr.push_back(pbcd[2]); 	
		formatDataStr.push_back(pbcd[3]); 	
		formatDataStr.push_back(pbcd[4]); 	
		formatDataStr.push_back(pbcd[5]); 	
		return true;	
	}
	else if (Integerbit == 6 && Floatbit == 4)
	{
		//float
		formatDataStr.push_back(pbcd[0]); 	
		formatDataStr.push_back(pbcd[1]); 	

		//integer
		formatDataStr.push_back(pbcd[2]); 	
		formatDataStr.push_back(pbcd[3]); 	
		formatDataStr.push_back(pbcd[4]); 	
		return true;	
	}
	else if (Integerbit == 6 && Floatbit == 2)
	{
		//float
		formatDataStr.push_back(pbcd[1]); 	

		//integer
		formatDataStr.push_back(pbcd[2]); 	
		formatDataStr.push_back(pbcd[3]); 	
		formatDataStr.push_back(pbcd[4]); 	
		return true;	
	}
	else if (Integerbit == 4 && Floatbit == 2)
	{
		//float
		formatDataStr.push_back(pbcd[1]); 	

		//integer
		formatDataStr.push_back(pbcd[2]); 	
		formatDataStr.push_back(pbcd[3]); 	
		return true;	
	}
	else if (Integerbit == 3 && Floatbit == 3)
	{
		formatDataStr.push_back((pbcd[1]&0x0f<<4)|(pbcd[0]&0xf0>>4)); 	
		formatDataStr.push_back((pbcd[2]&0x0f<<4)|(pbcd[1]&0xf0>>4)); 	
		formatDataStr.push_back((pbcd[3]&0x0f<<4)|(pbcd[2]&0xf0>>4)); 	
		return true;	
	}
	else if (Integerbit == 3 && Floatbit == 1)
	{
		formatDataStr.push_back((pbcd[2]&0x0f<<4)|(pbcd[1]&0xf0>>4)); 	
		formatDataStr.push_back((pbcd[3]&0x0f<<4)|(pbcd[2]&0xf0>>4)); 	
		return true;	
	}
	else if (Integerbit == 2 && Floatbit == 4)
	{
		//float
		formatDataStr.push_back(pbcd[0]); 	
		formatDataStr.push_back(pbcd[1]); 	

		//integer
		formatDataStr.push_back(pbcd[2]); 	
		return true;	
	}
	else if (Integerbit == 1 && Floatbit == 3)
	{
		formatDataStr.push_back((pbcd[1]&0x0f<<4)|(pbcd[0]&0xf0>>4)); 	
		formatDataStr.push_back((pbcd[2]&0x0f<<4)|(pbcd[1]&0xf0>>4)); 	
		return true;	
	}


	return false;
}

/** @brief 转换ASCII字串成BCD数据
 *  @return BCD数据缓冲区
 */
const S_BYTE *C_BCD::ConvAsciiToBCD(const string& strdata, string& bcdData)
{
	S_BYTE tempValue = 0x00;
	string srcdata = strdata;

	reverse(srcdata.begin(), srcdata.end());

	if (srcdata.size()%2 != 0)
	{
		srcdata.push_back((S_BYTE)'0');
	}
	
	for (int i=0; i < (int)srcdata.size(); i++)
	{
		if (i%2 == 0)
		{
			tempValue |= (srcdata[i] - '0')&0x0f;
		}

		if (i%2 != 0)
		{
			tempValue |= ((srcdata[i] - '0')&0x0f)<<4;	
			bcdData.push_back(tempValue);
			tempValue = 0x00;
		}
	}
		
	return (S_BYTE *)(bcdData.data());
}

//nonmember
C_BCD operator+(const C_BCD &left, const C_BCD &right)
{
	C_BCD MyBcd(left);
	MyBcd += right;
	return MyBcd;
}

C_BCD operator+(const C_BCD &left, S_DWORD val)
{
	C_BCD MyBcd(val, 0);
	MyBcd += left;
	return MyBcd;
}

C_BCD operator-(const C_BCD &left, const C_BCD &right)
{
	C_BCD MyBcd(left);
	MyBcd -= right;
	return MyBcd;
}

C_BCD operator-(const C_BCD &left, S_DWORD val)
{
	C_BCD MyBcd(left), right(val, 0);
	MyBcd -= right;
	return MyBcd;
}

C_BCD operator*(const C_BCD &left, const C_BCD &right)
{
	C_BCD MyBcd(left);
	MyBcd *= right;
	return MyBcd;
}

C_BCD operator*(const C_BCD &left, S_DWORD val)
{
	C_BCD MyBcd(left);
	MyBcd *= val;
	return MyBcd;
}

C_BCD operator/(const C_BCD &left, const C_BCD &right)
{
	C_BCD MyBcd(left);
	MyBcd /= right;
	return MyBcd;
}

C_BCD operator/(const C_BCD &left, S_DWORD val)
{
	C_BCD MyBcd(left);
	MyBcd /= val;
	return MyBcd;
}

bool operator>(const C_BCD &left, const C_BCD &right)
{
	if (left.m_Integer > right.m_Integer)
		return true;
	else if (left.m_Integer == right.m_Integer && left.m_Float > right.m_Float)
		return true;
	return false;
}

bool operator>=(const C_BCD &left, const C_BCD &right)
{
	if (left > right) return true;
	else if (left == right) return true;
	return false;
}

bool operator<(const C_BCD &left, const C_BCD &right)
{
	if (left.m_Integer < right.m_Integer)
		return true;
	else if (left.m_Integer == right.m_Integer && left.m_Float < right.m_Float)
		return true;
	return false;
}

bool operator<=(const C_BCD &left, const C_BCD &right)
{
	if (left < right) return true;
	else if (left == right) return true;
	return false;
}

bool operator==(const C_BCD &left, const C_BCD &right)
{
	return left.m_Integer == right.m_Integer && left.m_Float == right.m_Float;
}

bool operator!=(const C_BCD &left, const C_BCD &right)
{
	return left.m_Integer != right.m_Integer || left.m_Float != right.m_Float;
}

//
// BCD串处理集
//
C_BcdString::C_BcdString(const string &strBcd)
{
	if (VerifyBcd(strBcd) == true)
		append(strBcd);
	else
		append((size_t)strBcd.size(), (char)0x00);
}

C_BcdString::C_BcdString(const string &strBcd, bool fVerifyBcd)
{
	if (fVerifyBcd == false)
		append(strBcd);
	else if (VerifyBcd(strBcd) == true)
		append(strBcd);
	else
		append((size_t)strBcd.size(), (char)0x00);
}

C_BcdString::C_BcdString(S_DWORD val)
{
	for (int i = 0; i < 4; i++)
		push_back(hai_cbcd((S_BYTE)(val%100))), val /= 100;
}

C_BcdString::C_BcdString(INT64U val)
{
	for (int i = 0; i < 5; i++)
		push_back(hai_cbcd((S_BYTE)(val%100))), val /= 100;
}

C_BcdString &C_BcdString::operator<<(S_WORD BcdBit)
{
	for (S_WORD i = 0; i < BcdBit; i++)
	{
		for (string::iterator iter = begin(); iter < end(); ++iter)
		{
			char cur = *iter, fill = 0;
			if (iter+1 < end())
				fill = *(iter+1);
			*iter = ((cur>>4)&0x0F) | ((fill<<4)&0xF0);
		}
	}
	return *this;
}

C_BcdString &C_BcdString::operator>>(S_WORD BcdBit)
{
	for (S_WORD i = 0; i < BcdBit; i++)
	{
		for (string::reverse_iterator iter = rbegin(); iter < rend(); ++iter)
		{
			char cur = *iter, fill = 0;
			if (iter+1 < rend())
				fill = *(iter+1);
			*iter = ((cur<<4)&0xF0) | ((fill>>4)&0x0F);
		}
	}
	return *this;
}

S_DWORD C_BcdString::transform(S_DWORD val)
{
	if (size() > 0) val += (S_DWORD)hai_bcdc(at(0)) * 1;
	if (size() > 1) val += (S_DWORD)hai_bcdc(at(1)) * 100; 
	if (size() > 2) val += (S_DWORD)hai_bcdc(at(2)) * 10000;
	if (size() > 3) val += (S_DWORD)hai_bcdc(at(3)) * 1000000;
	return val;
}

INT64U C_BcdString::transform(INT64U val)
{
	if (size() > 0) val += (INT64U)hai_bcdc(at(0)) * 1;
	if (size() > 1) val += (INT64U)hai_bcdc(at(1)) * 100; 
	if (size() > 2) val += (INT64U)hai_bcdc(at(2)) * 10000;
	if (size() > 3) val += (INT64U)hai_bcdc(at(3)) * 1000000;
	if (size() > 4) val += (INT64U)hai_bcdc(at(4)) * 100000000;
	return val;
}

S_WORD C_BcdString::PrintAscii(string &strAscii)
{
	return PrintAscii(strAscii, ' ');
}

S_WORD C_BcdString::PrintAscii(string &strAscii, char delimit)
{
	for (size_t i = 0; i < size(); i++)
	{
		S_BYTE val = at(i);
		strAscii.push_back(hai_Hex2Char(val>>4));
		strAscii.push_back(hai_Hex2Char(val&0x0F));
		if (delimit != 0) strAscii.push_back(delimit);
	}
	return (S_WORD)strAscii.size();
}

string C_BcdString::PrintAscii(S_BYTE BitSizeInteger, S_BYTE BitSizeFloat, bool fSign)
{
	string strAscii;
	string strData(size(), 0);
	reverse_copy(begin(), end(), strData.begin());
	if (VerifyBcd(strData) == false)
	{
		strAscii.append((size_t)BitSizeInteger, '-').append(".").append((size_t)BitSizeFloat, '-');
		return strAscii;
	}

	bool fNegative = false;
	if (fSign == true)
	{
		if (strData[0] & 0x80)
			fNegative = true;
		strData[0] &= 0x7F; //clear sign
	}

	for (size_t i = 0; i < strData.size(); i++)
	{
		strAscii.push_back(hai_Hex2Char((strData[i]>>4)&0x0F));
		strAscii.push_back(hai_Hex2Char((strData[i]>>0)&0x0F));
	}

	if (BitSizeInteger < strAscii.size())
		strAscii.insert(BitSizeInteger, ".");

	string::size_type start = strAscii.find_first_not_of('0');
	if (start != string::npos)
	{
		if (strAscii[start] == '.')
			start -= 1;
		strAscii.erase(0, start);//去除前面多余的0
	}

	if (fNegative == true)
		strAscii.insert(0, "-");
	return strAscii;
}

bool C_BcdString::VerifyBcd(const string &strData)
{
	for (S_WORD i = 0; i < strData.size(); i++)
	{
		if ((strData[i]&0xF0) > 0x90 || (strData[i]&0x0F) > 0x09)
			return false;
	}
	return true;
}

/*
void C_BcdString_test(void)
{
	C_BcdString strData1("123456");
	C_BcdString strData2("123456");
	C_BcdString strData3("123456");
	C_BcdString strData4("123456");
	C_BcdString strData5("123456");
	C_BcdString strData6("123456");
	C_BcdString strData7("123456");
	C_BcdString strData8("123456");

	strData1>>1;
	strData2>>2;
	strData3>>3;
	strData4>>14;
	strData5<<1;
	strData6<<2;
	strData7<<3;
	strData8<<14;

	S_BYTE data1[] = {0xEE,0xEE,0xEE,0xEE};
	S_BYTE data2[] = {0x12,0x34,0x56,0x88};
	S_BYTE data3[] = {0x12,0x34,0x56,0x00};
	S_BYTE data4[] = {0x00,0x34,0x56,0x00};

	C_BcdString bcdString1(string((char *)data1, 4), false);
	C_BcdString bcdString2(string((char *)data2, 4), false);
	C_BcdString bcdString3(string((char *)data3, 4), false);
	C_BcdString bcdString4(string((char *)data4, 4), false);

	string strNum1 = bcdString1.PrintAscii(6, 2, true);
	string strNum2 = bcdString2.PrintAscii(6, 2, true);
	string strNum3 = bcdString3.PrintAscii(6, 2, true);
	string strNum4 = bcdString4.PrintAscii(6, 2, true);
	string strNum5 = bcdString4.PrintAscii(4, 4, true);
}
*/
/*
void C_BCD_test(void)
{
	C_BCD mybcd1(0.2345);
	C_BCD mybcd2(345.0001);
	C_BCD mybcd3(90.0123);
	C_BCD mybcd4(7889.00008);

	C_BCD mybcd(6, 1);
	C_BCD bcd01("0.1"), bcd12("1.2"), bcd14("1.4"), bcd21("2.1"), bcd00("0.0"), bcd99("99.9999"), bcd89("8.9");

	C_BCD bcdA = mybcd + (S_DWORD)5;
	C_BCD bcdB = bcd01 + bcd12;
	C_BCD bcdC = bcd12 + bcd00;
	C_BCD bcdD = bcd12 + bcd99;
	C_BCD bcdE = bcd14 - bcd12;
	C_BCD bcdF = bcd21 - bcd14;
	C_BCD bcdG = bcd21 - bcd00;
	C_BCD bcdH = bcd89 * bcd89;
	C_BCD bcdI = bcd99 * bcd99;
	C_BCD bcdJ = bcd99 * bcd89;
	C_BCD bcdK = bcd89 * bcd00;
	C_BCD bcdL = bcd89 * bcd01;
	C_BCD bcdM = bcd14 * bcd21;

	C_BCD bcdN = bcd89 / bcd89;
	C_BCD bcdO = bcd99 / bcd99;
	C_BCD bcdP = bcd99 / bcd89;
	C_BCD bcdQ = bcd89 / bcd00;
	C_BCD bcdR = bcd89 / bcd01;
	C_BCD bcdS = bcd14 / bcd21;

	string strBcdA = bcd00;
	string strBcdB = bcd99;
	const S_BYTE *pBcdA = bcd00.GetBcdData();
	const S_BYTE *pBcdB = bcd99.GetBcdData();
}
*/



