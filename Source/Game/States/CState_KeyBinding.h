#ifndef FF_JS_CSTATE_KEYBINDING_H_
#define FF_JS_CSTATE_KEYBINDING_H_

#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"
#include "../Objects/Base/IAnimable.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"

class CButton;

class CState_KeyBinding : public IGameState, public IAnimable
{
	enum EMenu { MIN, UP = MIN, DOWN, LEFT, RIGHT, MOVE, ATTACK, STOP, STOVE, RALLY_POINT, COOKBOOK, BACK, MAX = BACK};

	class CKeyBindingOnClickFunctor : public IFunctor
	{
		CState_KeyBinding* m_State;
		unsigned char m_KeyPressed;
		eKeyBindings m_Key;
	public:
		void operator()();

		void SetState(CState_KeyBinding* State);
		void SetKeyPressed(unsigned char KeyPressed);
		void SetKeyBinding(eKeyBindings Key);
	};

	CKeyBindingOnClickFunctor KeyBindingOnClick[10];

	class CBackButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} BackButtonOnClick;

	CButton* m_Buttons[11];
	EMenu m_nSelection;
	
	bool m_bIsChangingBind;
public:
	CState_KeyBinding(); // Default Constructor
	CState_KeyBinding(const CState_KeyBinding& ref); // Copy Constructor - Disabled
	CState_KeyBinding& operator=(const CState_KeyBinding& ref); // Assignment Operator - Disabled
	~CState_KeyBinding(); // Destructor

	bool Input();
	void Update(float _delta);
	void Draw();

	bool IsBinding();
	void ToggleBinding();

	EMenu GetSelection() const;
	void SetSelection(EMenu Selection);
};
#endif