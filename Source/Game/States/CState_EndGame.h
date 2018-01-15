//////////////////////////////////////////////////////
// File: "CState_EndGame.h"
// Creator: MG
// Creation Date: 10/05/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////

#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"
#include "../Objects/Base/IAnimable.h"

class CButton;

class CState_EndGame : public IGameState, public IAnimable
{
	class CExitButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} ExitButtonOnClick;

	CButton* m_ExitButton;

	char m_Text[24];
	bool m_bWinner;
	float m_EndTimer;
public:
	CState_EndGame(bool _win); // Default Constructor
	CState_EndGame(const CState_EndGame& ref); // Copy Constructor - Disabled
	CState_EndGame& operator=(const CState_EndGame& ref); // Assignment Operator - Disabled
	~CState_EndGame(); // Destructor

	void OnEnter();
	void OnExit();

	bool Input(){return true;}
	void Update(float _delta);
	void Draw();
};