/** @file
 *  @brief 基本定义
 *  @author jinhailiao
 *  @date 2011/10/22
 *  @version 0.1
 *  1、基本类型宏定义
 *  2、基本库文件包含
 */
#ifndef __HAI_BASEDEF_H__
#define __HAI_BASEDEF_H__

#include "hconfig.h"

//
//type define
//
typedef void				S_VOID;
typedef char				S_CHAR;
typedef short				S_SHORT;
typedef long				S_LONG;
typedef int				S_INT;
typedef unsigned char		S_BYTE;
typedef unsigned short		S_WORD;
typedef unsigned long		S_DWORD;
typedef unsigned int		S_UINT;

typedef unsigned long long		INT64U;
typedef signed long long		INT64S;
typedef float					FP32;
typedef double				FP64;

#define INT8U	S_BYTE
#define INT16U	S_WORD
#define INT32U	S_DWORD
#define INT8S	S_CHAR
#define INT16S	S_SHORT
#define INT32S	S_LONG

#define S_TIME	S_DWORD
#define S_BOOL	bool
#define S_NULL	((void *)0)

//
//base struct define
//
typedef struct tagPoint
{
	S_SHORT  x;
	S_SHORT  y;
}
S_POINT, *SP_POINT;

typedef struct tagRect
{
	S_WORD  x;
	S_WORD  y;
	S_WORD  w;
	S_WORD  h;
}
S_RECT, *SP_RECT;

typedef struct tagRgn
{
	S_SHORT  left;
	S_SHORT  top;
	S_SHORT  right;
	S_SHORT  bottom;
}
S_RGN, *SP_RGN;

typedef struct tagLine
{
	S_SHORT  x0;
	S_SHORT  y0;
	S_SHORT  x1;
	S_SHORT  y1;
}
S_LINE, *SP_LINE;

typedef struct tagArea
{
	S_SHORT  x;
	S_SHORT  y;
	S_SHORT  cx;
	S_SHORT  cy;
}
S_AREA, *SP_AREA;

//
//general function
//
/** @brief 将p指向的内存按小端方式组合成WORD */
inline S_WORD HAI_MAKEWORD(const S_BYTE *p)
{
	return ((((S_WORD)(*(p)))&0xFF)|((((S_WORD)(*((p)+1)))<<8)&0xFF00));
}
/** @brief 将p指向的内存按小端方式组合成DWORD */
inline S_DWORD HAI_MAKEDWORD(const S_BYTE *p)
{
	return ((((S_DWORD)(*(p)))&0xFF)|((((S_DWORD)(*((p)+1)))<<8)&0xFF00)|((((S_DWORD)(*((p)+2)))<<16)&0xFF0000)|((((S_DWORD)(*((p)+3)))<<24)&0xFF000000));
}
/** @brief 将p指向的内存按小端方式组合成DWORD */
inline S_DWORD HAI_MAKEDWORD3(const S_BYTE *p)
{
	return ((((S_DWORD)(*(p)))&0xFF)|((((S_DWORD)(*((p)+1)))<<8)&0xFF00)|((((S_DWORD)(*((p)+2)))<<16)&0xFF0000));
}
/** @brief 合并BYTE */
inline S_WORD HAI_COMBWORD(S_BYTE h,S_BYTE l)
{
	return ((((S_WORD)(h))<<8)|((S_WORD)(l)));
}
/** @brief 合并WORD */
inline S_DWORD HAI_COMBDWORD(S_WORD h,S_WORD l)
{
	return ((((S_DWORD)(h))<<16)|((S_DWORD)(l)));
}
/** @brief 将WORD按小端方式写入p指向的内存 */
inline void HAI_WRITEWORD(S_BYTE *p,S_WORD w)
{
	*(p)=(S_BYTE)((w)&0xFF),*((p)+1)=(S_BYTE)(((w)>>8)&0xFF);
}
/** @brief 将DWORD按小端方式写入p指向的内存 */
inline void HAI_WRITEDWORD(S_BYTE *p, S_DWORD dw)
{
	*(p)=(S_BYTE)((dw)&0xFF),*((p)+1)=(S_BYTE)(((dw)>>8)&0xFF),*((p)+2)=(S_BYTE)(((dw)>>16)&0xFF),*((p)+3)=(S_BYTE)(((dw)>>24)&0xFF);
}
/** @brief 将DWORD按小端方式写入p指向的内存 */
inline void HAI_WRITEDWORD3(S_BYTE *p, S_DWORD dw)
{
	*(p)=(S_BYTE)((dw)&0xFF),*((p)+1)=(S_BYTE)(((dw)>>8)&0xFF),*((p)+2)=(S_BYTE)(((dw)>>16)&0xFF);
}
/** @brief WORD高低BYTE交换 */
inline S_WORD HAI_SWAPWORD(S_WORD w)
{
	return ((((w)>>8)&0xFF)|(((w)<<8)&0xFF00));
}
/** @brief DWORD高低BYTE交换 */
inline S_DWORD HAI_SWAPDWORD(S_DWORD dw)
{
	return ((((dw)>>24)&0xFF)|(((dw)>>8)&0xFF00)|(((dw)<<8)&0xFF0000)|(((dw)<<24)&0xFF000000));
}
/** @brief 取高BYTE */
inline S_BYTE HAI_GETHIBYTE(S_WORD w)
{
	return ((S_BYTE)(((w)>>8)&0xFF));
}
/** @brief 取低BYTE */
inline S_BYTE HAI_GETLOBYTE(S_WORD w)
{
	return ((S_BYTE)((w)&0xFF));
}
/** @brief 取高WORD */
inline S_WORD HAI_GETHIWORD(S_DWORD dw)
{
	return ((S_WORD)(((dw)>>16)&0xFFFF));
}
/** @brief 取低WORD */
inline S_WORD HAI_GETLOWORD(S_DWORD dw)
{
	return ((S_WORD)((dw)&0xFFFF));
}

/** @brief 16进制转换为BCD码
 *  @param[in] v 0-99
 *  @return 16进制对应的BCD码
 */
inline S_BYTE hai_cbcd(S_BYTE v)
{
	return ((((v)%100/10)<<4)+((v)%10));
}
/** @brief BCD码转换为16进制
 *  @param[in] v 0x00-0x99(BCD)
 *  @return BCD码对应的16进制数值
 */
inline S_BYTE hai_bcdc(S_BYTE v)
{
	return ((((v)>>4)&0x0f)*10+((v)&0x0f));
}
/** @brief 取数字ASCII码的数值
 *  @param[in] ch '0'-'F' or '0'-'f'
 *  @return 数字ASCII码对应的数值
 */
inline S_BYTE hai_Char2Hex(S_CHAR ch)
{
	if (ch >= '0' && ch <= '9')
		return (ch - '0');
	else if (ch >= 'A' && ch <= 'F')
		return (ch - 'A' + 0x0A);
	else if (ch >= 'a' && ch <= 'f')
		return (ch - 'a' + 0x0A);
	return 0;
}
/** @brief 数值转成ASCII
 *  @param[in] hex 0-15
 *  @return '0'-'F'
 */
inline S_CHAR hai_Hex2Char(S_BYTE hex)
{
	if (hex <= 0x09)
		return ('0' + hex);
	else if (hex <= 0x0F)
		return ('A' + (hex - 0x0A));
	return '0';
}

/** @brief 16进制转换为BCD码
 *  @param[in] v 0-9999
 *  @return 16进制对应的BCD码
 */
inline S_WORD hai_Word2Bcd(S_WORD v)
{
	return HAI_COMBWORD(hai_cbcd((S_BYTE)(v/100)), hai_cbcd((S_BYTE)(v%100)));
}
/** @brief BCD码转换为16进制
 *  @param[in] bcdH 0x00-0x99(BCD)
 *  @param[in] bcdL 0x00-0x99(BCD)
 *  @return BCD码对应的16进制数值
 */
inline S_WORD hai_Bcd2Word(S_BYTE bcdH, S_BYTE bcdL)
{
	return hai_bcdc(bcdH) * 100 + hai_bcdc(bcdL);
}
/** @brief BCD码转换为16进制
 *  @param[in] bcd 0x00-0x9999(BCD)
 *  @return BCD码对应的16进制数值
 */
inline S_WORD hai_Bcd2Word(S_WORD bcd)
{
	return hai_Bcd2Word(HAI_GETHIBYTE(bcd), HAI_GETLOBYTE(bcd));
}

/** @brief 毫秒级睡眠函数 */
inline void SleepMS(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	usleep(ms*1000);
#endif
}

/** @brief 秒级睡眠函数 */
inline void SleepSecond(int Second)
{
#ifdef WIN32
	Sleep(Second*1000);
#else
	sleep(Second);
#endif
}

/** @brief 系统命令 */
inline int SystemCmd(const char *command)
{
#ifdef WIN32
	return 0;
#else
	int ok = system(command);
	SleepMS(1);
	return ok;
#endif
}

#define HAI_ABS(n)			((n) < 0 ? (-(n)) : (n))
#define HAI_MIN(a, b)		((a) < (b) ? (a) : (b))
#define HAI_MAX(a, b)		((a) > (b) ? (a) : (b))
#define HAI_MEMBER_OF(array)		(sizeof(array)/sizeof(array[0]))


#endif//__HAI_BASEDEF_H__

