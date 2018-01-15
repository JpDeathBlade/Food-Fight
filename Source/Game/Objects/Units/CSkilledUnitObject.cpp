//////////////////////////////////////////////////////
// File: "CSkilledUnitObject.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: 9/17/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CSkilledUnitObject.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include "../../../Managers/Bad Chef/Networking/NetworkUpdateClasses.h"
#include "../../../Managers/Bad Chef/Audio/CAudioManager.h"
#include "../GUI Objects/CCursor.h"
#include "../../Unit Commands/IUnitAICommand.h"
#include "../../Unit Commands/CMoveCommand.h"
#include "../GUI Objects/CFloatingText.h"

#include "IAbility.h"

#define LAYER_UNIT 1

CSkilledUnitObject::CSkilledUnitObject(void)
{
	m_usExperience	= 0;
	m_ucLevel		= 1;	
}

void CSkilledUnitObject::Update(float Delta)
{
	CUnitObject::Update(Delta);

	//If theres no Command then the unit is Idle
	if(!GetUnitAICommand())
		SetState(STATE_IDLE);

}

void CSkilledUnitObject::Draw(void)
{
	CUnitObject::Draw();
}

void CSkilledUnitObject::OnHover(void)
{
	CUnitObject::OnHover();
}

void CSkilledUnitObject::ContextSensitiveClick(void)
{
	CUnitObject::ContextSensitiveClick();

	// If camera - pos = positive
	if(CCursor::GetInstance()->GetPosition().x < GetPositionX())
		SetDirection(DIRECTION_LEFT);
	else
		SetDirection(DIRECTION_RIGHT);
}

int CSkilledUnitObject::GetExpForNextLevel()
{
	return (int)(14 * pow((float)(m_ucLevel + 1), 2.0f) + (10 * (m_ucLevel + 1)) - 4);
}

void CSkilledUnitObject::AddExperience(int amount)
{
	if(this->GetNWOwner())
	{
		m_usExperience += amount;

		if(m_ucLevel < SKILLEDUNIT_MAXLEVEL && GetExpForNextLevel() <= m_usExperience)
			LevelUp();
	}
}

void CSkilledUnitObject::LevelUp()
{
	if(m_usExperience < GetExpForNextLevel())
		m_usExperience = GetExpForNextLevel();
	m_ucLevel++;

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BUG-020
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	CFloatingText::SpawnFloatingText(this->GetObjectManagerOwner(), "Level Up!", GetPosition(), D3DXVECTOR2(0.0f, -50.0f), 3.0f, 0.8f, 0.0f);

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BUG-020
	//////////////////////////////////////////////////////////////////////////


	// Temp Level Up =3
	SetDefense((unsigned short)(GetDefense() * 1.15f));
	SetAttack((unsigned short)(GetAttack() * 1.15f));
	SetEvade(GetEvade() + 1.0f);

	SetMaxHealth((unsigned short)(GetMaxHealth() * 1.15f));
	SetDefense((unsigned short)(GetDefense() *1.15f));

	if(this->GetNWOwner())
	{
		NW_MANAGER_START
			if(CNetworkManager::GetInstance()->IsConnected())
			{
				CNetworkManager::GetInstance()->SendUnitStatUpdatePacket(this);
			}
		NW_MANAGER_END
	}
}

IObject* CreateToast(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/Toast.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_IDLE);
	pNewUnit->SetDirection(pNewUnit->DIRECTION_RIGHT);

	pNewUnit->SetMaxHealth(100);
	pNewUnit->SetHealth(100);
	pNewUnit->SetAttack(10);
	pNewUnit->SetAttackSpeed(2.0f);
	pNewUnit->SetDefense(15);
	pNewUnit->SetEvade(10.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(80.0f);
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_WHEAT);

	pNewUnit->SetSkill(0,new CSkill_Toast_Crust(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Toast_Yeast(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Toast_SlicedBread(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/ByTheCrumb.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateSunnySideUpEgg(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/SunnySideUp.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_IDLE);
	pNewUnit->SetDirection(pNewUnit->DIRECTION_RIGHT);

	pNewUnit->SetMaxHealth(60);
	pNewUnit->SetHealth(80);
	pNewUnit->SetAttack(8);
	pNewUnit->SetAttackSpeed(2.0f);
	pNewUnit->SetDefense(9);
	pNewUnit->SetEvade(10.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(300.0f);
	pNewUnit->SetBullet("BaconArrow");
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_EGG);

	CGameObject* m_pFace		= new CGameObject();
	m_pFace->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/FaceMini.spr"));
	pNewUnit->SetFace(m_pFace);

	pNewUnit->SetSkill(0,new CSkill_Sunny_Benedict(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Sunny_Caviar(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Sunny_OverEasy(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/Maggot.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateScrambledEgg(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/Scambled.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_IDLE);
	pNewUnit->SetDirection(pNewUnit->DIRECTION_RIGHT);

	pNewUnit->SetMaxHealth(100);
	pNewUnit->SetHealth(100);
	pNewUnit->SetAttack(15);
	pNewUnit->SetAttackSpeed(1.8f);
	pNewUnit->SetDefense(15);
	pNewUnit->SetEvade(15.0f);
	pNewUnit->SetMovementSpeed(100);
	pNewUnit->SetRange(200.0f);
	pNewUnit->SetBullet("BaconArrow");
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_EGG);

	CGameObject* m_pFace		= new CGameObject();
	m_pFace->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/FaceMini.spr"));
	pNewUnit->SetFace(m_pFace);

	pNewUnit->SetSkill(0,new CSkill_Scrambled_BeatIt(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Scrambled_Raw(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Scrambled_Easter(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/BeatIt.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateSausage(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/Sausage.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_IDLE);

	pNewUnit->SetMaxHealth(100);
	pNewUnit->SetHealth(100);
	pNewUnit->SetAttack(25);
	pNewUnit->SetAttackSpeed(2.5f);
	pNewUnit->SetDefense(10);
	pNewUnit->SetEvade(10.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(80.0f);
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_MEAT);

	CGameObject* m_pFace		= new CGameObject();
	m_pFace->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/FaceMini.spr"));
	pNewUnit->SetFace(m_pFace);

	pNewUnit->SetSkill(0,new CSkill_Sausage_Kielbasa(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Sausage_Bratwurste(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Sausage_SPAM(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/SausageFest.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateHam(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/Ham.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_IDLE);

	pNewUnit->SetMaxHealth(120);
	pNewUnit->SetHealth(120);
	pNewUnit->SetAttack(30);
	pNewUnit->SetAttackSpeed(2.5f);
	pNewUnit->SetDefense(10);
	pNewUnit->SetEvade(5.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(80.0f);
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_MEAT);

	CGameObject* m_pFace		= new CGameObject();
	m_pFace->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/FaceMini.spr"));
	pNewUnit->SetFace(m_pFace);

	pNewUnit->SetSkill(0,new CSkill_Ham_HoneyRoasted(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Ham_Butcher(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Ham_RumpRoast(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/Kosher.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreatePancake(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/Pancake.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_NONE);

	pNewUnit->SetMaxHealth(60);
	pNewUnit->SetHealth(60);
	pNewUnit->SetAttack(10);
	pNewUnit->SetAttackSpeed(2.0f);
	pNewUnit->SetDefense(10);
	pNewUnit->SetEvade(5.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(200.0f);
	pNewUnit->SetBullet("BaconArrow");
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_WHEAT);

	CGameObject* m_pFace		= new CGameObject();
	m_pFace->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/FaceMini.spr"));
	pNewUnit->SetFace(m_pFace);

	pNewUnit->SetSkill(0,new CSkill_Pancake_Buttermilk(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Pancake_Flappjack(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Pancake_Topping(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/BatterUp.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateWaffle(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/Waffle.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_NONE);

	pNewUnit->SetMaxHealth(200);
	pNewUnit->SetHealth(200);
	pNewUnit->SetAttack(30);
	pNewUnit->SetAttackSpeed(3.0f);
	pNewUnit->SetDefense(20);
	pNewUnit->SetEvade(10.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(200.0f);
	pNewUnit->SetBullet("BaconArrow");
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_WHEAT);

	pNewUnit->SetSkill(0,new CSkill_Waffle_BelgianSurprise(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Waffle_Sap(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Waffle_FlavoredSyrup(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/ROFLWaffle.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateBluberryMuffin(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/BlueBerryMuffin.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_NONE);

	pNewUnit->SetMaxHealth(200);
	pNewUnit->SetHealth(200);
	pNewUnit->SetAttack(25);
	pNewUnit->SetAttackSpeed(3.0f);
	pNewUnit->SetDefense(25);
	pNewUnit->SetEvade(5.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(80.0f);
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_FRUIT);

	pNewUnit->SetSkill(0,new CSkill_Blueberry_Scone(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Blueberry_Shrapnel(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Blueberry_BlueAndSweet(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/Almost.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateFrenchToast(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/FrenchToast.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_NONE);

	pNewUnit->SetMaxHealth(100);
	pNewUnit->SetHealth(100);
	pNewUnit->SetAttack(15);
	pNewUnit->SetAttackSpeed(2.0f);
	pNewUnit->SetDefense(20);
	pNewUnit->SetEvade(15.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(80.0f);
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_EGG);

	CGameObject* m_pFace		= new CGameObject();
	m_pFace->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/FaceFrench.spr"));
	pNewUnit->SetFace(m_pFace);

	pNewUnit->SetSkill(0,new CSkill_FrenchToast_PowderedSugar(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_FrenchToast_Baguette(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_FrenchToast_EggWash(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/ItIsLife.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateGrapefruit(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/GrapeFruit.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_NONE);

	pNewUnit->SetMaxHealth(60);
	pNewUnit->SetHealth(60);
	pNewUnit->SetAttack(12);
	pNewUnit->SetAttackSpeed(3.0f);
	pNewUnit->SetDefense(10);
	pNewUnit->SetEvade(4.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(150.0f);
	pNewUnit->SetBullet("BaconArrow");
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_FRUIT);

	pNewUnit->SetSkill(0,new CSkill_Grapefruit_Citrus(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Grapefruit_BarrelRoll(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Grapefruit_ForbiddenFruit(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/BarrelRoll.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateBanana(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/Banana.spr");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_NONE);

	pNewUnit->SetMaxHealth(80);
	pNewUnit->SetHealth(80);
	pNewUnit->SetAttack(18);
	pNewUnit->SetAttackSpeed(3.5f);
	pNewUnit->SetDefense(13);
	pNewUnit->SetEvade(5.0f);
	pNewUnit->SetMovementSpeed(150);
	pNewUnit->SetRange(80.0f);
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_FRUIT);

	CGameObject* m_pArm		= new CGameObject();
	m_pArm->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/LegsBanana.spr"));
	pNewUnit->SetArmRight(m_pArm);

	CGameObject* m_pArmL		= new CGameObject();
	m_pArmL->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/LegsBanana.spr"));
	pNewUnit->SetArmLeft(m_pArmL);

	CGameObject* m_pFace		= new CGameObject();
	m_pFace->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/LegsBanana.spr"));
	pNewUnit->SetFace(m_pFace);

	CGameObject* m_pLeg		= new CGameObject();
	m_pLeg->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/LegsBanana.spr"));
	pNewUnit->SetLegs(m_pLeg);

	pNewUnit->SetSkill(0,new CSkill_Banana_Peel(pNewUnit));
	pNewUnit->SetSkill(1,new CSkill_Banana_Potassium(pNewUnit));
	pNewUnit->SetSkill(2,new CSkill_Banana_Split(pNewUnit));

	pNewUnit->SetVoice(CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/Hiss.wav", "Voices"));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

IObject* CreateStrawberries(CObjectManager* pManager)
{
	CSkilledUnitObject* pNewUnit = new CSkilledUnitObject();

	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/FF_Unit_Strawberry.PNG");
	if(!Sprite)
		return NULL;

	pNewUnit->SetSprite(Sprite);
	pNewUnit->SetAnimationType(pNewUnit->PLAY_LOOPING);
	pNewUnit->SetState(pNewUnit->STATE_NONE);

	pNewUnit->SetMaxHealth(1000);
	pNewUnit->SetHealth(1000);
	pNewUnit->SetAttack(100);
	pNewUnit->SetAttackSpeed(1.0f);
	pNewUnit->SetDefense(50);
	pNewUnit->SetEvade(50.0f);
	pNewUnit->SetMovementSpeed(200);
	pNewUnit->SetRange(300.0f);
	pNewUnit->SetUnitAICommand(NULL);
	pNewUnit->SetBaseType(CUnitObject::BASE_FRUIT);

	pNewUnit->SetSkill(1, new CSkill_GOD_Nuke(pNewUnit));

	pManager->AddObjectToLayer(LAYER_UNIT, pNewUnit);

	return pNewUnit;
}

int CSkilledUnitObject::Attack(IAttackable* _pTarget)
{
	int damDone = CUnitObject::Attack(_pTarget);

	if(damDone > 0)
	{
		// Add experience if you can
		CSkilledUnitObject* pUnitEnemy = dynamic_cast<CSkilledUnitObject*>(_pTarget);
		if(pUnitEnemy)
			AddExperience(pUnitEnemy->GetExperienceValue() + damDone);

		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BUG-012
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		//Get the most updated attack timer
		//SetAttackTimer(GetTickCount() + GetAttackSpeed());

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BUG-012
		//////////////////////////////////////////////////////////////////////////
	}

	return damDone;
}