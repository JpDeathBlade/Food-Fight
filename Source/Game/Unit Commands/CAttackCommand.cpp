//////////////////////////////////////////////////////
// File: "CAttackCommand.cpp"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 9/21/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CAttackCommand.h"
#include "../Objects/Base/IAttackable.h"
#include "../Objects/Appliances/CStove.h"
#include "../Objects/Units/CUnitObject.h"
#include "../Objects/Units/CSkilledUnitObject.h"
#include "CMoveCommand.h"

CAttackCommand::CAttackCommand(void)
{
	m_pMoveCommand	= NULL;
	m_pTarget		= NULL;
}

CAttackCommand::CAttackCommand(IAttackable* pTarget)
{
	if(pTarget)
	{
		m_pTarget = pTarget;
		CGameObject* pObject = dynamic_cast<CGameObject*>(pTarget);

		if(pObject)
			pObject->AcquireReference();
	}

	m_pMoveCommand = NULL;
}

CAttackCommand::~CAttackCommand(void)
{
	if(m_pMoveCommand)
	{
		m_pMoveCommand->ReleaseReference();
		m_pMoveCommand = NULL;
	}

	if(m_pTarget)
	{
		CGameObject* pObject = dynamic_cast<CGameObject*>(m_pTarget);

		if(pObject)
			pObject->ReleaseReference();
	}
}

bool CAttackCommand::Execute(CUnitObject* pObject)
{
	CGameObject* pEnemy = dynamic_cast<CGameObject*>(m_pTarget);

	if(pEnemy)
	{
		if(!pEnemy->CheckRemove())
		{
			CUnitObject* pUnit = dynamic_cast<CUnitObject*>(pObject);
			if(pUnit)
			{
				if(!pUnit->CheckIsDead())
				{
					D3DXVECTOR2 vDistance;
					D3DXVec2Subtract(&vDistance, &pEnemy->GetPosition(), &pObject->GetPosition()); 
					float fDistance = D3DXVec2Length(&vDistance);

					if(fDistance < pObject->GetRange())
					{
						if(m_pMoveCommand)
						{
							m_pMoveCommand->ReleaseReference();
							m_pMoveCommand = NULL;
							pObject->SetVelocityX(0.0f);
							pObject->SetVelocityY(0.0f);
						}

						CSkilledUnitObject* pSkilledUnit = dynamic_cast<CSkilledUnitObject*>(pUnit);
						if(pSkilledUnit)
						{
							CSkilledUnitObject* pUnitEnemy = dynamic_cast<CSkilledUnitObject*>(m_pTarget);
							if(pUnitEnemy)
								pSkilledUnit->Attack(pUnitEnemy);
						}
						else
							pUnit->Attack(m_pTarget);
					}
					else 
					{
						if(!m_pMoveCommand)
						{
							m_pMoveCommand = new CMoveCommand();
							m_pMoveCommand->AcquireReference();
							m_pMoveCommand->SetDestination(pEnemy->GetPosition());
						}
						else if(m_pMoveCommand->Execute(pObject))
							m_pMoveCommand->SetDestination(pEnemy->GetPosition());
					}
				}
				else
					return true;
			}

			CStove* pStove = NULL;
			pStove = dynamic_cast<CStove*>(m_pTarget);
			if(pStove)
			{
				D3DXVECTOR2 vHalfSize = D3DXVECTOR2((float)((pStove->GetCollisionRectangle().right - pStove->GetCollisionRectangle().left)/2), (float)((pStove->GetCollisionRectangle().bottom - pStove->GetCollisionRectangle().top)/2));

				D3DXVECTOR2 vRelativePos = (pStove->GetPosition() + vHalfSize);

				D3DXVECTOR2 vPointToCheck = D3DXVECTOR2(0.0f, 0.0f);

				if(pObject->GetPositionX() > (vRelativePos.x + vHalfSize.x))
					vPointToCheck.x = (vRelativePos.x + vHalfSize.x);
				else if(pObject->GetPositionX() < (vRelativePos.x - vHalfSize.x))
					vPointToCheck.x = (vRelativePos.x - vHalfSize.x);
				else
					vPointToCheck.x = pObject->GetPositionX();

				if(pObject->GetPositionY() > (vRelativePos.y + vHalfSize.y))
					vPointToCheck.y = (vRelativePos.y + vHalfSize.y);
				else if(pObject->GetPositionY() < (vRelativePos.y - vHalfSize.y))
					vPointToCheck.y = (vRelativePos.y - vHalfSize.y);
				else
					vPointToCheck.y = pObject->GetPositionY();

				D3DXVECTOR2 vDistance;
				D3DXVec2Subtract(&vDistance, &vPointToCheck, &pObject->GetPosition()); 
				float fDistance = D3DXVec2Length(&vDistance);

				if(fDistance <= pObject->GetRange())
				{
					if(m_pMoveCommand)
					{
						m_pMoveCommand->ReleaseReference();
						m_pMoveCommand = NULL;
						pObject->SetVelocityX(0.0f);
						pObject->SetVelocityY(0.0f);
					}

					pObject->Attack(pStove);
				}
				else 
				{
					if(!m_pMoveCommand)
					{
						m_pMoveCommand = new CMoveCommand();
						m_pMoveCommand->AcquireReference();
						m_pMoveCommand->SetDestination(pEnemy->GetPosition());
					}
					else if(m_pMoveCommand->Execute(pObject))
						m_pMoveCommand->SetDestination(pEnemy->GetPosition());
				}
			}
			
		}
		else
			return true;
	}
	else
		return true;

	return false;
}

void CAttackCommand::SetTarget(IAttackable* const Target)
{
	m_pTarget = Target;

	//SEND ATTACK COMMAND PACKET
}

IAttackable* CAttackCommand::GetTarget() const
{
	return m_pTarget;
}