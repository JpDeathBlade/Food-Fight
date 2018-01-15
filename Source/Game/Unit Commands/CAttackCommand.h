//////////////////////////////////////////////////////
// File: "CAttackCommand.h"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 9/21/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CATTACKCOMMAND_H_
#define CATTACKCOMMAND_H_

#include "IUnitAICommand.h"

class IAttackable;
class CUnitObject;
class CMoveCommand;

class CAttackCommand : public IUnitAICommand
{
private:
	IAttackable*	m_pTarget;
	CMoveCommand*	m_pMoveCommand;

public:
	/////////////////Constructors and destructors//////////////////
	CAttackCommand(void);
	CAttackCommand(IAttackable* pTarget);
	~CAttackCommand(void);

	///////////////////////////////////////////////////////////////
	// Function: Execute
	//
	// In: An unit object pointer to execute this command's function upon.
	//
	// Out: bool that lets you know when the command has finished executing.
	//
	// Purpose: Tells the unit what to do until certain conditions are met.
	///////////////////////////////////////////////////////////////
	bool Execute(CUnitObject* pObject);

	void SetTarget(IAttackable* const Target);
	IAttackable* GetTarget() const;
};
#endif