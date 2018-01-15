//////////////////////////////////////////////////////
// File: "CState_Gameplay.h"
// Creator: MG
// Creation Date: 9/28/09
// Last Modified: 9/28/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#ifndef _CExampleState_H_
#define _CExampleState_H_
#include "IGameState.h"
#include "../Enemy AI/CEnemyArtificialIntelligence.h"
#include "../../Managers/Bad Chef/Events/CEvent.h"
#include "../../CGame.h"

#include <string>

class CObjectFactory;
class CState_Gameplay;

class CFocusLostFunc : public CEventFunctor<CGame>
{
	CState_Gameplay* m_pGameplay;

public:
	void operator()();
	void SetGameplay(CState_Gameplay* pGameplay) { m_pGameplay = pGameplay; }
};

class CState_Gameplay : public IGameState
{
	CObjectFactory* m_ObjectFactory;
	CEnemyArtificialIntelligence m_EnemyAI;
	bool m_bNetworked;
	float m_NetworkUpdateTimer;
	CFocusLostFunc m_FocusLost;

	bool m_bPaused;

public:
	CState_Gameplay(int _playernum = 1, bool _networked = false, std::string mapFile = "Resources/Data/Airborne.FFM"); // Default Constructor
	CState_Gameplay(const CState_Gameplay& Source); // Copy Constructor - Disabled
	CState_Gameplay& operator=(const CState_Gameplay& Source); // Assignment Operator - Disabled
	~CState_Gameplay(); // Destructor

	void OnEnter();
	void OnExit();

	bool Input();
	void Update(float Delta);
	void UpdateUnder(float Delta);
	void Draw();

	void DrawUnder() { Draw(); m_bPaused = true; }

	void SetPaused(bool bPaused) { m_bPaused = bPaused; }
};
#endif