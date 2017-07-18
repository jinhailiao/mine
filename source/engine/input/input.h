/** @file
 *  @brief  ‰»Î◊¥Ã¨–≈œ¢
 *  @author jinhailiao
 *  @date 2017/03/06
 *  @version 0.1
 *  @see 
 */
#ifndef __MINE_INPUT_H__
#define __MINE_INPUT_H__
#include "haibase.h"

enum
{
	MOUSE_STATE_NOTHING = 0,
	MOUSE_STATE_CLICK,
	MOUSE_STATE_DBCLICK,

	MOUSE_STATE_MAX
};

class C_INPUT
{
public:
	virtual ~C_INPUT();
	static C_INPUT &GetInstance(void);

	S_WORD GetMouseState(S_DWORD &position);
	S_WORD SetMouseState(S_WORD state, S_DWORD position);

protected:
	C_INPUT(void);

protected:
	S_WORD m_state;
	S_DWORD m_position;
};

#endif//__MINE_INPUT_H__

