//////////////////////////////////////////////////////
// File: "CExampleState.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _CExampleState_H_
#define _CExampleState_H_
#include "Game/States/IGameState.h"
#include "Game/Enemy AI/CEnemyAI.h"

class CObjectFactory;

class CExampleState : public IGameState
{
	CObjectFactory* m_ObjectFactory;
	CEnemyAI m_EnemyAI;
public:
	CExampleState(); // Default Constructor
	CExampleState(const CExampleState& Source); // Copy Constructor - Disabled
	CExampleState& operator=(const CExampleState& Source); // Assignment Operator - Disabled
	~CExampleState(); // Destructor

	void OnEnter();
	void OnExit();

	bool Input();
	void Update(float Delta);
	void Draw();
};

class IObject;

IObject* ExampleButtonFactoryMethod(CObjectManager* ObjectManager);
IObject* ToastFactoryMethod(CObjectManager* ObjectManager);
#endif