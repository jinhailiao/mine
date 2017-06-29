//---------------------------------------------------------------------------------
// Copyright jinhailiao 2009-2010
// E-mail:   jinhailiao@163.com
// Project:			HGUI
// File:			hguirect.h
// Description:
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
// 2.0.0		2017-06-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __HGUI_RECT_H__
#define __HGUI_RECT_H__
#include "haibase.h"

class C_HGUIRECT
{
public:
	C_HGUIRECT(void);
	C_HGUIRECT(const S_RECT &rect);
	C_HGUIRECT(const C_HGUIRECT &HGuiRect);
	~C_HGUIRECT(void);

	bool Intersect(const S_RECT &rect)const;
	bool Add(const S_RECT &rect);
	bool PointInRect(S_WORD x, S_WORD y)const;
	bool Covered(const S_RECT &rect)const;
	bool Intersect(const C_HGUIRECT &HGuiRect)const;
	bool Add(const C_HGUIRECT &HGuiRect);
	bool Covered(const C_HGUIRECT &HGuiRect)const;
	
protected:
	S_RECT m_rect;
};

#endif //__HGUI_RECT_H__

