#ifndef FF_JS_CSTATE_PROFILE_H_
#define FF_JS_CSTATE_PROFILE_H_

#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"
#include "../Objects/Base/IAnimable.h"

class CButton;

class CState_Profile : public IGameState, public IAnimable
{
	enum EMenu { MIN, PROFILEA = MIN, PROFILEB, PROFILEC, DEFAULT, BACK, MAX = BACK};

	class CProfileButtonOnClickFunctor : public IFunctor
	{
		unsigned char m_ProfileNumber;
	public:
		void operator()();

		unsigned char GetProfileNumber() const;
		void SetProfileNumber(unsigned char ProfileNumber);
	};

	CProfileButtonOnClickFunctor ProfileAButtonOnClick;
	CProfileButtonOnClickFunctor ProfileBButtonOnClick;
	CProfileButtonOnClickFunctor ProfileCButtonOnClick;

	class CBackButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} BackButtonOnClick;

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BUG-016
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	class CSetToDefaultOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} DefaultButtonOnClick;

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BUG-016
	//////////////////////////////////////////////////////////////////////////


	CButton* m_Buttons[5];

	EMenu m_nSelection;
	
	EMenu GetSelection() const;
	void SetSelection(EMenu Selection);
public:
	CState_Profile(); // Default Constructor
	CState_Profile(const CState_Profile& ref); // Copy Constructor - Disabled
	CState_Profile& operator=(const CState_Profile& ref); // Assignment Operator - Disabled
	~CState_Profile(); // Destructor

	bool Input();
	void Update(float _delta);
	void Draw();
};

#endif