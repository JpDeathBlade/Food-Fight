//////////////////////////////////////////////////////
// File: "CSkill.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "IAbility.h"

#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include "../../../Managers/Bad Chef/Networking/NetworkUpdateClasses.h"
#include "../../../Managers/Bad Chef/CObjectFactory.h"
#include "../../../Managers/Bad Chef/Particles/CParticleManager.h"
#include "../../../Managers/Bad Chef/Audio/CAudioManager.h"
#include "../GUI Objects/CCursor.h"
#include "CUnitObject.h"
#include "CSkilledUnitObject.h"
#include "CStatusEffect.h"
#include "CProjectile.h"
#include "../../Unit Commands/CMoveCommand.h"
#include "../Tiles/CTileStage.h"
#include "../Appliances/CStove.h"

IAbility::IAbility()
{
	m_EnableTime = 0;
	m_Frame.SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/Button.spr"));
	m_Frame.SetFontFileName("Resources/Data/Font.spr");
	m_Frame.SetStringOffset(6,6);
	m_Frame.SetSize(256, 196);
	m_Frame.SetPosition(1024-256, 768-196-128);
	m_Frame.SetFontScale(0.6f, 0.6f);
	m_Frame.FitTextToSize(true);
}

struct SCasterAndEffect
{
	SCasterAndEffect()
	{
		caster = NULL;
		effect = NULL;
	}
	CUnitObject* caster;
	CStatusEffect* effect;
};

void CSkill_GOD_Suicide::operator()()
{
	GetOwner()->DoRemove();
}

void CSkill_GOD_Nuke::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
	CCursor::GetInstance()->OverrideClick(new CSkill_GOD_Nuke_Override);
}

void CSkill_GOD_Nuke_Override::operator()()
{
	CCursor::GetInstance()->GetHoveredOverObject()->DoRemove();

	CCursor::GetInstance()->OverrideClick(NULL);
	CCursor::GetInstance()->ResetSprite();
	delete this;
}

//////////////////////////////////////////////

bool StartEffect_Toast_Crust(CUnitObject* _unit)
{	
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/Crunch.wav", "Effects")->Play(_unit->GetPosition(), _unit->GetVelocity());

	_unit->AddDefenseMultiply(0.2f);
	return true;
}

bool EndEffect_Toast_Crust(CUnitObject* _unit)
{
	_unit->AddDefenseMultiply(-0.2f);
	return true;
}

void CSkill_Toast_Crust::operator()()
{
	SetCooldown(30);
	CStatusEffect* effect_Crust = new CStatusEffect();
	effect_Crust->Begin = StartEffect_Toast_Crust;
	effect_Crust->End   = EndEffect_Toast_Crust;
	effect_Crust->SetTimer(10);
	effect_Crust->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/1.spr"));

	if(GetOwner()->GetNWOwner())
	{
		NW_MANAGER_START
			if(CNetworkManager::GetInstance()->IsConnected())
			{
				CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
			}
		NW_MANAGER_END
	}
}

/////////

bool StartEffect_Toast_Yeast(CUnitObject* _unit)
{	
	_unit->AddDefenseMultiply(0.2f);
	_unit->AddAttackMultiply(0.2f);
	_unit->AddEvadeMultiply(0.2f);
	_unit->AddMovementMultiply(0.2f);
	return true;
}

bool EndEffect_Toast_Yeast(CUnitObject* _unit)
{
	_unit->AddDefenseMultiply(-0.2f);
	_unit->AddAttackMultiply(-0.2f);
	_unit->AddEvadeMultiply(-0.2f);
	_unit->AddMovementMultiply(-0.2f);
	return true;
}

void CSkill_Toast_Yeast::operator()()
{
	Disable();
	CStatusEffect* effect_Yeast = new CStatusEffect();
	effect_Yeast->Begin = StartEffect_Toast_Yeast;
	effect_Yeast->End   = EndEffect_Toast_Yeast;
	effect_Yeast->SetTimer(20);
	effect_Yeast->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/2.spr"));

	if(GetOwner()->AddStatusEffect(effect_Yeast))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 1);
				}
			NW_MANAGER_END
		}
	}
}

/////////

void CSkill_Toast_SlicedBread::operator()()
{
	CSkilledUnitObject* newUnit = NULL;
	if(newUnit = dynamic_cast<CSkilledUnitObject*>(GetOwner()))
	{
		if(newUnit->GetLevel() >= 4)
		{
			for(int i = 0; i < 3; i++)
			{
				CUnitObject* _newToast = (CUnitObject*)CObjectFactory::GetInstance()->Construct("Toast", GetOwner()->GetObjectManagerOwner());
				_newToast->SetPosition(GetOwner()->GetPositionX() - ((i-1)*64), GetOwner()->GetPositionY());
				_newToast->SetTeamValue(GetOwner()->GetTeamValue());
				if(GetOwner()->GetNWOwner())
				{
					NW_MANAGER_START
						if(CNetworkManager::GetInstance()->IsConnected())
						{
							CNetworkManager::GetInstance()->SendCreateUnitPacket(_newToast, "Toast");
						}
					NW_MANAGER_END
				}
			}

			GetOwner()->ApplyDamage(GetOwner()->GetHealth());
			GetOwner()->SetHealth(0);
		}
	}
}

///////////////

void CSkill_Sunny_Benedict::operator()()
{
	SetCooldown(60);
	GetOwner()->SetHealth(GetOwner()->GetHealth() + (unsigned short)(GetOwner()->GetMaxHealth() * 0.3f));
	if(GetOwner()->GetNWOwner())
	{
		NW_MANAGER_START
			if(CNetworkManager::GetInstance()->IsConnected())
			{
				CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
			}
		NW_MANAGER_END
	}
}

////////////////

bool StartEffect_Sunny_Caviar(CUnitObject* _unit)
{
	_unit->AddMovementMultiply(0.3f);
	return true;
}

bool EndEffect_Sunny_Caviar(CUnitObject* _unit)
{
	_unit->AddMovementMultiply(-0.3f);
	return true;
}

void CSkill_Sunny_Caviar::operator()()
{
	SetCooldown(60);
	CStatusEffect* effect_Caviar = new CStatusEffect();
	effect_Caviar->Begin = StartEffect_Sunny_Caviar;
	effect_Caviar->End = EndEffect_Sunny_Caviar;
	effect_Caviar->SetTimer(20);
	effect_Caviar->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/2.spr"));

	if(GetOwner()->AddStatusEffect(effect_Caviar))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 1);
				}
			NW_MANAGER_END
		}
	}
}

////////////////

bool StartEffect_Sunny_OverEasy(CUnitObject* _unit)
{

	for(std::list<CStatusEffect*>::iterator Iterator = _unit->GetStatusEffects()->begin(); Iterator != _unit->GetStatusEffects()->end(); ++Iterator)
		if((*Iterator)->Begin == StartEffect_Sunny_OverEasy)
			return false;
	
	_unit->AddMovementMultiply(0.4f);
	return true;
}

bool EndEffect_Sunny_OverEasy(CUnitObject* _unit)
{
	_unit->AddMovementMultiply(-0.4f);
	return true;
}

bool Cast_OverEasy(IObject* obj, SCasterAndEffect* cast)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		D3DXVECTOR2 dist;
		D3DXVec2Subtract(&dist, &testUnit->GetPosition(), &cast->caster->GetPosition());
		if(testUnit->GetTeamValue() == cast->caster->GetTeamValue() && 
			testUnit->GetRange() >= 150 &&
			D3DXVec2LengthSq(&dist) < 100000)
		{
			testUnit->AddStatusEffect(cast->effect);
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendStatusEffectPacket(testUnit, 30, 1, 0, 2, 0, 0, 0);
				}
			NW_MANAGER_END
		}
	}
	return false;
}

void CSkill_Sunny_OverEasy::operator()()
{
	SetCooldown(80);

	CStatusEffect* effect_OverEasy = new CStatusEffect();
	effect_OverEasy->AcquireReference();
	effect_OverEasy->Begin = StartEffect_Sunny_OverEasy;
	effect_OverEasy->End = EndEffect_Sunny_OverEasy;
	effect_OverEasy->SetTimer(30);
	effect_OverEasy->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/3.spr"));

	SCasterAndEffect cast;
	cast.caster = GetOwner();
	cast.effect = effect_OverEasy;
	GetOwner()->GetObjectManagerOwner()->Iterate<SCasterAndEffect>(&Cast_OverEasy, &cast);

	effect_OverEasy->ReleaseReference();
}

////////////////

bool StartEffect_Scrambled_BeatIt(CUnitObject* _unit)
{
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/EggBeater.wav", "Effects")->Play(_unit->GetPosition(), _unit->GetVelocity());
	_unit->AddMovementMultiply(0.4f);
	return true;
}

bool EndEffect_Scrambled_BeatIt(CUnitObject* _unit)
{
	_unit->AddMovementMultiply(-0.4f);
	return true;
}

void CSkill_Scrambled_BeatIt::operator()()
{
	SetCooldown(30);
	CStatusEffect* effect_BeatIt = new CStatusEffect();
	effect_BeatIt->Begin = StartEffect_Scrambled_BeatIt;
	effect_BeatIt->End = EndEffect_Scrambled_BeatIt;
	effect_BeatIt->SetTimer(12);
	effect_BeatIt->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/1.spr"));

	if(GetOwner()->AddStatusEffect(effect_BeatIt))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
				}
			NW_MANAGER_END
		}
	}
}

////////////////

bool StartEffect_Scrambled_Raw(CUnitObject* _unit)
{
	_unit->Stun();
	_unit->AddDefenseMultiply(100.0f);
	return true;
}

bool EndEffect_Scrambled_Raw(CUnitObject* _unit)
{
	_unit->Recover();
	_unit->AddDefenseMultiply(-100.0f);
	return true;
}

void CSkill_Scrambled_Raw::operator()()
{
	SetCooldown(15);
	CStatusEffect* effect_Raw = new CStatusEffect();
	effect_Raw->Begin = StartEffect_Scrambled_Raw;
	effect_Raw->End = EndEffect_Scrambled_Raw;
	effect_Raw->SetTimer(8);
	effect_Raw->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/2.spr"));

	if(GetOwner()->AddStatusEffect(effect_Raw))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 1);
				}
			NW_MANAGER_END
		}
	}
}

////////////////

bool StartEffect_Scrambled_Easter(CUnitObject* _unit)
{
	_unit->AddEvadeMultiply(0.4f);
	return true;
}

bool EndEffect_Scrambled_Easter(CUnitObject* _unit)
{
	_unit->AddEvadeMultiply(-0.4f);
	return true;
}

void CSkill_Scrambled_Easter::operator()()
{
	SetCooldown(80);
	CStatusEffect* effect_Easter = new CStatusEffect();
	effect_Easter->Begin = StartEffect_Scrambled_Easter;
	effect_Easter->End = EndEffect_Scrambled_Easter;
	effect_Easter->SetTimer(10);
	effect_Easter->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/3.spr"));

	GetOwner()->AddStatusEffect(effect_Easter);
}

////////////////

bool StartEffect_Sausage_Kielbasa(CUnitObject* _unit)
{
	_unit->AddAttackMod(0.2f);
	return true;
}

bool EndEffect_Sausage_Kielbasa(CUnitObject* _unit)
{
	_unit->AddAttackMod(-0.2f);
	return true;
}

void CSkill_Sausage_Kielbasa::operator()()
{
	SetCooldown(60);
	CStatusEffect* effect_Kielbasa = new CStatusEffect();
	effect_Kielbasa->Begin = StartEffect_Sausage_Kielbasa;
	effect_Kielbasa->End = EndEffect_Sausage_Kielbasa;
	effect_Kielbasa->SetTimer(12);
	effect_Kielbasa->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/1.spr"));

	if(GetOwner()->AddStatusEffect(effect_Kielbasa))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
				}
			NW_MANAGER_END
		}
	}
}

////////////////

bool StartEffect_Sausage_Bratwurste(CUnitObject* _unit)
{
	for(std::list<CStatusEffect*>::iterator Iterator = _unit->GetStatusEffects()->begin(); Iterator != _unit->GetStatusEffects()->end(); ++Iterator)
		if((*Iterator)->Begin == StartEffect_Sausage_Bratwurste)
			return false;
	
	_unit->AddAttackMod(0.2f);
	_unit->AddMovementMod(0.2f);
	return true;
}

bool EndEffect_Sausage_Bratwurste(CUnitObject* _unit)
{
	_unit->AddAttackMod(-0.2f);
	_unit->AddMovementMod(-0.2f);
	return true;
}

void CSkill_Sausage_Bratwurste::operator()()
{
	SetCooldown(60);
	CStatusEffect* effect_Bratwurste = new CStatusEffect();
	effect_Bratwurste->Begin = StartEffect_Sausage_Bratwurste;
	effect_Bratwurste->End = EndEffect_Sausage_Bratwurste;
	effect_Bratwurste->SetTimer(12);
	effect_Bratwurste->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/2.spr"));

	if(GetOwner()->AddStatusEffect(effect_Bratwurste))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
				}
			NW_MANAGER_END
		}
	}
}

////////////////

bool StartEffect_Sausage_SPAM(CUnitObject* _unit)
{
	for(std::list<CStatusEffect*>::iterator Iterator = _unit->GetStatusEffects()->begin(); Iterator != _unit->GetStatusEffects()->end(); ++Iterator)
		if((*Iterator)->Begin == StartEffect_Sausage_SPAM)
			return false;

	return true;
}

bool EndEffect_Sausage_SPAM(CUnitObject* _unit)
{
	return true;
}

void CSkill_Sausage_SPAM::operator()()
{
	CStatusEffect* effect_SPAM = new CStatusEffect();
	effect_SPAM->Begin = StartEffect_Sausage_SPAM;
	effect_SPAM->End = EndEffect_Sausage_SPAM;
	effect_SPAM->SetTimer(1);
	effect_SPAM->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/3.spr"));

	GetOwner()->AddStatusEffect(effect_SPAM);
}

////////////////

bool StartEffect_Ham_Butcher(CUnitObject* _unit)
{
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/SharpenKnife.wav", "Effects")->Play(_unit->GetPosition(), _unit->GetVelocity());

	//Give an axe?
	return true;
}

bool UpdateButcher(CUnitObject* unit, CStatusEffect* effect)
{
	if(((CSkilledUnitObject*)unit)->GetExperience() > ((CExpTrackEffect*)effect)->GetLastExp())
	{
		unit->AddAttackMod(1);
		((CExpTrackEffect*)effect)->AddTracker(1);
		((CExpTrackEffect*)effect)->SetLastExp(((CSkilledUnitObject*)unit)->GetExperience());
	}
	return true;
}

bool EndEffect_Ham_Butcher(CUnitObject* _unit, CStatusEffect* effect)
{
	_unit->AddAttackMod(float(-((CExpTrackEffect*)effect)->GetTracker()));
	return true;
}

void CSkill_Ham_Butcher::operator()()
{
	CExpTrackEffect* butcher = new CExpTrackEffect();
	butcher->SetLastExp(((CSkilledUnitObject*)GetOwner())->GetExperience());
	butcher->SetTracker(0);
	butcher->Begin = StartEffect_Ham_Butcher;
	butcher->UpdateSender = UpdateButcher;
	butcher->EndSender = EndEffect_Ham_Butcher;
	butcher->SetTimer(12);

	GetOwner()->AddStatusEffect(butcher);
}

////////////////

bool UpdateRumpRoast(CUnitObject* unit, CStatusEffect* effect)
{
	((CSkilledUnitObject*)unit)->SetExperience(((CExpTrackEffect*)effect)->GetLastExp());
	return true;
}

void CSkill_Ham_RumpRoast::operator()()
{
	SetCooldown(200);
	CExpTrackEffect* noExp = new CExpTrackEffect();
	noExp->SetLastExp(((CSkilledUnitObject*)GetOwner())->GetExperience());
	noExp->UpdateSender = UpdateRumpRoast;
	noExp->SetTimer(200);
	GetOwner()->AddStatusEffect(noExp);
	GetOwner()->ApplyDamage(-(GetOwner()->GetMaxHealth() - GetOwner()->GetHealth()));
	GetOwner()->SetHealth(GetOwner()->GetMaxHealth());
}

////////////////

bool StartEffect_Ham_HoneyRoasted(CUnitObject* _unit)
{
	_unit->AddAttackMultiply(0.4f);
	_unit->AddMovementMultiply(-0.3f);

	return true;
}

bool EndEffect_Ham_HoneyRoasted(CUnitObject* _unit)
{
	_unit->AddAttackMultiply(-0.4f);
	_unit->AddMovementMultiply(0.3f);
	return true;
}

void CSkill_Ham_HoneyRoasted::operator()()
{
	SetCooldown(60);
	CStatusEffect* effect_HoneyRoasted = new CStatusEffect();
	effect_HoneyRoasted->Begin = StartEffect_Ham_HoneyRoasted;
	effect_HoneyRoasted->End = EndEffect_Ham_HoneyRoasted;
	effect_HoneyRoasted->SetTimer(20);
	effect_HoneyRoasted->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/3.spr"));

	if(GetOwner()->AddStatusEffect(effect_HoneyRoasted))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
				}
			NW_MANAGER_END
		}
	}
}

////////////////

void CSkill_Pancake_Buttermilk::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
 	CCursor::GetInstance()->OverrideClick(new CSkill_Pancake_Buttermilk_Override(GetOwner()));
	GetOwner()->AcquireReference();
}

void CSkill_Pancake_Buttermilk_Override::operator()()
{
	if(GetOwner()->CheckIsDead())
	{
		GetOwner()->ReleaseReference();
		CCursor::GetInstance()->OverrideClick(NULL);
 		CCursor::GetInstance()->ResetSprite();
		return;
	}

	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/Chimes.wav", "Effects")->Play(CCursor::GetInstance()->GetPosition());

	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(CCursor::GetInstance()->GetHoveredOverObject());

	if(pUnit)
	{
		if(pUnit->GetTeamValue() == GetOwner()->GetTeamValue())
		{
			//Heal here
			pUnit->ApplyDamage(((short)pUnit->GetMaxHealth())/-2);
			pUnit->SetHealth(pUnit->GetHealth() + pUnit->GetMaxHealth()/2);
			
			//Play particle effect here
			CParticleEmitter* pNewEmitter = CParticleManager::GetInstance()->GetParticleEffect("Heal");
			pNewEmitter->SetIsLooping(false);
			pNewEmitter->InitParticles();
			pNewEmitter->SetObjectToTrack(pUnit);
			pNewEmitter->SetOffset(D3DXVECTOR2(-48.0f, -32.0f));
			pUnit->GetObjectManagerOwner()->AddObjectToLayer(3, pNewEmitter);
			pNewEmitter->Play(false);

		}
	}

	GetOwner()->ReleaseReference();
	CCursor::GetInstance()->OverrideClick(NULL);
 	CCursor::GetInstance()->ResetSprite();
 	delete this;
}

////

void CSkill_Pancake_Flappjack::operator()()
{
	SetCooldown(60);
	GetOwner()->SetUnitAICommand(NULL);
	std::vector<CStove*> stoves = CTileStage::GetInstance()->GetStoves();
	for(int i = 0; i < 2; i++)
	{
		if(stoves[i]->GetTeamValue() == GetOwner()->GetTeamValue())
		{
			RECT spawnRect = stoves[i]->GetSpawnRect();
			int x = spawnRect.left + (rand()%(spawnRect.right - spawnRect.left));
			int y = spawnRect.top + (rand()%(spawnRect.bottom - spawnRect.top));
			GetOwner()->SetPosition((float)x,(float)y);
			GetOwner()->SendUpdatePacket();
		}
	}

	//Do animation???????
}

////

void CSkill_Pancake_Topping_Click::operator()()
{
	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(CCursor::GetInstance()->GetHoveredOverObject());

	if(GetOwner()->CheckIsDead())
	{
		GetOwner()->ReleaseReference();
		CCursor::GetInstance()->OverrideClick(NULL);
 		CCursor::GetInstance()->ResetSprite();
		return;
	}

	if(pUnit)
	{
		if(pUnit->GetTeamValue() == GetOwner()->GetTeamValue())
		{
			((CSkilledUnitObject*)pUnit)->LevelUp();
			pUnit->SetMovementSpeed((unsigned short)(pUnit->GetMovementSpeed() * 0.7f));
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUnitStatUpdatePacket(pUnit);
				}
			NW_MANAGER_END
		}
	}
	GetOwner()->ReleaseReference();
	CCursor::GetInstance()->OverrideClick(NULL);
 	CCursor::GetInstance()->ResetSprite();
}

void CSkill_Pancake_Topping::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
	CSkill_Pancake_Topping_Click* newFunctor =  new CSkill_Pancake_Topping_Click(GetOwner());
 	CCursor::GetInstance()->OverrideClick(newFunctor);
	GetOwner()->AcquireReference();

	SetCooldown(300);
}

////////////////////////////////

bool Cast_Sap(IObject* obj, SCasterAndEffect* cast)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		D3DXVECTOR2 dist;
		D3DXVec2Subtract(&dist, &testUnit->GetPosition(), &cast->caster->GetPosition());
		if(testUnit->GetTeamValue() == cast->caster->GetTeamValue() && 
			D3DXVec2LengthSq(&dist) < 750000)
		{
			testUnit->AddStatusEffect(cast->effect);

			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendStatusEffectPacket(testUnit, 15, 8, 0, 9, 0, 0, 0);
				}
			NW_MANAGER_END
		}
	}
	return false;
}

bool StartEffect_Waffle_Sap(CUnitObject* _unit)
{
	_unit->AddMovementMultiply(0.3f);
	return true;
}

bool EndEffect_Waffle_Sap(CUnitObject* _unit)
{
	_unit->AddMovementMultiply(-0.3f);
	return true;
}

void CSkill_Waffle_Sap::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
	CCursor::GetInstance()->OverrideClick(new CSkill_Waffle_Sap_Override(GetOwner()));
	GetOwner()->AcquireReference();
}

void CSkill_Waffle_Sap_Override::operator()()
{
	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(CCursor::GetInstance()->GetHoveredOverObject());

	if(GetOwner()->CheckIsDead())
	{
		CCursor::GetInstance()->OverrideClick(NULL);
 		CCursor::GetInstance()->ResetSprite();
		GetOwner()->ReleaseReference();
		return;
	}

	if(pUnit)
	{
		GetOwner()->GetSkill(1)->SetCooldown(45);
		CStatusEffect* effect_Sap = new CStatusEffect();
		effect_Sap->Begin = StartEffect_Waffle_Sap;
		effect_Sap->End = EndEffect_Waffle_Sap;
		effect_Sap->SetTimer(15);

		SCasterAndEffect cast;
		cast.caster = pUnit;
		cast.effect = effect_Sap;
		pUnit->GetObjectManagerOwner()->Iterate<SCasterAndEffect>(&Cast_Sap, &cast);
	}

	GetOwner()->ReleaseReference();
	CCursor::GetInstance()->OverrideClick(NULL);
 	CCursor::GetInstance()->ResetSprite();
 	delete this;
}

////

bool StartEffect_Waffle_BelgianSurprise(CUnitObject* _unit)
{
	_unit->AddAttackMultiply(0.3f);
	return true;
}

bool EndEffect_Waffle_BelgianSurprise(CUnitObject* _unit)
{
	_unit->AddAttackMultiply(-0.3f);
	return true;
}

void CSkill_Waffle_BelgianSurprise::operator()()
{
	SetCooldown(70);
	CStatusEffect* effect_BelgianSurprise = new CStatusEffect();
	effect_BelgianSurprise->Begin = StartEffect_Waffle_BelgianSurprise;
	effect_BelgianSurprise->End = EndEffect_Waffle_BelgianSurprise;
	effect_BelgianSurprise->SetTimer(20);
	effect_BelgianSurprise->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/2.spr"));

	if(GetOwner()->AddStatusEffect(effect_BelgianSurprise))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
				}
			NW_MANAGER_END
		}
	}
}

////

bool StartEffect_Waffle_FlavoredSyrup(CUnitObject* _unit)
{
	_unit->Stun();
	return true;
}

bool EndEffect_Waffle_FlavoredSyrup(CUnitObject* _unit)
{
	_unit->Recover();
	return true;
}

void CSkill_Waffle_FlavoredSyrup_Click::operator()()
{
	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(CCursor::GetInstance()->GetHoveredOverObject());

	if(GetOwner()->CheckIsDead())
	{
		GetOwner()->ReleaseReference();
		CCursor::GetInstance()->OverrideClick(NULL);
 		CCursor::GetInstance()->ResetSprite();
		return;
	}

	if(pUnit)
	{
		if(pUnit->GetTeamValue() != GetOwner()->GetTeamValue())
		{
			if(CheckPathIsTraversable(GetOwner()->GetPosition(), CCursor::GetInstance()->GetPosition(), GetOwner()))
			{
				GetOwner()->GetSkill(2)->SetCooldown(45);
				CProjectile* newProj = static_cast<CProjectile*>(CObjectFactory::GetInstance()->Construct("BlueberryShrapnel", GetOwner()->GetObjectManagerOwner()));
				newProj->OnHit = WaffleAmberHit;
				newProj->SetupProjectile(GetOwner()->GetPosition(), pUnit, 1000, 0);

				newProj->SendCreationPacket("BlueberryShrapnel", 2);
			}
		}
	}
	GetOwner()->ReleaseReference();
	CCursor::GetInstance()->OverrideClick(NULL);
 	CCursor::GetInstance()->ResetSprite();
}

void CSkill_Waffle_FlavoredSyrup::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
	CSkill_Waffle_FlavoredSyrup_Click* newFunctor =  new CSkill_Waffle_FlavoredSyrup_Click(GetOwner());
 	CCursor::GetInstance()->OverrideClick(newFunctor);
	GetOwner()->AcquireReference();
}

//////////////////////////////////////////////////////////
//						French Toast
/////////////////////////////////////////////////////////

bool BeginSugar(CUnitObject* unit)
{
	unit->AddMovementMultiply(0.4f);
	return true;
}

bool EndSugar(CUnitObject* unit)
{
	unit->AddMovementMultiply(-0.4f);
	return true;
}

void CSkill_FrenchToast_PowderedSugar::operator()()
{
	SetCooldown(45);
	CStatusEffect* sugar = new CStatusEffect();
	sugar->Begin = BeginSugar;
	sugar->End = EndSugar;
	sugar->SetTimer(10);

	if(GetOwner()->AddStatusEffect(sugar))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
				}
			NW_MANAGER_END
		}
	}
}

bool BeginBaguette(CUnitObject* unit)
{
	unit->AddDefenseMultiply(0.2f);
	return true;
}

bool EndBaguette(CUnitObject* unit)
{
	unit->AddDefenseMultiply(-0.2f);
	return true;
}

void CSkill_FrenchToast_Baguette::operator()()
{
	SetCooldown(45);
	CStatusEffect* baguette = new CStatusEffect();
	baguette->Begin = BeginBaguette;
	baguette->End = EndBaguette;
	baguette->SetTimer(15);

	if(GetOwner()->AddStatusEffect(baguette))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 1);
				}
			NW_MANAGER_END
		}
	}
}

void CSkill_FrenchToast_EggWash::operator()()
{
	SetCooldown(10);
}

//////////////////////////////////////////////////////////
//						Blueberry Muffin
/////////////////////////////////////////////////////////

bool BeginScone(CUnitObject* unit)
{
	unit->AddDefenseMultiply(0.3f);
	return true;
}

bool EndScone(CUnitObject* unit)
{
	unit->AddDefenseMod(-0.3f);
	return true;
}

void CSkill_Blueberry_Scone::operator()()
{
	SetCooldown(45);

	CStatusEffect* scone = new CStatusEffect();
	scone->SetTimer(15);
	scone->Begin = BeginScone;
	scone->End = EndScone;

	if(GetOwner()->AddStatusEffect(scone))
	{
		if(GetOwner()->GetNWOwner())
		{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
				}
			NW_MANAGER_END
		}
	}
	
}

void CSkill_Blueberry_Shrapnel_Click::operator()()
{
	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(CCursor::GetInstance()->GetHoveredOverObject());

	if(GetOwner()->CheckIsDead())
	{
		CCursor::GetInstance()->OverrideClick(NULL);
 		CCursor::GetInstance()->ResetSprite();
		GetOwner()->ReleaseReference();
		return;
	}

	if(pUnit)
	{
		if(pUnit->GetTeamValue() != GetOwner()->GetTeamValue())
		{
			if(CheckPathIsTraversable(GetOwner()->GetPosition(), CCursor::GetInstance()->GetPosition(), GetOwner()))
			{
				GetOwner()->GetSkill(1)->SetCooldown(45);
				CProjectile* newProj = static_cast<CProjectile*>(CObjectFactory::GetInstance()->Construct("BlueberryShrapnel", GetOwner()->GetObjectManagerOwner()));
				newProj->OnHit = BlueberryShrapnelHit;
				newProj->SetNWOwner(true);
				newProj->SetupProjectile(GetOwner()->GetPosition(), pUnit, 1000, 30);
				
				newProj->SendCreationPacket("BlueberryShrapnel", 1);
			}
		}
	}
	CCursor::GetInstance()->OverrideClick(NULL);
 	CCursor::GetInstance()->ResetSprite();
	GetOwner()->ReleaseReference();
}

void CSkill_Blueberry_Shrapnel::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
	CSkill_Blueberry_Shrapnel_Click* newFunctor =  new CSkill_Blueberry_Shrapnel_Click(GetOwner());
 	CCursor::GetInstance()->OverrideClick(newFunctor);
	GetOwner()->AcquireReference();
}

bool BeginBlueAndSweet(CUnitObject* unit)
{
	//Shade unit blue/purple?
	return true;
}

bool UpdateBlueAndSweet(CUnitObject* unit, CStatusEffect* effect)
{
	if(((CSkilledUnitObject*)unit)->GetExperience() > ((CExpTrackEffect*)effect)->GetLastExp())
	{
		unit->ApplyDamage(-((((CSkilledUnitObject*)unit)->GetExperience() - ((CExpTrackEffect*)effect)->GetLastExp()) / 2));	//Change the 2 to modify how much health he gets
		unit->SetHealth(unit->GetHealth() + ((((CSkilledUnitObject*)unit)->GetExperience() - ((CExpTrackEffect*)effect)->GetLastExp()) / 2));
		((CExpTrackEffect*)effect)->SetLastExp(((CSkilledUnitObject*)unit)->GetExperience());
	}
	return true;
}

bool EndBlueAndSweet(CUnitObject* unit)
{
	//Remove shade?
	return true;
}

void CSkill_Blueberry_BlueAndSweet::operator()()
{
	SetCooldown(60);
	CExpTrackEffect* blueAndSweet = new CExpTrackEffect();
	blueAndSweet->SetTimer(10);
	blueAndSweet->Begin = BeginBlueAndSweet;
	blueAndSweet->UpdateSender = UpdateBlueAndSweet;
	blueAndSweet->End = EndBlueAndSweet;
	blueAndSweet->SetLastExp(((CSkilledUnitObject*)GetOwner())->GetExperience());

	GetOwner()->AddStatusEffect(blueAndSweet);

	NW_MANAGER_START
		if(CNetworkManager::GetInstance()->IsConnected())
		{
			CNetworkManager::GetInstance()->SendStatusEffectPacket(GetOwner(), 10, 3, 0, 4, 0, 0, 0);
		}
	NW_MANAGER_END
}

//////////////////////////////////////////////////////////
//						Grapefruit
/////////////////////////////////////////////////////////

void CSkill_Grapefruit_BarrelRoll_Click::operator()()
{
	if(GetOwner()->CheckIsDead())
	{
		GetOwner()->ReleaseReference();
		CCursor::GetInstance()->OverrideClick(NULL);
 		CCursor::GetInstance()->ResetSprite();
		return;
	}

	if(CheckPathIsTraversable(GetOwner()->GetPosition(), CCursor::GetInstance()->GetPosition(), GetOwner()))
	{
		GetOwner()->SetUnitAICommand(NULL);
		D3DXVECTOR2 force;
		D3DXVec2Subtract(&force, &CCursor::GetInstance()->GetPosition(), &GetOwner()->GetPosition());
		D3DXVec2Normalize(&force, &force);
		D3DXVec2Scale(&force, &force, 600);
		GetOwner()->SetForce(SPEED_FORCE, force);
	}
	else
	{
		//We cannot barrel roll
	}

	GetOwner()->ReleaseReference();
	CCursor::GetInstance()->OverrideClick(NULL);
 	CCursor::GetInstance()->ResetSprite();
}

void CSkill_Grapefruit_BarrelRoll::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
	CSkill_Grapefruit_BarrelRoll_Click* newFunctor =  new CSkill_Grapefruit_BarrelRoll_Click(GetOwner());
 	CCursor::GetInstance()->OverrideClick(newFunctor);
	GetOwner()->AcquireReference();

	SetCooldown(10);
}

bool BeginCitrus(CUnitObject* unit)
{
	unit->AddDefenseMultiply(-0.3f);
	return true;
}

bool EndCitrus(CUnitObject* unit)
{
	unit->AddDefenseMultiply(0.3f);
	return true;
}

void CSkill_Grapefruit_Citrus_Click::operator()()
{
	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(CCursor::GetInstance()->GetHoveredOverObject());

	if(GetOwner()->CheckIsDead())
	{
		GetOwner()->ReleaseReference();
		CCursor::GetInstance()->OverrideClick(NULL);
 		CCursor::GetInstance()->ResetSprite();
		return;
	}

	if(pUnit)
	{
		if(pUnit->GetTeamValue() != GetOwner()->GetTeamValue())
		{
			//Check dist

			CStatusEffect* citrus = new CStatusEffect();
			citrus->SetTimer(15);
			citrus->Begin = BeginCitrus;
			citrus->End = EndCitrus;

			GetOwner()->GetSkill(0)->SetCooldown(10);

			if(pUnit->AddStatusEffect(citrus))
			{
				((CSkilledUnitObject*)GetOwner())->AddExperience(15);
			}
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendStatusEffectPacket(pUnit, 15, 6, 0, 7, 0, 0, 0);
				}
			NW_MANAGER_END
		}
	}

	GetOwner()->ReleaseReference();
	CCursor::GetInstance()->OverrideClick(NULL);
 	CCursor::GetInstance()->ResetSprite();
}

void CSkill_Grapefruit_Citrus::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
	CSkill_Grapefruit_Citrus_Click* newFunctor =  new CSkill_Grapefruit_Citrus_Click(GetOwner());
 	CCursor::GetInstance()->OverrideClick(newFunctor);
	GetOwner()->AcquireReference();
}

bool BeginForbidden(CUnitObject* unit)
{
	unit->ScaleStats(2);
	return true;
}

bool EndForbidden(CUnitObject* unit)
{
	unit->ApplyDamage(unit->GetMaxHealth());
	return true;
}

void CSkill_Grapefruit_ForbiddenFruit_Click::operator()()
{
	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(CCursor::GetInstance()->GetHoveredOverObject());

	if(GetOwner()->CheckIsDead())
	{
		GetOwner()->ReleaseReference();
		CCursor::GetInstance()->OverrideClick(NULL);
 		CCursor::GetInstance()->ResetSprite();
		return;
	}

	if(pUnit)
	{
		if(pUnit->GetTeamValue() == GetOwner()->GetTeamValue())
		{
			//Check dist

			CStatusEffect* forbidden = new CStatusEffect();
			forbidden->SetTimer(15);
			forbidden->Begin = BeginForbidden;
			forbidden->End = EndForbidden;

			if(pUnit->AddStatusEffect(forbidden))
			{
				GetOwner()->ApplyDamage(GetOwner()->GetMaxHealth());
			}

			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendStatusEffectPacket(pUnit, 15, 5, 0, 0, 0, 0, 0);
				}
			NW_MANAGER_END
		}
	}

	GetOwner()->ReleaseReference();
	CCursor::GetInstance()->OverrideClick(NULL);
 	CCursor::GetInstance()->ResetSprite();
}


void CSkill_Grapefruit_ForbiddenFruit::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
	CSkill_Grapefruit_ForbiddenFruit_Click* newFunctor =  new CSkill_Grapefruit_ForbiddenFruit_Click(GetOwner());
 	CCursor::GetInstance()->OverrideClick(newFunctor);
	GetOwner()->AcquireReference();
}

//////////////////////////////////////////////////////////
//						Banana
/////////////////////////////////////////////////////////

void CSkill_Banana_Split::operator()()
{
	CSkilledUnitObject* newUnit = NULL;
	if(newUnit = dynamic_cast<CSkilledUnitObject*>(GetOwner()))
	{
		if(newUnit->GetLevel() >= 5)
		{
			for(int i = 0; i < 2; i++)
			{
				CUnitObject* _newBanana = (CUnitObject*)CObjectFactory::GetInstance()->Construct("Banana", GetOwner()->GetObjectManagerOwner());
				_newBanana->SetPosition(GetOwner()->GetPositionX() - ((i*64)-32), GetOwner()->GetPositionY());
				_newBanana->SetTeamValue(GetOwner()->GetTeamValue());
				if(GetOwner()->GetNWOwner())
				{
					NW_MANAGER_START
						if(CNetworkManager::GetInstance()->IsConnected())
						{
							CNetworkManager::GetInstance()->SendCreateUnitPacket(_newBanana, "Banana");
						}
					NW_MANAGER_END
				}
			}

			GetOwner()->ApplyDamage(GetOwner()->GetHealth()+1000);
			GetOwner()->SetHealth(0);
		}
	}
}

void CSkill_Banana_Peel::operator()()
{
	Disable();
	GetOwner()->AddMovementMultiply(0.2f);
	GetOwner()->AddDefenseMod(-0.2f);

	NW_MANAGER_START
		if(CNetworkManager::GetInstance()->IsConnected())
		{
			CNetworkManager::GetInstance()->SendUseSimpleSkillPacket(GetOwner(), 0);
		}
	NW_MANAGER_END
}

void CSkill_Banana_Potassium::operator()()
{
	CCursor::GetInstance()->SetSprite(GetIcon());
	CSkill_Banana_Potassium_Click* newFunctor =  new CSkill_Banana_Potassium_Click(GetOwner());
 	CCursor::GetInstance()->OverrideClick(newFunctor);
	GetOwner()->AcquireReference();

	SetCooldown(20);
}

bool BeginPotassium(CUnitObject* unit, CStatusEffect* effect)
{
	//Apply particle effect
	effect->SetParticleEmitter(CParticleManager::GetInstance()->GetParticleEffect("Poisoned"));
	unit->AddParticleEmitterOver(effect->GetParticleEmitter());
	effect->GetParticleEmitter()->SetObjectToTrack(unit);
	effect->GetParticleEmitter()->SetOffset(D3DXVECTOR2(-24.0f, 0.0f));
	effect->GetParticleEmitter()->InitParticles();
	effect->GetParticleEmitter()->Play();
	return true;
}

bool EndPotassium(CUnitObject* unit, CStatusEffect* effect)
{
	//Remove Particle effect
	unit->RemoveParticleEmitterOver(effect->GetParticleEmitter());
	delete effect->GetParticleEmitter();
	return true;
}

bool UpdatePotassium(CUnitObject* unit, CStatusEffect* effect)
{
	CDamageOverTimeEffect* dot = NULL;
	if(dot = dynamic_cast<CDamageOverTimeEffect*>(effect))
	{
		if(dot->GetStartTime() + dot->GetInterval() < GetTickCount())
		{
			unit->ApplyDamage(dot->GetDamage());
			dot->SetStartTime(dot->GetStartTime() + dot->GetInterval());
		}
	}
	return true;
}

void CSkill_Banana_Potassium_Click::operator()()
{
	CUnitObject* pUnit = dynamic_cast<CUnitObject*>(CCursor::GetInstance()->GetHoveredOverObject());

	if(GetOwner()->CheckIsDead())
	{
		GetOwner()->ReleaseReference();
		CCursor::GetInstance()->OverrideClick(NULL);
 		CCursor::GetInstance()->ResetSprite();
		return;
	}

	if(pUnit)
	{
		if(pUnit->GetTeamValue() != GetOwner()->GetTeamValue())
		{
			//Check dist

			CDamageOverTimeEffect* potassium = new CDamageOverTimeEffect();
			potassium->SetDamage(7);
			potassium->SetInterval(4);
			potassium->SetTimer(13);
			potassium->BeginSender = BeginPotassium;
			potassium->EndSender = EndPotassium;
			potassium->UpdateSender = UpdatePotassium;

			if(pUnit->AddStatusEffect(potassium))
			{
				((CSkilledUnitObject*)GetOwner())->AddExperience(50);
			}

			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendStatusEffectPacket(pUnit, 13, 0, 0, 0, 1, 0, 2);
				}
			NW_MANAGER_END
		}
	}
	
	//No cooldown on miss

	GetOwner()->ReleaseReference();
	CCursor::GetInstance()->OverrideClick(NULL);
 	CCursor::GetInstance()->ResetSprite();
 	delete this;
}
