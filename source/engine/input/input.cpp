/** @file
 *  @brief  ‰»Î◊¥Ã¨–≈œ¢
 *  @author jinhailiao
 *  @date 2017/03/06
 *  @version 0.1
 *  @see 
 */
#include "input.h"

//
//
//
C_INPUT::C_INPUT(void)
{
}

C_INPUT::~C_INPUT()
{
}

C_INPUT &C_INPUT::GetInstance(void)
{
	static C_INPUT s_MyInput;
	return s_MyInput;
}

S_WORD C_INPUT::GetMouseState(S_DWORD &position)
{
	S_WORD state = m_state;
	position = m_position;

	m_state = MOUSE_STATE_NOTHING;
	m_position = 0;
	return state;
}

S_WORD C_INPUT::SetMouseState(S_WORD state, S_DWORD position)
{
	m_state = state;
	m_position = position;
	return 0;
}


