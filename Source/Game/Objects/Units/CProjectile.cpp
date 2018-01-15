//////////////////////////////////////////////////////
// File: "CProjectile.cpp"
// Creator: MG
// Creation Date: 10/9/09
// Last Modified: 10/9/09
// Last Modified By: MG
//////////////////////////////////////////////////////

#include "CProjectile.h"
#include "CUnitObject.h"
#include "../Appliances/CStove.h"
#include "../../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include "../../../Managers/Bad Chef/Particles/CParticleManager.h"
#include "../../../Managers/Bad Chef/Audio/CAudioManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"

void CProjectile::SetupProjectile(D3DXVECTOR2 _startPos, CGameObject* _unitTracked, float _speed, short _damage, float fRotation)
{
	SetPosition(_startPos);
	m_UnitTracked = _unitTracked;
	m_Damage = _damage;
	m_Speed = _speed;
	SetRotation(fRotation);
}

void CProjectile::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{
	CGameObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CGameObject*>(Object))
	{
		if(testUnit == m_UnitTracked)
		{
			if(OnHit)
				OnHit(this);
			this->DoRemove();
		}
	}
}

void CProjectile::Update(float Delta)
{
	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(GetUnitTracked());

	if(pUnit)
	{
		if(pUnit->CheckIsDead())
		{
			DoRemove();
			return;
		}
	}

	D3DXVECTOR2 vel;
	D3DXVec2Subtract(&vel, &m_UnitTracked->GetPosition(), &GetPosition());
	D3DXVec2Normalize(&vel, &vel);
	D3DXVec2Scale(&vel, &vel, m_Speed);

	SetVelocity(vel);

	CGameObject::Update(Delta);
}

void CProjectile::Draw(void)
{
	GetSprite()->Draw(0, 0, GetPosition(), GetRotation());
}

CProjectile::FunctionPointer CProjectile::FindHitFunction(short index)
{
	switch(index)
	{
	case 1:
		return BlueberryShrapnelHit;
	case 2:
		return WaffleAmberHit;
	case 3:
		return BaconArrowHit;
	default:
		return NULL;
	}
}

void BlueberryShrapnelHit(CProjectile* proj)
{
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/ExplosionA.wav", "Effects")->Play(proj->GetPosition(), proj->GetVelocity());

	if(proj->GetNWOwner())
	{
		IAttackable* pAttackable = dynamic_cast<IAttackable*>(proj->GetUnitTracked());
		if(pAttackable)
			pAttackable->ApplyDamage(proj->GetDamage());
	}

	CParticleEmitter* pNewEmitter = CParticleManager::GetInstance()->GetParticleEffect("BlueBerryExplosion");
	pNewEmitter->SetPosition(proj->GetPosition());
	D3DXVECTOR2 normVec;
	D3DXVec2Normalize(&normVec, &proj->GetVelocity());
	pNewEmitter->SetParticleVelocity(normVec * 400);
	pNewEmitter->InitParticles();
	pNewEmitter->SetIsLooping(false);
	proj->GetObjectManagerOwner()->AddObjectToLayer(3, pNewEmitter);
	pNewEmitter->Play(false);

}

void WaffleAmberHit(CProjectile* proj)
{
	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(proj->GetUnitTracked());

	if(pUnit)
	{
		CStatusEffect* amber = new CStatusEffect();
		amber->Begin = StartEffect_Waffle_FlavoredSyrup;
		amber->End = EndEffect_Waffle_FlavoredSyrup;
		amber->SetTimer(4);

		pUnit->AddStatusEffect(amber);
	}
}

void BaconArrowHit(CProjectile* proj)
{
	if(proj->GetNWOwner())
	{
		IAttackable* pAttackable = dynamic_cast<IAttackable*>(proj->GetUnitTracked());
		if(pAttackable)
			pAttackable->ApplyDamage(proj->GetDamage());
	}
}

void CProjectile::SendCreationPacket(std::string _creationString, short _hitFunction)
{
	NW_MANAGER_START
		if(CNetworkManager::GetInstance()->IsConnected())
		{
			CUnitObject* pUnit = dynamic_cast<CUnitObject*>(m_UnitTracked);
			CStove* pStove = dynamic_cast<CStove*>(m_UnitTracked);
			if(pUnit)
				CNetworkManager::GetInstance()->SendCreateProjectilePacket(_creationString, _hitFunction, m_Speed, m_Damage, GetPositionX(), GetPositionY(), pUnit->GetSharedID(), pUnit->GetNWOwner());
			else if(pStove)
				CNetworkManager::GetInstance()->SendCreateProjectilePacket(_creationString, _hitFunction, m_Speed, m_Damage, GetPositionX(), GetPositionY(), pStove->GetSharedID(), pStove->GetNWOwner());
		}
	NW_MANAGER_END
}


IObject* CreateBlueberryShrapnel(CObjectManager* pManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Projectiles/BlueberryBomb.spr");
	if(!Sprite)
		return NULL;

	CProjectile* newRsc = new CProjectile();
	newRsc->SetSprite(Sprite);
	pManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

IObject* CreateBaconArrow(CObjectManager* pManager)
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Projectiles/BaconArrow.spr");
	if(!Sprite)
		return NULL;

	CProjectile* newProj = new CProjectile();
	newProj->SetSprite(Sprite);
	pManager->AddObjectToLayer(3, newProj);
	return newProj;
}