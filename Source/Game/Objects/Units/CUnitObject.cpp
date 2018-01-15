//////////////////////////////////////////////////////
// File: "CUnitObject.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/22/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CUnitObject.h"
#include "../GUI Objects/CCursor.h"
#include "../../Unit Commands/IUnitAICommand.h"
#include "../../Unit Commands/CMoveCommand.h"
#include "../../Unit Commands/CAttackCommand.h"
#include "../../Unit Commands/CAttackMoveCommand.h"
#include "../GUI Objects/CSelectionHUD.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include "../../../Managers/Bad Chef/Particles/CParticleManager.h"
#include "../../../Managers/Bad Chef/Audio/CAudioManager.h"
#include "../Appliances/CStove.h"
#include "CStatusEffect.h"
#include "CProjectile.h"
#include "../Base/CCamera.h"
#include "../GUI Objects/CFloatingText.h"

bool BeginBBCombo(CUnitObject* _unit)
{
	_unit->AddAttackMultiply(1);
	_unit->AddDefenseMultiply(1);
	_unit->AddEvadeMultiply(1);

	return true;
}

bool EndBBCombo(CUnitObject* _unit)
{
	_unit->AddAttackMultiply(-1);
	_unit->AddDefenseMultiply(-1);
	_unit->AddEvadeMultiply(-1);

	return true;
}

CUnitObject::CUnitObject(void)
{
	m_MovementSpeed		= 0;
	m_UnitAICommand		= NULL;
	m_usDefense			= 0;
	m_usAttack			= 0;
	m_fAttackSpeed		= 0.0f;
	m_fEvade			= 0.0f;
	m_fRange			= 0.0f;
	m_fPerceptionRange	= 200.0f;
	m_fPerceptionTimer	= 1.0f;
	m_fAccumulator		= 1.0f;
	m_pNextNode			= 0;
	m_bBalanceBonus		= false;
	m_ForceType			= NO_FORCE;
	m_VelocityForce.x   = 0;
	m_VelocityForce.y	= 0;
	m_szBullet			= "";
	m_bIsDead			= false;
	m_fDeadTimer		= 0.0f;

	SetAttackTimer(0.0f);
	m_fBuffTimer		= 0.0f;

	m_AttackSound = NULL;

	//Modifications to stats
	//Multiplying
	m_fAttackMultiply = 1.0;
	m_fDefenseMultiply = 1.0;
	m_fMovementMultiply = 1.0;
	m_fEvadeMultiply = 1.0;
	//Adding
	m_fAttackAdd = 0;
	m_fDefenseAdd = 0;
	m_fMovementAdd = 0;
	m_fEvadeAdd = 0;

	for(int i = 0; i < SKILL_AMOUNT; i++)
		m_Skills[i] = NULL;

	m_pArmRight = new CGameObject();
	m_pArmLeft  = new CGameObject();
	m_pLegs		= new CGameObject();
	m_pFace		= new CGameObject();

	m_pArmRight->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Arms.spr"));
	m_pArmRight->SetPosition(GetPosition());
	m_pArmRight->SetAnimationType(GetAnimationType());
	m_pArmRight->SetDirection(GetDirection());
	m_pArmRight->SetState(GetState());

	m_pArmLeft->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Arms.spr"));
	m_pArmLeft->SetPosition(GetPosition());
	m_pArmLeft->SetAnimationType(GetAnimationType());
	m_pArmLeft->SetDirection(GetDirection());
	m_pArmLeft->SetState(GetState());

	// TODO: Replace this and move it to the factory methods, so each unit can have different sounds.
	m_AttackSound = CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/Punch.wav", "Effects");
	m_pVoice = NULL;

	m_pLegs->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Legs.spr"));

	m_pLegs->SetPosition(GetPosition());
	m_pLegs->SetAnimationType(GetAnimationType());
	m_pLegs->SetDirection(GetDirection());
	m_pLegs->SetState(GetState());

	m_pFace->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Face.spr"));
	m_pFace->SetPosition(GetPosition());
	m_pFace->SetAnimationType(GetAnimationType());
	m_pFace->SetDirection(GetDirection());
	m_pFace->SetState(GetState());

	m_pCombo = CParticleManager::GetInstance()->GetParticleEffect("Combo");
	m_pCombo->InitParticles();
}

CUnitObject::CUnitObject(const CUnitObject& Source) : CGameObject(Source)
{
	SetMovementSpeed(GetMovementSpeed());
	SetUnitAICommand(Source.GetUnitAICommand());

	for(int i = 0; i < SKILL_AMOUNT; i++)
		m_Skills[i] = Source.m_Skills[i];

	m_pArmRight = Source.m_pArmRight;
	m_pArmLeft  = Source.m_pArmLeft;
	m_pLegs		= Source.m_pLegs;
	m_pFace		= Source.m_pFace;

	m_pVoice = Source.m_pVoice;

	m_pCombo = CParticleManager::GetInstance()->GetParticleEffect("Combo");
	m_pCombo->InitParticles();
}

CUnitObject& CUnitObject::operator=(const CUnitObject& Source)
{
	CGameObject::operator=(Source);
	ISelectable::operator=(Source);
	SetMovementSpeed(GetMovementSpeed());
	SetUnitAICommand(Source.GetUnitAICommand());

	for(int i = 0; i < SKILL_AMOUNT; i++)
		m_Skills[i] = Source.m_Skills[i];

	m_pArmRight = Source.m_pArmRight;
	m_pArmLeft  = Source.m_pArmLeft;
	m_pLegs		= Source.m_pLegs;
	m_pFace		= Source.m_pFace;
	m_pVoice = Source.m_pVoice;

	return *this;
}

CUnitObject::~CUnitObject(void)
{
	for(std::list<CStatusEffect*>::iterator i = m_ActiveStatusEffects.begin(); i != m_ActiveStatusEffects.end(); ++i)
		(*i)->ReleaseReference();

	m_ActiveStatusEffects.clear();

	for(int i = 0; i < SKILL_AMOUNT; i++)
		delete m_Skills[i];

	delete m_pArmRight;
	delete m_pArmLeft;
	delete m_pLegs;
	delete m_pFace;

	if(m_UnitAICommand)
		m_UnitAICommand->ReleaseReference();

	delete m_pCombo;
	//m_pCombo->DeleteEmitter();
	//m_pCombo = NULL;
}

void CUnitObject::SetFace(CGameObject* _face)
{
	if(m_pFace)
		delete m_pFace;

	m_pFace = _face;
}

void CUnitObject::SetLegs(CGameObject* _legs)
{
	if(m_pLegs)
		delete m_pLegs;

	m_pLegs = _legs;
}

void CUnitObject::SetArmRight(CGameObject* _arm)
{
	if(m_pArmRight)
		delete m_pArmRight;

	m_pArmRight = _arm;
}

void CUnitObject::SetArmLeft(CGameObject* _arm)
{
	if(m_pArmLeft)
		delete m_pArmLeft;

	m_pArmLeft = _arm;
}

float CUnitObject::GetZDepth() const
{
	return CGameObject::GetZDepth();
}

void CUnitObject::Update(float Delta)
{
	

	if(GetHealth() <= 0)
	{
		m_bIsDead = true;
		SetState(STATE_DEAD);
		SetCollideable(false);
	}

	if(m_bIsDead)
	{
		m_fDeadTimer += Delta;

		if(m_fDeadTimer >= 1.0f)
			DoRemove();

		SetVelocity(0,0);

		IAnimable::UpdateAnimation(Delta);

		m_pArmRight->UpdateAnimation(Delta);
		m_pArmLeft->UpdateAnimation(Delta);
		m_pLegs->UpdateAnimation(Delta);
		m_pFace->UpdateAnimation(Delta);

	}
	else
	{
		m_pCombo->SetPosition(GetPosition() - D3DXVECTOR2(0.0f, -24.0f));
		m_pCombo->Update(Delta);

		m_AttackTimer += Delta;
		
		m_pArmRight->Update(Delta);
		m_pArmLeft->Update(Delta);
		m_pLegs->Update(Delta);
		m_pFace->Update(Delta);

		if(m_bMine)
		{
			SetVelocity(0,0);
			if(m_ForceType != NO_FORCE)
			{
				SetVelocity(m_VelocityForce);

				switch(m_ForceType)
				{
				case SPEED_FORCE:
					m_VelocityForce.x += m_VelocityForce.x * -Delta;
					m_VelocityForce.y += m_VelocityForce.y * -Delta;
					break;
				case KNOCKBACK_FORCE:
					m_VelocityForce.x += m_VelocityForce.x * -Delta * 2;
					m_VelocityForce.y += m_VelocityForce.y * -Delta * 2;
					break;
				}

				if(m_VelocityForce.x < 0.5 && m_VelocityForce.x > -0.5)
				{
					m_VelocityForce.x = 0;
				}

				if(m_VelocityForce.y < 0.5 && m_VelocityForce.y > -0.5)
				{
					m_VelocityForce.y = 0;
				}

				if(m_VelocityForce.x == 0 && m_VelocityForce.y == 0)
				{
					m_ForceType = NO_FORCE;
				}
			}

			//Process all status effects
			std::list<CStatusEffect*> eraseList;

			for(std::list<CStatusEffect*>::iterator Iterator = m_ActiveStatusEffects.begin(); Iterator != m_ActiveStatusEffects.end(); ++Iterator)
			{
				if(!(*Iterator)->Process(this))
				{
					eraseList.push_back((*Iterator));
					(*Iterator)->ReleaseReference();
				}
			}

			// Remove invalid status effects.
			for(std::list<CStatusEffect*>::iterator Iterator = eraseList.begin(); Iterator != eraseList.end(); ++Iterator)
				m_ActiveStatusEffects.remove((*Iterator));

			if(m_UnitAICommand)
				if(m_UnitAICommand->Execute(this))
				{
					SetUnitAICommand(NULL);
					SetVelocityX(0.0f);
					SetVelocityY(0.0f);
					SetState(STATE_IDLE);
				}

				CGameObject::Update(Delta);

				m_fAccumulator -= Delta;

				if(!GetUnitAICommand())
					SetState(STATE_IDLE);

				if(m_fAccumulator <= 0.0f)
				{
					if(GetState() == STATE_IDLE)
						GetObjectManagerOwner()->IterateLayer(1, &CUnitObject::CheckPerceptionRange, this);

					m_fAccumulator = m_fPerceptionTimer;
				}

				m_fBuffTimer += Delta;

				if(m_fBuffTimer >= 3.0f)
				{
					for(int i = 0; i < 4; ++i)
						m_bNearbyUnits[i] = false;

					m_bNearbyUnits[GetBaseType()] = true;

					GetObjectManagerOwner()->IterateLayer(1, &CUnitObject::CheckNearbyFriendlyUnits, this);

					bool bCombo = true;
					
					for(int i = 0; i < 4; ++i)
					{
						if(m_bNearbyUnits[i] == false)
						{
							bCombo = false;
							break;
						}
					}

					if(bCombo)
					{
						CStatusEffect* comboEffect = new CStatusEffect();
						comboEffect->Begin = BeginBBCombo;
						comboEffect->End = EndBBCombo;
						comboEffect->SetTimer(3);
						
						this->AddStatusEffect(comboEffect);
						m_pCombo->Play();
					}
					else
						m_pCombo->Stop();

					m_fBuffTimer = 0.0f;

				}

		}
		else
		{
			UpdateNetworked(Delta);
		}

		for(std::list<CParticleEmitter*>::iterator Iterator = m_OverParticleEmitters.begin(); Iterator != m_OverParticleEmitters.end(); ++Iterator)
		{
			(*Iterator)->Update(Delta);
			(*Iterator)->SetPosition(this->GetPosition());
		}

		for(std::list<CParticleEmitter*>::iterator Iterator = m_UnderParticleEmitters.begin(); Iterator != m_UnderParticleEmitters.end(); ++Iterator)
		{
			(*Iterator)->Update(Delta);
			(*Iterator)->SetPosition(this->GetPosition());
		}
	}

}

void CUnitObject::UpdateNetworked(float Delta)
{
	//Process all status effects
		std::list<CStatusEffect*> eraseList;

		for(std::list<CStatusEffect*>::iterator Iterator = m_ActiveStatusEffects.begin(); Iterator != m_ActiveStatusEffects.end(); ++Iterator)
		{
			if(!(*Iterator)->Process(this))
			{
				eraseList.push_back((*Iterator));
				(*Iterator)->ReleaseReference();
			}
		}

		// Remove invalid status effects.
		for(std::list<CStatusEffect*>::iterator Iterator = eraseList.begin(); Iterator != eraseList.end(); ++Iterator)
			m_ActiveStatusEffects.remove((*Iterator));


		SetVelocity(0, 0);

		if(GetHealth() <= 0)
			DoRemove();

		if(m_UnitAICommand)
			if(m_UnitAICommand->Execute(this))
			{
				SetUnitAICommand(NULL);
				SetVelocityX(0.0f);
				SetVelocityY(0.0f);
				SetState(STATE_IDLE);
			}

			CGameObject::Update(Delta);

			m_fAccumulator -= Delta;

			if(!GetUnitAICommand())
				SetState(STATE_IDLE);

			if(m_fAccumulator <= 0.0f)
			{
				if(GetState() == STATE_IDLE)
					GetObjectManagerOwner()->IterateLayer(1, &CUnitObject::CheckPerceptionRange, this);

				m_fAccumulator = m_fPerceptionTimer;
			}
}

void CUnitObject::SendUpdatePacket()
{
	NW_MANAGER_START
		if(CNetworkManager::GetInstance()->IsConnected())
			CNetworkManager::GetInstance()->SendUnitUpdatePacket(this);
	NW_MANAGER_END
}

void CUnitObject::Draw(void)
{
	if(GetCurrentFrame()->GetNumAnchorPoints() > 1)
	{
		if(GetCurrentFrame()->GetAnchorPoint("ArmRight") && m_pArmRight)
		{
			m_pArmRight->SetPositionX(GetCurrentFrame()->GetAnchorPoint("ArmRight")->GetAnchorPoint().x
				- GetCurrentFrame()->GetAnchorPoint("Base")->GetAnchorPoint().x + GetPositionX());
			m_pArmRight->SetPositionY(GetCurrentFrame()->GetAnchorPoint("ArmRight")->GetAnchorPoint().y
				- GetCurrentFrame()->GetAnchorPoint("Base")->GetAnchorPoint().y + GetPositionY());

			m_pArmRight->SetDirection(GetDirection());
			m_pArmRight->SetState(GetState());
		}

		if(GetCurrentFrame()->GetAnchorPoint("ArmLeft") && m_pArmLeft)
		{
			m_pArmLeft->SetPositionX(GetCurrentFrame()->GetAnchorPoint("ArmLeft")->GetAnchorPoint().x
				- GetCurrentFrame()->GetAnchorPoint("Base")->GetAnchorPoint().x + GetPositionX());
			m_pArmLeft->SetPositionY(GetCurrentFrame()->GetAnchorPoint("ArmLeft")->GetAnchorPoint().y
				- GetCurrentFrame()->GetAnchorPoint("Base")->GetAnchorPoint().y + GetPositionY());

			m_pArmLeft->SetDirection(GetDirection());
			m_pArmLeft->SetState(GetState());
		}

		if(GetCurrentFrame()->GetAnchorPoint("Legs") && m_pLegs)
		{
			m_pLegs->SetPositionX(GetCurrentFrame()->GetAnchorPoint("Legs")->GetAnchorPoint().x
				- GetCurrentFrame()->GetAnchorPoint("Base")->GetAnchorPoint().x + GetPositionX());
			m_pLegs->SetPositionY(GetCurrentFrame()->GetAnchorPoint("Legs")->GetAnchorPoint().y
				- GetCurrentFrame()->GetAnchorPoint("Base")->GetAnchorPoint().y + GetPositionY());

			m_pLegs->SetDirection(GetDirection());
			m_pLegs->SetState(GetState());
		}

		if(GetCurrentFrame()->GetAnchorPoint("Face") && m_pFace)
		{
			m_pFace->SetPositionX(GetCurrentFrame()->GetAnchorPoint("Face")->GetAnchorPoint().x
				- GetCurrentFrame()->GetAnchorPoint("Base")->GetAnchorPoint().x + GetPositionX());
			m_pFace->SetPositionY(GetCurrentFrame()->GetAnchorPoint("Face")->GetAnchorPoint().y
				- GetCurrentFrame()->GetAnchorPoint("Base")->GetAnchorPoint().y + GetPositionY());

			m_pFace->SetDirection(GetDirection());
			m_pFace->SetState(GetState());
		}
	}

	m_pCombo->Draw();
	for(std::list<CParticleEmitter*>::iterator Iterator = m_UnderParticleEmitters.begin(); Iterator != m_UnderParticleEmitters.end(); ++Iterator)
	{
		(*Iterator)->Draw();
	}

	//Draw the legs first, they will always be behind
	if(GetCurrentFrame()->GetAnchorPoint("Legs") && m_pLegs)
		m_pLegs->Draw();


	//If facing Right draw this stuff first (ARMS)
	if(GetCurrentFrame()->GetNumAnchorPoints() > 1)
	{
		if(GetDirection() == DIRECTION_LEFT)
		{
			if(GetCurrentFrame()->GetAnchorPoint("ArmRight") && m_pArmRight)
				m_pArmRight->Draw();
		}

		if(GetDirection() == DIRECTION_RIGHT)
		{
			if(GetCurrentFrame()->GetAnchorPoint("ArmLeft") && m_pArmLeft)
					m_pArmLeft->Draw();
		}
	}

	D3DCOLOR Color = D3DCOLOR_XRGB(255, 255, 255);
	if(IsSelected())
	{
		if(IsSubSelected())
		{
			CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Units/SubSelectionCircle.spr")->Draw(0, 0, GetPosition());
		}
		else
		{
			CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Units/SelectionCircle.spr")->Draw(0, 0, GetPosition());
		}
	}
	else
	{
		if(IsBeingHoveredOver())
		{
			switch(GetTeamValue())
			{
			case TEAM_PLAYER:
				Color = D3DCOLOR_XRGB(127, 255, 127); // Light Green
				break;
			case TEAM_NONE:
				Color = D3DCOLOR_XRGB(127, 127, 127); // Grey
				break;
			case TEAM_ENEMY:
				Color = D3DCOLOR_XRGB(255, 127, 127); // Light Red
				break;
			default:
				Color = D3DCOLOR_XRGB(255, 255, 255); // White
			}
		}
		else
			Color = D3DCOLOR_XRGB(255, 255, 255); // White
	}

	DrawAnimation(GetPosition(), GetRotation(), GetScale(), Color);

	// If facing Right draw this stuff last (ARMS)
	if(GetCurrentFrame()->GetNumAnchorPoints() > 1)
	{
		if(GetDirection() == DIRECTION_RIGHT)
		{
			if(GetCurrentFrame()->GetAnchorPoint("ArmRight") && m_pArmRight)
				m_pArmRight->Draw();
		}

		if(GetDirection() == DIRECTION_LEFT)
		{
			if(GetCurrentFrame()->GetAnchorPoint("ArmLeft") && m_pArmLeft)
				m_pArmLeft->Draw();
		}
	}

	//Draw Face
	if(GetCurrentFrame()->GetAnchorPoint("Face") && m_pFace)
		m_pFace->Draw();


	//Draw particles emitters that should be OVER everything
	for(std::list<CParticleEmitter*>::iterator Iterator = m_OverParticleEmitters.begin(); Iterator != m_OverParticleEmitters.end(); ++Iterator)
	{
		(*Iterator)->Draw();
	}

	// Draw Health Bar while hovering.

	if(IsBeingHoveredOver())
	{
		float HealthPercentage = static_cast<float>(GetHealth()) / GetMaxHealth();

		POINT Offset;
		Offset.x = (LONG)CAMERA->GetPositionX();
		Offset.y = (LONG)CAMERA->GetPositionY();

		D3DCOLOR Color = D3DCOLOR_XRGB(static_cast<unsigned char>((1 - HealthPercentage) * 255), static_cast<unsigned char>(HealthPercentage * 255), 0);
		CSGD_Direct3D::GetInstance()->DrawLine(GetPosition() + D3DXVECTOR2((float)(-25 - Offset.x), (float)(15 - Offset.y)), GetPosition() + D3DXVECTOR2((float)((50 * HealthPercentage) - 25 - Offset.x), (float)(15 - Offset.y)), 4, Color);
	}
}

//Adding a status effect to a unit
bool CUnitObject::AddStatusEffect(CStatusEffect* _effect)
{
	if(_effect->Begin)
	{
		if(!_effect->Begin(this))
		{
			return false;
		}
	}
	else if(_effect->BeginSender)
	{
		if(!_effect->BeginSender(this, _effect))
		{
			return false;
		}
	}
	_effect->AcquireReference();
	m_ActiveStatusEffects.push_back(_effect);
	return true;
}


unsigned short CUnitObject::GetMovementSpeed(void) const
{
	return (unsigned short)((m_MovementSpeed * m_fMovementMultiply) + m_fMovementAdd);
}

void CUnitObject::SetMovementSpeed(unsigned short MovementSpeed)
{
	m_MovementSpeed = MovementSpeed;
}

IUnitAICommand* CUnitObject::GetUnitAICommand(void) const
{
	return m_UnitAICommand;
}

void CUnitObject::SetUnitAICommand(IUnitAICommand* UnitAICommand)
{
	if(m_UnitAICommand)
		m_UnitAICommand->ReleaseReference();
	m_UnitAICommand = UnitAICommand;
	if(m_UnitAICommand)
		m_UnitAICommand->AcquireReference();

	if(UnitAICommand)
	{
		if(this == CSelectionHUD::GetInstance()->GetSubSelected())
			PlayVoice();
	}
}

void CUnitObject::ContextSensitiveClick(void)
{
	IAttackable* pAttackable = dynamic_cast<IAttackable*>(CCursor::GetInstance()->GetHoveredOverObject());

	if(pAttackable)
	{
		ISelectable* pSelectable = dynamic_cast<ISelectable*>(CCursor::GetInstance()->GetHoveredOverObject());
		if(pSelectable)
		{
			if(pSelectable->GetTeamValue() != GetTeamValue())
			{
				CAttackCommand* pAttackCommand = new CAttackCommand(pAttackable);
				SetUnitAICommand(pAttackCommand);
			}
		}
	}
	else
	{
		CMoveCommand* Command = new CMoveCommand;
		Command->SetDestination(CCursor::GetInstance()->GetPosition());
		SetUnitAICommand(Command);
	}
}

void CUnitObject::SetAnimationType(EAnimation _type)
{
	IAnimable::SetAnimationType(_type);

	if(m_pArmRight)
		m_pArmRight->SetAnimationType(_type);
	if(m_pArmLeft)
		m_pArmLeft->SetAnimationType(_type);
	if(m_pLegs)
		m_pLegs->SetAnimationType(_type);
	if(m_pFace)
		m_pFace->SetAnimationType(_type);
}

void CUnitObject::SetState(EUnitState _state)
{
	IAnimable::SetState(_state);

	if(m_pArmRight)
		m_pArmRight->SetState(_state);
	if(m_pArmLeft)
		m_pArmLeft->SetState(_state);
	if(m_pLegs)
		m_pLegs->SetState(_state);
	if(m_pFace)
		m_pFace->SetState(_state);
}

void CUnitObject::SetDirection(EUnitDirection _direction)
{
	IAnimable::SetDirection(_direction);

	if(m_pArmRight)
		m_pArmRight->SetDirection(_direction);
	if(m_pArmLeft)
		m_pArmLeft->SetDirection(_direction);
	if(m_pLegs)
		m_pLegs->SetDirection(_direction);
	if(m_pFace)
		m_pFace->SetDirection(_direction);
}

bool CUnitObject::CheckPerceptionRange(IObject* pObject, CUnitObject* pCaller)
{
	IAttackable* pAttackable = dynamic_cast<IAttackable*>(pObject);

	if(pAttackable)
	{
		ISelectable* pSelectable = dynamic_cast<ISelectable*>(pObject);

		if(pSelectable)
		{
			CUnitObject* pUnitObject = dynamic_cast<CUnitObject*>(pCaller);

			if(pSelectable->GetTeamValue() != pUnitObject->GetTeamValue())
			{
				CUnitObject* pGameObject;

				if((pGameObject = dynamic_cast<CUnitObject*>(pObject)) && !pGameObject->CheckIsDead())
				{

					D3DXVECTOR2 vDistance;
					D3DXVec2Subtract(&vDistance, &pGameObject->GetPosition(), &pUnitObject->GetPosition());
					float fDistance = D3DXVec2Length(&vDistance);

					if(fDistance <= pUnitObject->GetPerceptionRange())
					{
						if(pUnitObject->m_UnitAICommand)
						{
							CAttackMoveCommand* pMoveAttackCommand = dynamic_cast<CAttackMoveCommand*>(pUnitObject->m_UnitAICommand);

							if(pMoveAttackCommand)
							{
								if(!pMoveAttackCommand->GetAttackCommand())
								{
									CAttackCommand* pAttackCommand = new CAttackCommand(pAttackable);
									pMoveAttackCommand->SetAttackCommand(pAttackCommand);
								}
								else
								{
									CGameObject* pPreviousTarget = dynamic_cast<CGameObject*>(pMoveAttackCommand->GetAttackCommand()->GetTarget());

									if(pPreviousTarget)
									{
										D3DXVec2Subtract(&vDistance, &pPreviousTarget->GetPosition(), &pUnitObject->GetPosition());
										float fOldDistance = D3DXVec2Length(&vDistance);

										if(fDistance < fOldDistance)
										{
											CAttackCommand* pAttackCommand = new CAttackCommand(pAttackable);
											pMoveAttackCommand->SetAttackCommand(pAttackCommand);
										}
									}

								}
								return false;
							}


							//CAttackCommand* pAttackCommand = dynamic_cast<CAttackCommand*>(pUnitObject->m_UnitAICommand);

							//if(pAttackCommand)
							//{
							//	CGameObject* pPreviousTarget = dynamic_cast<CGameObject*>(pAttackCommand->GetTarget());

							//	if(pPreviousTarget)
							//	{
							//		D3DXVec2Subtract(&vDistance, &pPreviousTarget->GetPosition(), &pUnitObject->GetPosition());
							//		float fOldDistance = D3DXVec2Length(&vDistance);

							//		if(fDistance < fOldDistance)
							//			pAttackCommand->SetTarget(pAttackable);
							//	}
							//}
						}
						else
						{
							CAttackCommand* pAttackCommand = new CAttackCommand(pAttackable);
							pUnitObject->SetUnitAICommand(pAttackCommand);
						}
					}
					
				}

				CStove* pStove = dynamic_cast<CStove*>(pObject);

				if(pStove)
				{
					D3DXVECTOR2 vHalfSize = D3DXVECTOR2((float)((pStove->GetCollisionRectangle().right - pStove->GetCollisionRectangle().left)/2), (float)((pStove->GetCollisionRectangle().bottom - pStove->GetCollisionRectangle().top)/2));

					D3DXVECTOR2 vRelativePos = (pStove->GetPosition() + vHalfSize);

					D3DXVECTOR2 vPointToCheck = D3DXVECTOR2(0.0f, 0.0f);

					if(pCaller->GetPositionX() > (vRelativePos.x + vHalfSize.x))
						vPointToCheck.x = (vRelativePos.x + vHalfSize.x);
					else if(pCaller->GetPositionX() < (vRelativePos.x - vHalfSize.x))
						vPointToCheck.x = (vRelativePos.x - vHalfSize.x);
					else
						vPointToCheck.x = pCaller->GetPositionX();

					if(pCaller->GetPositionY() > (vRelativePos.y + vHalfSize.y))
						vPointToCheck.y = (vRelativePos.y + vHalfSize.y);
					else if(pCaller->GetPositionY() < (vRelativePos.y - vHalfSize.y))
						vPointToCheck.y = (vRelativePos.y - vHalfSize.y);
					else
						vPointToCheck.y = pCaller->GetPositionY();

					D3DXVECTOR2 vDistance;
					D3DXVec2Subtract(&vDistance, &vPointToCheck, &pCaller->GetPosition()); 
					float fDistance = D3DXVec2Length(&vDistance);

					if(fDistance <= pCaller->GetPerceptionRange())
					{
						if(pCaller->m_UnitAICommand)
						{
							CAttackMoveCommand* pMoveAttackCommand = dynamic_cast<CAttackMoveCommand*>(pCaller->m_UnitAICommand);

							if(pMoveAttackCommand)
							{
								if(!pMoveAttackCommand->GetAttackCommand())
								{
									CAttackCommand* pAttackCommand = new CAttackCommand(pAttackable);
									pMoveAttackCommand->SetAttackCommand(pAttackCommand);
								}
								else
								{
									CGameObject* pPreviousTarget = dynamic_cast<CGameObject*>(pMoveAttackCommand->GetAttackCommand()->GetTarget());

									if(pPreviousTarget)
									{
										D3DXVec2Subtract(&vDistance, &pPreviousTarget->GetPosition(), &pCaller->GetPosition());
										float fOldDistance = D3DXVec2Length(&vDistance);

										if(fDistance < fOldDistance)
										{
											CAttackCommand* pAttackCommand = new CAttackCommand(pAttackable);
											pMoveAttackCommand->SetAttackCommand(pAttackCommand);
										}
									}

								}
								return false;
							}
						}
						else
						{
							CAttackCommand* pAttackCommand = new CAttackCommand(pAttackable);
							pCaller->SetUnitAICommand(pAttackCommand);
						}
					}
				}
			}
		}
	}
	return false;
}

bool CUnitObject::CheckNearbyFriendlyUnits(IObject* pObject, CUnitObject* pCaller)
{
	CUnitObject* pCheckUnit = dynamic_cast<CUnitObject*>(pObject);

	if(pCheckUnit)
	{
		if(pCheckUnit->GetTeamValue() == pCaller->GetTeamValue())
		{
			D3DXVECTOR2 vDistance;
			D3DXVec2Subtract(&vDistance, &pCheckUnit->GetPosition(), &pCaller->GetPosition());
			int nDistance = (int)D3DXVec2LengthSq(&vDistance);

			if(160000 >= nDistance)
			{
				pCaller->m_bNearbyUnits[pCheckUnit->GetBaseType()] = true;

				for(int i = 0; i < 4; ++i)
				{
					if(pCaller->m_bNearbyUnits[i] == false)
						return false;
				}

				return true;
			}
		}
	}
	return false;
}

//TELL MG IF YOU MODIFY THIS IN ANY WAY
void CUnitObject::ApplyDamage(short Damage)
{
	IAttackable::ApplyDamage(Damage);

	if(this->GetHealth() > this->GetMaxHealth())
	{
		this->SetHealth(this->GetMaxHealth());
	}
}

void CUnitObject::ScaleStats(float _percentage)
{
	m_usDefense = (unsigned short)(m_usDefense * _percentage);
	m_usAttack = (unsigned short)(m_usAttack * _percentage);
	m_fEvade *= (unsigned short)(m_fEvade * _percentage);
}

void CUnitObject::ChangeAvailableSkills(int _numSkills)
{
	switch(_numSkills)
	{
	case 0:
		if(m_Skills[0])
		{
			delete m_Skills[0];
			m_Skills[0] = NULL;
		}
	case 1:
		if(m_Skills[1])
		{
			delete m_Skills[1];
			m_Skills[1] = NULL;
		}
	case 2:
		if(m_Skills[2])
		{
			delete m_Skills[2];
			m_Skills[2] = NULL;
		}

	}
}

int CUnitObject::Attack(IAttackable* _pTarget)
{
	int damage = 0;

	if(GetAttackSpeed() <= GetAttackTimer())
	{
		SetAttackTimer(0.0f);

		if(m_AttackSound)
			m_AttackSound->Play(GetPosition());

		// IF you didnt miss the attack
		int parry = (rand() % 100) + 1;

		CUnitObject* pUnitEnemy = NULL;
		pUnitEnemy = dynamic_cast<CUnitObject*>(_pTarget);
		if(pUnitEnemy && parry > pUnitEnemy->GetEvade())
		{
			//Calculate the damage
			damage = GetAttack() - ((pUnitEnemy->GetDefense() / 100) * GetAttack());

			if(m_szBullet == "")
			{
				if(this->GetNWOwner())
				{
#if _DEBUG
					printf("Game: Attack from %s to %s\n", this->GetCreationString().c_str(), pUnitEnemy->GetCreationString().c_str());
#endif
					pUnitEnemy->ApplyDamage(damage);
				}
			}
			else
			{
				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BUG-011
				// BUG FIX START
				/////////////////////////////////////////////////////////////////
				if(this->GetNWOwner())
				{
					CProjectile* newProj = static_cast<CProjectile*>(CObjectFactory::GetInstance()->Construct(this->GetBullet(), GetObjectManagerOwner()));
					newProj->OnHit = BaconArrowHit;
					newProj->SetNWOwner(true);

					float fAngle = 0.0f;
					/*D3DXVECTOR2 vVectorToEnemy = pUnitEnemy->GetPosition() - GetPosition();
					D3DXVec2Normalize(&vVectorToEnemy, &vVectorToEnemy);
					float fDot = D3DXVec2Dot(&vVectorToEnemy, &D3DXVECTOR2(1.0f, 0.0f));
					float fLength =  D3DXVec2Length(&vVectorToEnemy);

					if (fLength == 0.0f)
						fAngle = 0.0f;
					else
					{
						fAngle = acos( fDot / fLength );

						if (_isnan(fAngle))
							fAngle = 0.0f;
					}

					if(fDot < 0.0f)
						fAngle = fAngle - (2 * D3DX_PI);*/

					newProj->SetupProjectile((GetPosition() - D3DXVECTOR2(0.0f, 32.0f)), pUnitEnemy, 100, damage, 0.0f);
					
					newProj->SendCreationPacket(this->GetBullet(), 3);
				}

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BUG-011
				//////////////////////////////////////////////////////////////////////////
			}

			return damage;
		}
		else if(pUnitEnemy && parry < pUnitEnemy->GetEvade())
			CFloatingText::SpawnFloatingText(this->GetObjectManagerOwner(), "Miss!", GetPosition(), D3DXVECTOR2(0.0f, -30.0f), 2.0f, 0.6f, 0.0f, D3DCOLOR_XRGB(255, 0, 0), false);

		CStove* testStove = dynamic_cast<CStove*>(_pTarget);
		if(testStove)
		{
			damage = GetAttack();
			if(m_szBullet == "")
			{
				if(this->GetNWOwner())
					testStove->ApplyDamage(damage);
			}
			else
			{
				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BUG-011
				// BUG FIX START
				/////////////////////////////////////////////////////////////////
				if(this->GetNWOwner())
				{
					CProjectile* newProj = static_cast<CProjectile*>(CObjectFactory::GetInstance()->Construct(this->GetBullet(), GetObjectManagerOwner()));
					newProj->OnHit = BaconArrowHit;
					newProj->SetNWOwner(true);

					float fAngle = 0.0f;
					/*D3DXVECTOR2 vVectorToEnemy = pUnitEnemy->GetPosition() - GetPosition();
					D3DXVec2Normalize(&vVectorToEnemy, &vVectorToEnemy);
					float fDot = D3DXVec2Dot(&vVectorToEnemy, &D3DXVECTOR2(1.0f, 0.0f));
					float fLength =  D3DXVec2Length(&vVectorToEnemy);

					if (fLength == 0.0f)
						fAngle = 0.0f;
					else
					{
						fAngle = acos( fDot / fLength );

						if (_isnan(fAngle))
							fAngle = 0.0f;
					}

					if(fDot < 0.0f)
						fAngle = fAngle - (2 * D3DX_PI);*/

					newProj->SetupProjectile((GetPosition() - D3DXVECTOR2(0.0f, 32.0f)), testStove, 100, damage, 0.0f);
					
					newProj->SendCreationPacket(this->GetBullet(), 3);
				}

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BUG-011
				//////////////////////////////////////////////////////////////////////////
			}
			return damage;
		}
	}
	return damage;
}

CSound* CUnitObject::GetAttackSound() const
{
	return m_AttackSound;
}

void CUnitObject::SetAttackSound(CSound* AttackSound)
{
	m_AttackSound = AttackSound;
}

void CUnitObject::AddParticleEmitterOver(CParticleEmitter* _emitter)
{
	m_OverParticleEmitters.push_back(_emitter);
}

void CUnitObject::AddParticleEmitterUnder(CParticleEmitter* _emitter)
{
	m_UnderParticleEmitters.push_back(_emitter);
}

void CUnitObject::RemoveParticleEmitterOver(CParticleEmitter* _emitter)
{
	m_OverParticleEmitters.remove(_emitter);
}

void CUnitObject::RemoveParticleEmitterUnder(CParticleEmitter* _emitter)
{
	m_UnderParticleEmitters.remove(_emitter);
}

void CUnitObject::Stun()
{
	m_SavedMovementSpeed = m_MovementSpeed;
	m_SavedAttackSpeed = m_fAttackSpeed;
	m_MovementSpeed = 0;
	SetAttackTimer(-60.0f);
}

void CUnitObject::Recover()
{
	m_MovementSpeed = m_SavedMovementSpeed;
	m_fAttackSpeed = m_SavedAttackSpeed;
	SetAttackTimer(GetAttackSpeed());
}

void CUnitObject::PlayVoice(void)
{
	if(m_pVoice)
	{
		if(!m_pVoice->IsPlaying())
			m_pVoice->Play(GetPosition());
	}
}

void CUnitObject::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{
	IUnitAICommand* UnitAICommand = GetUnitAICommand();

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-014
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	CMoveCommand* MoveCommand = dynamic_cast<CMoveCommand*>(UnitAICommand);
	if(MoveCommand)
	{
		// Unit arrived if they're within 50 pixels of the destination when they collide.
		bool ResetCommand = false;
		D3DXVECTOR2 Destination = MoveCommand->GetDestination();
		if(D3DXVec2LengthSq(&(Destination - GetPosition())) <= 2500)
			ResetCommand = true;
		else if(Object)
		{
			// Unit arrived if the object collided with is over the destination.
			RECT CollisionRectangle = Object->GetCollisionRectangle();
			if(Destination.y > CollisionRectangle.top && Destination.y < CollisionRectangle.bottom && Destination.x > CollisionRectangle.left && Destination.x < CollisionRectangle.right)
				ResetCommand = true;
		}
		if(ResetCommand)
			SetUnitAICommand(NULL);
	}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-014
	//////////////////////////////////////////////////////////////////////////

	CProjectile* pProj = dynamic_cast<CProjectile*>(Object);

	if(pProj == NULL)
		CGameObject::OnCollision(Object, Overlap);
}