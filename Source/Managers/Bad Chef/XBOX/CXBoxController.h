#ifndef JS_CXBoxController_H_
#define JS_CXBoxController_H_

#include <windows.h>
#include <XInput.h>

#pragma comment(lib, "XInput.lib")

#define XBOX_VIBRATE_MAX 65535
#define XBOX_VIBRATE_MIN 0
#define XBOX_TRIGGER_MAX 255
#define XBOX_TRIGGER_MIN 0
#define XBOX_THUMB_MAX 32767
#define XBOX_THUMB_MIN -32768

#define XBOX_TIMEBETWEEN 300

class CXBoxController
{
private:
	XINPUT_STATE m_ControllerState;
	unsigned int m_nControllerNumber;

	char m_cTriggerThreshold;
	int  m_nThumbDeadZone_Right;
	int  m_nThumbDeadZone_Left;

	WORD	m_PreviousButton;
	float	m_fLastPress;

	float m_fDelta;

public:
	CXBoxController(unsigned int _playerNumber);

	XINPUT_STATE GetState();
	bool IsConnected();
	void Vibrate(int _left = 0, int _right = 0, int _seconds = 0);

	void Update();

	bool IsDown_DPAD_UP();
	bool IsDown_DPAD_DOWN();
	bool IsDown_DPAD_LEFT();
	bool IsDown_DPAD_RIGHT();
	bool IsDown_START();
	bool IsDown_BACK();
	bool IsDown_THUMB_LEFT_Press();
	bool IsDown_THUMB_RIGHT_Press();
	bool IsDown_SHOULDER_LEFT();
	bool IsDown_SHOULDER_RIGHT();

	bool IsDown_TRIGGER_LEFT();
	bool IsDown_TRIGGER_RIGHT();

	int IsDown_THUMB_LEFT_X();
	int IsDown_THUMB_LEFT_Y();
	int IsDown_THUMB_RIGHT_X();
	int IsDown_THUMB_RIGHT_Y();

	bool IsDown_A();
	bool IsDown_B();
	bool IsDown_X();
	bool IsDown_Y();

	bool IsDown(WORD _button);

	///////////////////////////////////

	bool IsPressed_DPAD_UP();
	bool IsPressed_DPAD_DOWN();
	bool IsPressed_DPAD_LEFT();
	bool IsPressed_DPAD_RIGHT();
	bool IsPressed_START();
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

	bool IsPressed_A();
	bool IsPressed_B();
	bool IsPressed_X();
	bool IsPressed_Y();

	bool IsPressed(WORD _button);

	///////////////////////////////////////

	bool IsReleased_A();

	///////////////////////////////////////

	int GetThumbLeft_X();
	int GetThumbLeft_Y();
	int GetThumbRight_X();
	int GetThumbRight_Y();

	int GetTriggerLeft();
	int GetTriggerRight();

	unsigned int GetControllerNumber()			{ return m_nControllerNumber; }

	char GetTriggerThreshold()					{ return m_cTriggerThreshold; }
	void SetTriggerThreshold(char _threshold)	{ m_cTriggerThreshold = _threshold; }

	int  GetThumbDeadZone_Right()				{ return m_nThumbDeadZone_Right; }
	void SetThumbDeadZone_Right(int _dead)		{ m_nThumbDeadZone_Right = _dead; }
	
	int  GetThumbDeadZone_Left()				{ return m_nThumbDeadZone_Left; }
	void SetThumbDeadZone_Left(int _dead)		{ m_nThumbDeadZone_Left = _dead; }
};

#endif