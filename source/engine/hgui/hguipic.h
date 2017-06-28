//---------------------------------------------------------------------------------
// Copyright (c) Haisoft 2006-9-1
// Copyright jinhailiao 2008-2010
// E-mail:   jinhailiao@163.com
// Project:			HGui
// File:			hguipic.h
// Description:		
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 2.0.0		2006-09-06  create                  Kingsea
// 3.0.0		2008-03-01	update					Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_PIC_H__
#define __HGUI_PIC_H__
#include "haibase.h"

#define BITMAP_WIDTHBYTES(w, bits) (((w*bits+31)>>5)<<2)

struct S_BmpInfo
{
	S_WORD m_width;
	S_WORD m_height;
	S_WORD m_BitsPixel;
	S_WORD	m_WidthBytes;

	S_BmpInfo(void){m_width=0,m_height=0,m_BitsPixel=0,m_WidthBytes=0;}
};

struct S_IconInfo
{
	S_WORD m_width;
	S_WORD m_height;
	S_WORD m_BitsPixel;
	S_WORD	m_WidthBytes;
	S_VOID *m_lpMask;

	S_IconInfo(void){m_width=0,m_height=0,m_BitsPixel=0,m_WidthBytes=0,m_lpMask=NULL;}
};

class C_HGUIPIC
{
public:
	/** @brief Bmp��Pcx����
	 *  @param[in] pxxxData ԭʼͼ������
	 *  @param[out] BmpInfo ������Ϣ
	 *  @return ==NULL ʧ��;!=NULL ��ʾ����(new�����ڴ�)
	 */
	S_VOID *BmpDecode(const S_VOID *pBmpData, S_BmpInfo &BmpInfo);
	S_VOID *PcxDecode(const S_VOID *pPcxData, S_BmpInfo &BmpInfo);

	/** @brief Icon����
	 *  @param[in] pIcoData ԭʼͼ������
	 *  @return Icon��֡����
	 */
	S_BYTE GetIconFrameTotal(const S_VOID *pIcoData);

	/** @brief Icon����
	 *  @param[in] pxxxData ԭʼͼ������
	 *  @param[out] BmpInfo ������Ϣ
	 *  @param[in] frame ֡���
	 *  @return ==NULL ʧ��;!=NULL ��ʾ����(new�����ڴ�)
	 */
	S_VOID *IconDecode(const S_VOID *pIcoData, S_IconInfo &IconInfo, S_BYTE frame);

protected:
	
};

#endif //__HGUI_PIC_H__

