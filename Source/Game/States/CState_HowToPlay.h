#ifndef FF_JS_CSTATE_HOWTOPLAY_H_
#define FF_JS_CSTATE_HOWTOPLAY_H_

#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"
#include "../Objects/Base/IAnimable.h"

class CButton;

class CState_HowToPlay : public IGameState, public IAnimable
{
	class CBackButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} BackButtonOnClick;

	CButton* m_BackButton;
public:
	CState_HowToPlay(); // Default Constructor
	CState_HowToPlay(const CState_HowToPlay& ref); // Copy Constructor - Disabled
	CState_HowToPlay& operator=(const CState_HowToPlay& ref); // Assignment Operator - Disabled
	~CState_HowToPlay(); // Destructor

	bool Input();
	void Update(float _delta);
	void Draw();
};

#endif