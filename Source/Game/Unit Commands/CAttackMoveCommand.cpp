//////////////////////////////////////////////////////
// File: "CAttackMoveCommand.cpp"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 9/22/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CAttackMoveCommand.h"
#include "CMoveCommand.h"
#include "CAttackCommand.h"
#include "../../Managers/Bad Chef/CObjectManager.h"
#include "../Objects/Units/CUnitObject.h"

CAttackMoveCommand::CAttackMoveCommand(void)
{
	m_pMoveCommand		= new CMoveCommand();
	m_pAttackCommand	= NULL;
}

CAttackMoveCommand::CAttackMoveCommand(D3DXVECTOR2 vDestination, CObjectManager* pManager)
{
	m_pMoveCommand = new CMoveCommand();
	m_pMoveCommand->SetDestination(vDestination);
	m_pMoveCommand->AcquireReference();

	m_pManager = pManager;

	m_pAttackCommand = NULL;
}

CAttackMoveCommand::~CAttackMoveCommand(void)
{
	if(m_pMoveCommand)
	{
		m_pMoveCommand->ReleaseReference();
		m_pMoveCommand = NULL;
	}

	if(m_pAttackCommand)
	{
		m_pAttackCommand->ReleaseReference();
		m_pAttackCommand = NULL;
	}
}

void CAttackMoveCommand::SetMoveCommand(CMoveCommand* pMoveCommand)
{
	if(m_pMoveCommand)
	{
		m_pMoveCommand->ReleaseReference();
		m_pMoveCommand = NULL;
	}

	m_pMoveCommand = pMoveCommand;
	m_pMoveCommand->AcquireReference();
}

void CAttackMoveCommand::SetAttackCommand(CAttackCommand* pAttackCommand)
{
	
	if(m_pAttackCommand)
	{
		m_pAttackCommand->ReleaseReference();
		m_pAttackCommand = NULL;
	}

	m_pAttackCommand = pAttackCommand;
	m_pAttackCommand->AcquireReference();
}

bool CAttackMoveCommand::Execute(CUnitObject* Object)
{
	if(m_pManager)
	{
		m_pManager->IterateLayer(1, &CUnitObject::CheckPerceptionRange, Object);
	}

	if(m_pAttackCommand)
	{
		if(m_pAttackCommand->Execute(Object))
		{
			m_pAttackCommand->ReleaseReference();
			m_pAttackCommand = NULL;
		}
	}
	else
	{
		if(m_pMoveCommand->Execute(Object))
			return true;
	}
	return false;
}

D3DXVECTOR2 CAttackMoveCommand::GetDestination() const
{
	return m_pMoveCommand->GetDestination();
}

float CAttackMoveCommand::GetDestinationX() const
{
	return m_pMoveCommand->GetDestinationX();
}

float CAttackMoveCommand::GetDestinationY() const
{
	return m_pMoveCommand->GetDestinationY();
}

void CAttackMoveCommand::SetDestination(D3DXVECTOR2 Destination)
{
	m_pMoveCommand->SetDestination(Destination);
}

void CAttackMoveCommand::SetDestination(float X, float Y)
{
	SetDestinationX(X);
	SetDestinationY(Y);
}

void CAttackMoveCommand::SetDestinationX(float X)
{
	m_pMoveCommand->SetDestinationX(X);
}

void CAttackMoveCommand::SetDestinationY(float Y)
{
	m_pMoveCommand->SetDestinationY(Y);
}
