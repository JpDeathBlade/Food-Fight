//////////////////////////////////////////////////////
// File: "IGameState.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _IGameState_H_
#define _IGameState_H_

#include "../../Managers/Bad Chef/CObjectManager.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"

#define STRING CStringTable::GetInstance()
#define SPRITEMANAGER CSpriteManager::GetInstance()

class IGameState
{
protected:
	CObjectManager m_ObjectManager;

public:
	IGameState(void)						{ } // Default Constructor
	IGameState(const IGameState& Source);			// Copy Constructor - Disabled
	IGameState& operator=(const IGameState& Source); // Assignment Operator - Disabled
	virtual ~IGameState(void) = 0			{ } // Destructor

	virtual void OnPush(void)				{ } // Executes when the state is pushed.
	virtual void OnEnter(void)				{ } // Executes when the state is entered.
	virtual void OnExit(void)				{ } // Executes when the state is exited.
	virtual void OnPop(void)				{ delete this; } // Executes when the state is popped.

	virtual bool Input(void)				{ return true; } // Processes input.
	virtual void Update(float _delta)		{ } // Updates the state.
	virtual void UpdateUnder(float _delta)	{ Update(_delta); }
	virtual void Draw(void)					{ } // Draws the state.
	virtual void DrawUnder(void)			{ Draw(); }

	CObjectManager* GetObjectManager() { return &m_ObjectManager; }
};
#endif