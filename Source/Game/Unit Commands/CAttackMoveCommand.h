//////////////////////////////////////////////////////
// File: "CAttackMoveCommand.h"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 09/22/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CATTACKMOVECOMMAND_H_
#define CATTACKMOVECOMMAND_H_

#include "IUnitAICommand.h"
#include <d3dx9.h>

class CAttackCommand;
class CMoveCommand;
class CObjectManager;

class CAttackMoveCommand : public IUnitAICommand
{
private:
	CMoveCommand*		m_pMoveCommand;
	CAttackCommand*		m_pAttackCommand;
	CObjectManager*		m_pManager;

public:
	//////////////Constructors and Destructor//////////////
	CAttackMoveCommand(void);
	CAttackMoveCommand(D3DXVECTOR2 vDestination, CObjectManager* pManager = NULL);
	~CAttackMoveCommand(void);

	///////////////////////////////////////////////////////////////
	// Function: Execute
	//
	// In: An IObject pointer of the object to act upon.
	//
	// Out: bool for whether or not the command has finished.
	//
	// Purpose: To move a unit to a point while attacking other 
	//			units on the way.
	///////////////////////////////////////////////////////////////
	bool Execute(CUnitObject* Object);

	////////////////Accessors and mutators////////////////
	CMoveCommand* GetMoveCommand(void)					{ return m_pMoveCommand; }
	void SetMoveCommand(CMoveCommand* pMoveCommand);

	CAttackCommand* GetAttackCommand(void)				{ return m_pAttackCommand; }
	void SetAttackCommand(CAttackCommand* pAttackCommand);

	CObjectManager* GetManager(void)					{ return m_pManager; }
	void SetManager(CObjectManager* pManager)			{ m_pManager = pManager; }

	D3DXVECTOR2 GetDestination() const;
	float GetDestinationX() const;
	float GetDestinationY() const;

	void SetDestination(D3DXVECTOR2 Destination);
	void SetDestination(float X, float Y);
	void SetDestinationX(float X);
	void SetDestinationY(float Y);
};

#endif