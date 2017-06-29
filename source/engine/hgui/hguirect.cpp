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
#include "hguirect.h"

C_HGUIRECT::C_HGUIRECT(void)
{
	m_rect.x = 0, m_rect.y = 0;
	m_rect.w = 0, m_rect.h = 0;
}

C_HGUIRECT::C_HGUIRECT(const S_RECT &rect)
{
	m_rect.x = rect.x, m_rect.y = rect.y;
	m_rect.w = rect.w, m_rect.h = rect.h;
}

C_HGUIRECT::C_HGUIRECT(const C_HGUIRECT &HGuiRect)
{
	m_rect.x = HGuiRect.m_rect.x, m_rect.y = HGuiRect.m_rect.y;
	m_rect.w = HGuiRect.m_rect.w, m_rect.h = HGuiRect.m_rect.h;
}

C_HGUIRECT::~C_HGUIRECT(void)
{
}

bool C_HGUIRECT::Intersect(const S_RECT &rect)const
{
	S_WORD l =  (m_rect.x > rect.x)? (m_rect.x):(rect.x);
	S_WORD t =  (m_rect.y > rect.y)? (m_rect.y):(rect.y);
	S_WORD r =  (m_rect.x+m_rect.w < rect.x+rect.w)? (m_rect.x+m_rect.w):(rect.x+rect.w);
	S_WORD b =  (m_rect.y+m_rect.h < rect.y+rect.h)? (m_rect.y+m_rect.h):(rect.y+rect.h);
	return (l<r && t<b);
}

bool C_HGUIRECT::Add(const S_RECT &rect)
{
	S_WORD l =  (m_rect.x < rect.x)? (m_rect.x):(rect.x);
	S_WORD t =  (m_rect.y < rect.y)? (m_rect.y):(rect.y);
	S_WORD r =  (m_rect.x+m_rect.w > rect.x+rect.w)? (m_rect.x+m_rect.w):(rect.x+rect.w);
	S_WORD b =  (m_rect.y+m_rect.h > rect.y+rect.h)? (m_rect.y+m_rect.h):(rect.y+rect.h);

	m_rect.x = l;
	m_rect.y = t;
	m_rect.w = r-l;
	m_rect.h = b-t;

	return true;
}

bool C_HGUIRECT::PointInRect(S_WORD x, S_WORD y)const
{
	if (x >= m_rect.x && x < m_rect.x+m_rect.w 
			&& y >= m_rect.y && y < m_rect.y+m_rect.h)
		return true;
	return false;
}

bool C_HGUIRECT::Covered(const S_RECT &rect)const
{
	if (m_rect.x <= rect.x && m_rect.x+m_rect.w >= rect.x+rect.w
			&& m_rect.y <= rect.y && m_rect.y+m_rect.h >= rect.y+rect.h)
		return true;
	return false;
}

bool C_HGUIRECT::Intersect(const C_HGUIRECT &HGuiRect)const
{
	return Intersect(HGuiRect.m_rect);
}

bool C_HGUIRECT::Add(const C_HGUIRECT &HGuiRect)
{
	return Add(HGuiRect.m_rect);
}

bool C_HGUIRECT::Covered(const C_HGUIRECT &HGuiRect)const
{
	return Covered(HGuiRect.m_rect);
}



