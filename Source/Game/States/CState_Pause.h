#ifndef FF_JS_CSTATE_PAUSE_H_
#define FF_JS_CSTATE_PAUSE_H_

#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"
#include "../Objects/Base/IAnimable.h"

class CButton;

class CState_Pause : public IGameState, public IAnimable
{
	enum EMenu { MIN, OPTIONS = MIN, HOWTOPLAY, RESUME, EXIT, MAX = EXIT};

	class COptionsButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} OptionsButtonOnClick;

	class CHowToPlayButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} HowToPlayButtonOnClick;

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-021
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Removed Profiles Button Functor

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-021
	//////////////////////////////////////////////////////////////////////////

	class CResumeButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} ResumeButtonOnClick;

	class CExitButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} ExitButtonOnClick;

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-021
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Removed Profiles Button Index
	CButton* m_Buttons[4];

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-021
	//////////////////////////////////////////////////////////////////////////

	EMenu m_nSelection;

	EMenu GetSelection() const;
	void SetSelection(EMenu Selection);
public:
	CState_Pause();
	CState_Pause(const CState_Pause& Source); // Copy Constructor - Disabled
	CState_Pause& operator=(const CState_Pause& Source); // Assignment Operator - Disabled
	~CState_Pause(); // Destructor

	void OnEnter();

	bool Input();
	void Draw();
};

#endif