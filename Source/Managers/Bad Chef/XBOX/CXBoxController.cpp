#include "CXBoxController.h"

CXBoxController::CXBoxController(unsigned int _playerNumber)
{
	m_nControllerNumber = _playerNumber;

	m_cTriggerThreshold    = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	m_nThumbDeadZone_Right = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	m_nThumbDeadZone_Left  = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	m_fDelta = 0;
	m_PreviousButton = 0;
}

XINPUT_STATE CXBoxController::GetState()
{
	ZeroMemory(&m_ControllerState, sizeof(XINPUT_STATE));
	XInputGetState(m_nControllerNumber, &m_ControllerState);

	return m_ControllerState;
}

bool CXBoxController::IsConnected()
{
	ZeroMemory(&m_ControllerState, sizeof(XINPUT_STATE));
	DWORD isConnected = XInputGetState(m_nControllerNumber, &m_ControllerState);

	if(isConnected == ERROR_SUCCESS)
		return true;
	else
		return false;
}

void CXBoxController::Vibrate(int _left, int _right, int _seconds)
{
	XINPUT_VIBRATION vibrate;

	ZeroMemory(&vibrate, sizeof(XINPUT_VIBRATION));

	vibrate.wLeftMotorSpeed = _left;
	vibrate.wRightMotorSpeed = _right;

	XInputSetState(m_nControllerNumber, &vibrate);
	m_fDelta = (float)(GetTickCount() + (long)(_seconds*1000));
}

void CXBoxController::Update()
{
	if(m_fDelta != 0 && GetTickCount() > m_fDelta)
		Vibrate();

	if(m_PreviousButton != 0 && GetTickCount() > m_fLastPress)
		m_PreviousButton = 0;
}

bool CXBoxController::IsDown_DPAD_UP()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			return true;

	return false;
}

bool CXBoxController::IsDown_DPAD_DOWN()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			return true;

	return false;
}

bool CXBoxController::IsDown_DPAD_LEFT()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			return true;

	return false;
}

bool CXBoxController::IsDown_DPAD_RIGHT()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			return true;

	return false;
}

bool CXBoxController::IsDown_START()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START)
			return true;

	return false;
}

bool CXBoxController::IsDown_BACK()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
			return true;

	return false;
}

bool CXBoxController::IsDown_THUMB_LEFT_Press()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
			return true;

	return false;
}

bool CXBoxController::IsDown_THUMB_RIGHT_Press()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
			return true;

	return false;
}

bool CXBoxController::IsDown_SHOULDER_LEFT()
{
	if(IsConnected())
		if( (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) )
				return true;

	return false;
}

bool CXBoxController::IsDown_SHOULDER_RIGHT()
{
	if(IsConnected())
		if( (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) )
				return true;

	return false;
}

bool CXBoxController::IsDown_TRIGGER_LEFT()
{
	if(IsConnected())
		if( (GetState().Gamepad.bLeftTrigger) &&
			(GetState().Gamepad.bLeftTrigger) > m_cTriggerThreshold)
				return true;

	return false;
}

bool CXBoxController::IsDown_TRIGGER_RIGHT()
{
	if(IsConnected())
		if( (GetState().Gamepad.bRightTrigger) &&
			(GetState().Gamepad.bRightTrigger) > m_cTriggerThreshold)
				return true;

	return false;
}

bool CXBoxController::IsDown_A()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
			return true;

	return false;
}

bool CXBoxController::IsDown_B()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
			return true;

	return false;
}

bool CXBoxController::IsDown_X()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
			return true;

	return false;
}

bool CXBoxController::IsDown_Y()
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			return true;

	return false;
}

bool CXBoxController::IsDown(WORD _button)
{
	if(IsConnected())
		if(GetState().Gamepad.wButtons & _button)
			return true;

	return false;
}

int CXBoxController::IsDown_THUMB_LEFT_X()
{
	if(IsConnected())
		if( (GetState().Gamepad.sThumbLX) )
			if( (GetState().Gamepad.sThumbLX) > m_nThumbDeadZone_Left)
				return 1;
			else if( (GetState().Gamepad.sThumbLX) < -m_nThumbDeadZone_Left)
				return -1;

	return false;
}

int CXBoxController::IsDown_THUMB_LEFT_Y()
{
	if(IsConnected())
		if( (GetState().Gamepad.sThumbLY) )
			if( (GetState().Gamepad.sThumbLY) > m_nThumbDeadZone_Left)
				return 1;
			else if( (GetState().Gamepad.sThumbLY) < -m_nThumbDeadZone_Left)
				return -1;

	return false;
}

int CXBoxController::IsDown_THUMB_RIGHT_X()
{
	if(IsConnected())
		if( (GetState().Gamepad.sThumbRX) )
			if( (GetState().Gamepad.sThumbRX) > m_nThumbDeadZone_Right)
				return 1;
			else if( (GetState().Gamepad.sThumbRX) < -m_nThumbDeadZone_Right)
				return -1;

	return false;
}

int CXBoxController::IsDown_THUMB_RIGHT_Y()
{
	if(IsConnected())
		if( (GetState().Gamepad.sThumbRY) )
			if( (GetState().Gamepad.sThumbRY) > m_nThumbDeadZone_Right)
				return 1;
			else if( (GetState().Gamepad.sThumbRY) < -m_nThumbDeadZone_Right)
				return -1;

	return false;
}

int CXBoxController::GetThumbLeft_X()
{
	if(IsConnected())
		if( (GetState().Gamepad.sThumbLX) )
			if( (GetState().Gamepad.sThumbLX) > m_nThumbDeadZone_Left)
				return (GetState().Gamepad.sThumbLX) - m_nThumbDeadZone_Left;
			else if( (GetState().Gamepad.sThumbLX) < -m_nThumbDeadZone_Left)
				return (GetState().Gamepad.sThumbLX) + m_nThumbDeadZone_Left;

	return false;
}

int CXBoxController::GetThumbLeft_Y()
{
	if(IsConnected())
		if( (GetState().Gamepad.sThumbLY) )
			if( (GetState().Gamepad.sThumbLY) > m_nThumbDeadZone_Left)
				return (GetState().Gamepad.sThumbLY) - m_nThumbDeadZone_Left;
			else if( (GetState().Gamepad.sThumbLY) < -m_nThumbDeadZone_Left)
				return (GetState().Gamepad.sThumbLY) + m_nThumbDeadZone_Left;

	return false;
}

int CXBoxController::GetThumbRight_X()
{
	if(IsConnected())
		if( (GetState().Gamepad.sThumbRX) )
			if( (GetState().Gamepad.sThumbRX) > m_nThumbDeadZone_Right)
				return (GetState().Gamepad.sThumbRX) - m_nThumbDeadZone_Right;
			else if( (GetState().Gamepad.sThumbRX) < -m_nThumbDeadZone_Right)
				return (GetState().Gamepad.sThumbRX) + m_nThumbDeadZone_Right;

	return false;
}

int CXBoxController::GetThumbRight_Y()
{
	if(IsConnected())
		if( (GetState().Gamepad.sThumbRY) )
			if( (GetState().Gamepad.sThumbRY) > m_nThumbDeadZone_Right)
				return (GetState().Gamepad.sThumbRY) - m_nThumbDeadZone_Right;
			else if( (GetState().Gamepad.sThumbRY) < -m_nThumbDeadZone_Right)
				return (GetState().Gamepad.sThumbRY) + m_nThumbDeadZone_Right;

	return false;
}

int CXBoxController::GetTriggerLeft()
{
	return (int)(GetState().Gamepad.bLeftTrigger);
}

int CXBoxController::GetTriggerRight()
{
	return (int)(GetState().Gamepad.bRightTrigger);
}

bool CXBoxController::IsPressed_DPAD_UP()
{
	if(IsDown_DPAD_UP() && m_PreviousButton != XINPUT_GAMEPAD_DPAD_UP)
	{
		m_PreviousButton = XINPUT_GAMEPAD_DPAD_UP;
		m_fLastPress = (float)(GetTickCount() + XBOX_TIMEBETWEEN);
		return true;
	}

	return false;
}

bool CXBoxController::IsPressed_DPAD_DOWN()
{
	if(IsDown_DPAD_DOWN() && m_PreviousButton != XINPUT_GAMEPAD_DPAD_DOWN)
	{
		m_PreviousButton = XINPUT_GAMEPAD_DPAD_DOWN;
		m_fLastPress = (float)(GetTickCount() + XBOX_TIMEBETWEEN);
		return true;
	}

	return false;
}

bool CXBoxController::IsPressed_DPAD_LEFT()
{
	if(IsDown_DPAD_LEFT() && m_PreviousButton != XINPUT_GAMEPAD_DPAD_LEFT)
	{
		m_PreviousButton = XINPUT_GAMEPAD_DPAD_LEFT;
		m_fLastPress = (float)(GetTickCount() + XBOX_TIMEBETWEEN);
		return true;
	}

	return false;
}

bool CXBoxController::IsPressed_DPAD_RIGHT()
{
	if(IsDown_DPAD_RIGHT() && m_PreviousButton != XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		m_PreviousButton = XINPUT_GAMEPAD_DPAD_RIGHT;
		m_fLastPress = (float)(GetTickCount() + XBOX_TIMEBETWEEN);
		return true;
	}

	return false;
}

bool CXBoxController::IsPressed_START()
{
	if(IsDown_START() && !(m_PreviousButton & XINPUT_GAMEPAD_START))
	{
		m_PreviousButton ^= XINPUT_GAMEPAD_START;
		m_fLastPress = (float)(GetTickCount() + XBOX_TIMEBETWEEN);
		return true;
	}

	return false;
}

bool IsPressed_BACK();
bool IsPressed_THUMB_LEFT_Press();
bool IsPressed_THUMB_RIGHT_Press();
bool IsPressed_SHOULDER_LEFT();
bool IsPressed_SHOULDER_RIGHT();

bool IsPressed_TRIGGER_LEFT();
bool IsPressed_TRIGGER_RIGHT();

int IsPressed_THUMB_LEFT_X();
int IsPressed_THUMB_LEFT_Y();
int IsPressed_THUMB_RIGHT_X();
int IsPressed_THUMB_RIGHT_Y();

bool CXBoxController::IsPressed_A()
{
	if(IsDown_A() && !(m_PreviousButton & XINPUT_GAMEPAD_A))
	{
		m_PreviousButton ^= XINPUT_GAMEPAD_A;
		m_fLastPress = (float)(GetTickCount() + XBOX_TIMEBETWEEN);
		return true;
	}

	return false;
}

bool CXBoxController::IsPressed_B()
{
	if(IsDown_B() && m_PreviousButton != XINPUT_GAMEPAD_B)
	{
		m_PreviousButton = XINPUT_GAMEPAD_B;
		m_fLastPress = (float)(GetTickCount() + XBOX_TIMEBETWEEN);
		return true;
	}

	return false;
}

bool IsPressed_X();

bool CXBoxController::IsPressed_Y()
{
	if(IsDown_Y() && m_PreviousButton != XINPUT_GAMEPAD_Y)
	{
		m_PreviousButton = XINPUT_GAMEPAD_Y;
		m_fLastPress = (float)(GetTickCount() + XBOX_TIMEBETWEEN);
		return true;
	}

	return false;

}

bool IsPressed(WORD _button);

bool CXBoxController::IsReleased_A()
{
	if(!IsDown_A() && (m_PreviousButton & XINPUT_GAMEPAD_A))
	{
		m_PreviousButton ^= XINPUT_GAMEPAD_A;
		return true;
	}

	return false;
}