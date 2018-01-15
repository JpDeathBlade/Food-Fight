#ifndef FF_JS_CSTATE_CREDITS_H_
#define FF_JS_CSTATE_CREDITS_H_

#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"
#include "../Objects/Base/IAnimable.h"

class CButton;

class CState_Credits : public IGameState, public IAnimable
{
	class CBackButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} BackButtonOnClick;

	CButton* m_BackButton;
public:
	CState_Credits(); // Default Constructor
	CState_Credits(const CState_Credits& ref); // Copy Constructor - Disabled
	CState_Credits& operator=(const CState_Credits& ref); // Assignment Operator - Disabled
	~CState_Credits(); // Destructor

	bool Input();
	void Update(float _delta);
	void Draw();
};
#endif