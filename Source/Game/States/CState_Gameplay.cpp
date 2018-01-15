//////////////////////////////////////////////////////
// File: "CState_Gameplay.cpp"
// Creator: MG
// Creation Date: 9/28/09
// Last Modified: 9/28/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#include "CState_Gameplay.h"
#include "CState_RecipeBook.h"
#include "CState_MainMenu.h"
#include "CState_Pause.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/CObjectFactory.h"
#include "../../Game/Objects/Units/CUnitObject.h"
#include "../../Game/Objects/GUI Objects/CButton.h"
#include "../../Game/Objects/Base/CCamera.h"
#include "../../Game/Cooking Mini-Game/CCookingResource.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include "../../Game/Objects/Tiles/CTileStage.h"
#include "../../Game/Objects/Path Nodes/CPathNode.h"
#include "../../Game/Objects/Appliances/CStove.h"
#include "../../Game/Objects/Appliances/CPlate.h"
#include "../../Game/Objects/Appliances/CCoffeeMaker.h"
#include "../../Game/Objects/Appliances/CToaster.h"
#include "../../Game/Objects/GUI Objects/CSelectionHUD.h"
#include "../../Game/Objects/GUI Objects/CActionsHUD.h"
#include "../../Game/Objects/GUI Objects/CUnitDataHUD.h"
#include "../../Game/Objects/GUI Objects/CStoveHUD.h"
#include "../../Game/Objects/GUI Objects/CMiniMap.h"
#include "../../Game/Cooking Mini-Game/CPan.h"
#include "../../Game/Cooking Mini-Game/CCookingUnit.h"
#include "../../Game/Cooking Mini-Game/CSpiceHolder.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Game/Objects/Units/CSkilledUnitObject.h"
#include "../../Game/Objects/Units/CProjectile.h"
#include "../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include "../../Managers/Bad Chef/Networking/NetworkUpdateClasses.h"
#include "../../Managers/Bad Chef/Networking/INetworkable.h"
#include "../../Game/Unit Commands/CMoveCommand.h"
#include "../../Game/Objects/Path Nodes/CPathNode.h"
#include "../../Game/Unit Commands/CAttackCommand.h"
#include "../../Game/Unit Commands/CAttackMoveCommand.h"
#include "../../Managers/Bad Chef/Particles/CParticleManager.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"

struct SFindUnit
{
	unsigned int id;
	CUnitObject* retObj;
};

struct SProcAttack
{
	CUnitObject* atkUnit;
	unsigned int id;
};

void CFocusLostFunc::operator()()
{
	m_pGameplay->SetPaused(true);
	GetSender()->PushState(new CState_Pause);
}

bool FindUnitWithID(IObject* obj, CUpdateUnitPacket* packet);
bool SendUpdatePackets(IObject* obj, void* _ptr);
bool FindUnit(IObject* obj, SFindUnit* _find);
bool ProcMoveCommand(IObject* obj, CMoveCommandPacket* packet);
bool ProcAttackCommand(IObject* obj, SProcAttack* _atkUnit);
bool ProcDamage(IObject* obj, CDealDamagePacket* packet);
bool ProcDropResource(IObject* obj, CDropResourcePacket* packet);
bool ProcUnitStats(IObject* obj, CUpdateUnitStatsPacket* packet);
bool ProcSimpleSkill(IObject* obj, CUseSimpleSkillPacket* packet);
bool ProcStatusEffect(IObject* obj, CSendStatusEffectPacket* packet);
bool ProcCreateProjectile(IObject* obj, CCreateProjectilePacket* packet);

CState_Gameplay::CState_Gameplay(int _playernum, bool _networked, std::string mapFile)
{
	m_bNetworked = _networked;
	m_NetworkUpdateTimer = 3;

	m_bPaused = false;

	m_FocusLost.SetGameplay(this);

	CGame::GetInstance()->m_FocusLost += &m_FocusLost;

	// Load sprites.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Appliances/Plate.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/SpatulaCursor.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/EmptySpoon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/SugarSpoon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/CinnamonSpoon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/EmptyPortrait.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/SmokeDetector.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/MoveIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/AttackIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/StopIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/Stove.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/ResourcePlate.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/ResourceFruit.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/ResourceMeat.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/ResourceWheat.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/ResourceEgg.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/Pan.spr");	

	SpriteManager->LoadSpriteFromSprite("Resources/Data/Toast.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/SunnySideUp.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Scambled.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Sausage.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Ham.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Pancake.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Waffle.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/BlueBerryMuffin.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/FrenchToast.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/GrapeFruit.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Banana.spr");

	SpriteManager->LoadSpriteFromSprite("Resources/Data/Arms.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Legs.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/LegsBanana.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Face.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/FaceMini.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/FaceFrench.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Units/SelectionCircle.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Units/SubSelectionCircle.spr");

	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/MiniMap.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/WhiteShaker.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/BlackShaker.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Status Effects/OverspiceStatus.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Status Effects/FastStatus.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Status Effects/1.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Status Effects/2.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Status Effects/3.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Appliances/Toaster.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Appliances/CoffeeMaker.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Projectiles/BlueberryBomb.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Projectiles/BaconArrow.spr");

	//Skill Icons
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/CrustIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/YeastIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/OverEasyIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/EasterIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/SlicedBreadIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/CaviarIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/BenedictIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/AmberIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/BlueAndSweetIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/ButtermilkIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/FlapjackIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/PeelIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/PotassiumIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/SapIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/SconeIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/ShrapnelIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/SplitIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/SurpriseIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/ToppingIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/BagetteIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/BarrelRollIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/BratIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/ButcherIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/CitrusIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/EggWashIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/FlagIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/FloatingEggIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/ForbiddenFruitIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/HoneyRoastedIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/KBasa.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/PowderedSugarIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/RumpRoastIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Skills/SpamIcon.spr");

	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Appliances/PlateOutline.spr");

	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Banana.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_BlueBerryMuffin.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_FrenchToast.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_GrapeFruit.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Ham.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Pancake.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Sausage.PNG");
	//SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_ScrambledEgg.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Strawberry.PNG");
	//SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_SunnySideUpEgg.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Waffle.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Graphics/User Interface/ff_CaptureBar.png");

	CParticleManager::GetInstance()->LoadParticleEffect("Resources/Graphics/Particles/Smoke.FFP");
	CParticleManager::GetInstance()->LoadParticleEffect("Resources/Graphics/Particles/Combo.FFP");
	CParticleManager::GetInstance()->LoadParticleEffect("Resources/Graphics/Particles/Poisoned.FFP");
	CParticleManager::GetInstance()->LoadParticleEffect("Resources/Graphics/Particles/BlueBerryExplosion.FFP");
	CParticleManager::GetInstance()->LoadParticleEffect("Resources/Graphics/Particles/Heal.FFP");


	// Register factory methods with object factory.
	m_ObjectFactory = CObjectFactory::GetInstance();
	m_ObjectFactory->AddMethod("SelectionBar", &CSelectionHUD::CreateSelectionHUD);
	m_ObjectFactory->AddMethod("ActionsBar", &CActionsHUD::CreateActionsHUD);
	m_ObjectFactory->AddMethod("UnitData", &CUnitDataHUD::CreateUnitDataHUD);
	m_ObjectFactory->AddMethod("StoveHUD", &CStoveHUD::CreateStoveHUD);
	m_ObjectFactory->AddMethod("MiniMap", &CMiniMap::CreateMiniMap);
	m_ObjectFactory->AddMethod("Stove", &CreateStoveFactoryMethod);
	m_ObjectFactory->AddMethod("ResourcePlate", &CreateResourcePlateFactoryMethod);
	m_ObjectFactory->AddMethod("ResourceEgg", &CreateEggFactoryMethod);
	m_ObjectFactory->AddMethod("ResourceMeat", &CreateMeatFactoryMethod);
	m_ObjectFactory->AddMethod("ResourceWheat", &CreateWheatFactoryMethod);
	m_ObjectFactory->AddMethod("ResourceFruit", &CreateFruitFactoryMethod);
	m_ObjectFactory->AddMethod("Pan", &CreatePanFactoryMethod);	
	m_ObjectFactory->AddMethod("Toast", &CreateToast);
	m_ObjectFactory->AddMethod("Sunny Side Up Egg", &CreateSunnySideUpEgg);
	m_ObjectFactory->AddMethod("Scrambled Eggs", &CreateScrambledEgg);
	m_ObjectFactory->AddMethod("Sausage", &CreateSausage);
	m_ObjectFactory->AddMethod("Ham", &CreateHam);
	m_ObjectFactory->AddMethod("Pancake", &CreatePancake);
	m_ObjectFactory->AddMethod("Waffle", &CreateWaffle);
	m_ObjectFactory->AddMethod("Blueberry Muffin", &CreateBluberryMuffin);
	m_ObjectFactory->AddMethod("French Toast", &CreateFrenchToast);
	m_ObjectFactory->AddMethod("Grapefruit", &CreateGrapefruit);
	m_ObjectFactory->AddMethod("Banana", &CreateBanana);
	m_ObjectFactory->AddMethod("Strawberries", &CreateStrawberries);
	m_ObjectFactory->AddMethod("Plate", &CreatePlateFactoryMethod);
	m_ObjectFactory->AddMethod("CoffeeMaker", &CreateCoffeeMakerFactoryMethod);
	m_ObjectFactory->AddMethod("CookingUnit", &CreateCookingUnitFactoryMethod);
	m_ObjectFactory->AddMethod("SaltHolder", &CreateSaltHolderFactoryMethod);
	m_ObjectFactory->AddMethod("PepperHolder", &CreatePepperHolderFactoryMethod);
	m_ObjectFactory->AddMethod("SugarHolder", &CreateSugarHolderFactoryMethod);
	m_ObjectFactory->AddMethod("CinnaHolder", &CreateCinnaHolderFactoryMethod);
	m_ObjectFactory->AddMethod("Salt", &CreateSaltFactoryMethod);
	m_ObjectFactory->AddMethod("Pepper", &CreatePepperFactoryMethod);
	m_ObjectFactory->AddMethod("Sugar", &CreateSugarFactoryMethod);
	m_ObjectFactory->AddMethod("Cinna", &CreateCinnaFactoryMethod);
	m_ObjectFactory->AddMethod("Toaster", &CreateToasterFactoryMethod);
	m_ObjectFactory->AddMethod("BlueberryShrapnel", &CreateBlueberryShrapnel);
	m_ObjectFactory->AddMethod("BaconArrow", &CreateBaconArrow);

	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Music/FunkCountry.mp3", "Music", false, true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Cooking Mini-Game/Fire.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Cooking Mini-Game/LightSizzling.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Cooking Mini-Game/SizzleA.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Cooking Mini-Game/SizzleB.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Cooking Mini-Game/TimerDing.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Units/ExplosionA.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Units/Crunch.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Units/SharpenKnife.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Units/EggBeater.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Units/Chimes.wav", "Effects", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/User Interface/PageFlip.wav", "User Interface");
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/User Interface/SmokeDetector.wav", "User Interface");
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/User Interface/EnemyCaptured.wav", "User Interface");
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/User Interface/PlayerCapture.wav", "User Interface");
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/User Interface/PlayerPlateLost.wav", "User Interface");
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/User Interface/EnemyPlateLost.wav", "User Interface");
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/BatterUp.wav", "Voices", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/BeatIt.wav", "Voices", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/ByTheCrumb.wav", "Voices", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/Hiss.wav", "Voices", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/ItIsLife.wav", "Voices", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/Kosher.wav", "Voices", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/ROFLWaffle.wav", "Voices", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/SausageFest.wav", "Voices", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/Almost.wav", "Voices", true);
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/Maggot.wav", "Voices", true);

	// Create objects.
	m_ObjectManager.Initialize(5);
	m_ObjectManager.SetLayerCoordinateSpace(4, CObjectLayer::ECoordinateSpace::CS_Screen);
	
	CTileStage::GetInstance()->InitMap(mapFile.c_str(), &m_ObjectManager, _playernum);

	/*for(int i = 0; i < 3; i++)
	{
		Stove->AddCookingResource(RESOURCE_WHEAT, &m_ObjectManager);
	}*/

	//Stove->AddCookingResource(RESOURCE_FRUIT, &m_ObjectManager);
	//Stove->AddCookingResource(RESOURCE_MEAT, &m_ObjectManager);

	// Create GUI.
	m_ObjectFactory->Construct("MiniMap", &m_ObjectManager);
	m_ObjectFactory->Construct("SelectionBar", &m_ObjectManager);
	m_ObjectFactory->Construct("ActionsBar", &m_ObjectManager);
	m_ObjectFactory->Construct("UnitData", &m_ObjectManager);
	m_ObjectFactory->Construct("StoveHUD", &m_ObjectManager);

	std::vector<CStove*>& Stoves = CTileStage::GetInstance()->GetStoves();
	for(unsigned char i = 0; i < Stoves.size(); ++i)
	{
		if(Stoves[i]->GetTeamValue() == TEAM_PLAYER)
		{
			if(!m_bNetworked)
			{
				m_EnemyAI.SetEnemyStove(Stoves[i]);
			}
			CStoveHUD::GetInstance()->SetTargetStove(Stoves[i]);

			CCookingResource* CookingResource = Stoves[i]->GetWheat();
			if(CookingResource)
			{
				CookingResource->SetUsed(true);
				Stoves[i]->GetBottomLeftPan()->AddResource(CookingResource);
				Stoves[i]->SubtractCookingResource(CookingResource->GetResourceType());
			}

			CookingResource = Stoves[i]->GetWheat();
			if(CookingResource)
			{
				CookingResource->SetUsed(true);
				Stoves[i]->GetTopRightPan()->AddResource(CookingResource);
				Stoves[i]->SubtractCookingResource(CookingResource->GetResourceType());
			}
			continue;
		}
		if(Stoves[i]->GetTeamValue() == TEAM_ENEMY)
		{
			if(!m_bNetworked)
			{
				m_EnemyAI.SetFriendlyStove(Stoves[i]);
			}
			continue;
		}
	}
	if(!m_bNetworked)
	{
		std::vector<CPlate*>& Plates = CTileStage::GetInstance()->GetPlates();
		for(unsigned char i = 0; i < Plates.size(); ++i)
		{
			m_EnemyAI.AddPlate(Plates[i]);
		}
		m_EnemyAI.Initialize();
	}	

	//CSkilledUnitObject* unitToast;
	//for(int i = 0; i < 2; i++)
	//{
	//	unitToast = static_cast<CSkilledUnitObject*>(m_ObjectFactory->Construct("Toast", &m_ObjectManager));
	//	unitToast->SetPosition(800 + (i*65), 150);
	//	unitToast->SetTeamValue(ETeamValue::TEAM_PLAYER);
	//}
	//if(!m_bNetworked)
	//{
	//	for(int i = 0; i < 2; i++)
	//	{
	//		unitToast = static_cast<CSkilledUnitObject*>(m_ObjectFactory->Construct("Toast", &m_ObjectManager));
	//		unitToast->SetPosition(1500 + i, 150);
	//		unitToast->SetTeamValue(ETeamValue::TEAM_ENEMY);
	//		m_EnemyAI.OwnUnit(unitToast);
	//	}
	//}
}

CState_Gameplay::~CState_Gameplay()
{
	CGame::GetInstance()->m_FocusLost -= &m_FocusLost;
	CAMERA->DeleteInstance();

	// Delete objects.
	m_ObjectManager.RemoveAllObjects();

	CTileStage::DeleteInstance();

	CPathNode::ClearPathing();

	// Unregister factory methods from object factory.
	m_ObjectFactory->RemoveMethod("Egg");
	m_ObjectFactory->RemoveMethod("Cursor");
	m_ObjectFactory->RemoveMethod("Button");
	m_ObjectFactory->RemoveMethod("ExampleUnit");
	m_ObjectFactory->RemoveMethod("Stove");

	m_ObjectFactory->RemoveMethod("Toast");
	m_ObjectFactory->RemoveMethod("SunnySideUpEgg");
	m_ObjectFactory->RemoveMethod("ScrambledEgg");
	m_ObjectFactory->RemoveMethod("Sausage");
	m_ObjectFactory->RemoveMethod("Ham");
	m_ObjectFactory->RemoveMethod("Pancake");
	m_ObjectFactory->RemoveMethod("Waffle");
	m_ObjectFactory->RemoveMethod("BluberryMuffin");
	m_ObjectFactory->RemoveMethod("FrenchToast");
	m_ObjectFactory->RemoveMethod("Grapefruit");
	m_ObjectFactory->RemoveMethod("Banana");
	m_ObjectFactory->RemoveMethod("Strawberries");
	m_ObjectFactory->RemoveMethod("BlueberryShrapnel");
	m_ObjectFactory->RemoveMethod("BaconArrow");

	CParticleManager::GetInstance()->ShutdownParticleManger();

	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Cooking Mini-Game/Fire.wav", "Effects");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Cooking Mini-Game/LightSizzling.wav", "Effects");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Cooking Mini-Game/SizzleA.wav", "Effects");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Cooking Mini-Game/SizzleB.wav", "Effects");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Cooking Mini-Game/TimerDing.wav", "Effects");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Units/ExplosionA.wav", "Effects");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Units/Crunch.wav", "Effects");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Units/SharpenKnife.wav", "Effects");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/User Interface/PageFlip.wav", "User Interface");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/User Interface/SmokeDetector.wav", "User Interface");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/User Interface/EnemyCaptured.wav", "User Interface");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/User Interface/PlayerCapture.wav", "User Interface");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/User Interface/PlayerPlateLost.wav", "User Interface");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/User Interface/EnemyPlateLost.wav", "User Interface");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/BatterUp.wav", "Voices");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/BeatIt.wav", "Voices");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/ByTheCrumb.wav", "Voices");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/Hiss.wav", "Voices");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/ItIsLife.wav", "Voices");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/Kosher.wav", "Voices");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/ROFLWaffle.wav", "Voices");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/SausageFest.wav", "Voices");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/Almost.wav", "Voices");
	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/Maggot.wav", "Voices");

	// Unload sprites.
	//BITMAP->ShutdownTextManager();
	//CSpriteManager::GetInstance()->UnloadAllSprites();

	//We should unload every sprite we init in this state

	CSelectionHUD::DeleteInstance();

	NW_MANAGER_START
		if(CNetworkManager::GetInstance()->IsConnected())
		{
			CNetworkManager::GetInstance()->SendDisconnectPacket();
			CNetworkManager::GetInstance()->SetConnectedIP(NULL);
		}
	NW_MANAGER_END
}

void CState_Gameplay::OnEnter()
{
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Music/ElevatorStyle.wav", "Music")->StopPlaying();
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Music/FunkCountry.mp3", "Music")->Play(-1);
	CCursor::GetInstance()->EnableDragSelection();
}

void CState_Gameplay::OnExit()
{
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Music/FunkCountry.mp3", "Music")->StopPlaying();
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Music/ElevatorStyle.wav", "Music")->Play(-1);
	CCursor::GetInstance()->DisableDragSelection();
}

bool CState_Gameplay::Input()
{
	if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[UP]) || XBOX->GetController(0)->IsDown_THUMB_RIGHT_Y() > 0)
		CAMERA->AccelerateY(-CAMERA->GetCameraSpeed());
	if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[DOWN]) || XBOX->GetController(0)->IsDown_THUMB_RIGHT_Y() < 0)
		CAMERA->AccelerateY(CAMERA->GetCameraSpeed());

	if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[LEFT]) || XBOX->GetController(0)->IsDown_THUMB_RIGHT_X() < 0)
		CAMERA->AccelerateX(-CAMERA->GetCameraSpeed());
	if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[RIGHT]) || XBOX->GetController(0)->IsDown_THUMB_RIGHT_X() > 0)
		CAMERA->AccelerateX(CAMERA->GetCameraSpeed());

	if((CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->KeyPressed(DIK_PAUSE)|| XBOX->GetController(0)->IsPressed_START()) && !m_bPaused)
	{
		m_bPaused = true;
		CGame::GetInstance()->PushState(new CState_Pause);
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[ATTACK]))
		CActionsHUD::GetInstance()->AttackButtonOnClick();

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[MOVE]))
		CActionsHUD::GetInstance()->MoveButtonOnClick();

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[STOP]))
		CActionsHUD::GetInstance()->StopButtonOnClick();

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[STOVE]))
		CStoveHUD::GetInstance()->GetSmokeDetectorButton()->OnClick();

	if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[COOKBOOK]) || XBOX->GetController(0)->IsPressed_Y())
		CGame::GetInstance()->PushState(new CState_RecipeBook);
	return true;
}

void CState_Gameplay::Update(float Delta)
{
	FMOD_VECTOR CameraPosition;
	CameraPosition.x = CAMERA->GetPositionX() + 512;
	CameraPosition.y = CAMERA->GetPositionY() + 384;
	CameraPosition.z = 0;
	CAudioManager::GetInstance()->GetFMODAPI()->set3DListenerAttributes(0, &CameraPosition, NULL, NULL, NULL);

	//Check for packets pertaining to this state
	CCreateUnitPacket* packet = NULL;
	CUpdateUnitPacket* packet2 = NULL;
	CMoveCommandPacket* packet3 = NULL;
	CAttackCommandPacket* packet4 = NULL;
	CDealDamagePacket* packet5 = NULL;
	CUpdateUnitStatsPacket* packet6 = NULL;
	CUseSimpleSkillPacket* packet7 = NULL;
	CSendStatusEffectPacket* packet8 = NULL;
	CCreateProjectilePacket* packet9 = NULL;
	CDisconnectPacket* packet10 = NULL;
	NW_MANAGER_START
		if(CNetworkManager::GetInstance()->IsConnected())
		{
			CNetworkManager::GetInstance()->Update(Delta);
			if(packet = static_cast<CCreateUnitPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_CREATEUNIT)))
			{
				CUnitObject* newUnit = static_cast<CUnitObject*>(m_ObjectFactory->Construct(packet->createString, &m_ObjectManager));
				newUnit->SetPosition(packet->posx, packet->posy);
				newUnit->SetNWOwner(false);
				newUnit->SetSharedID(packet->shareID);
				newUnit->SetTeamValue(TEAM_ENEMY);

				//Set unit stats
				newUnit->SetBaseAttack(packet->attack);
				newUnit->SetBaseDefense(packet->defense);
				newUnit->SetBaseAttackSpeed(packet->attackSpeed);
				newUnit->SetBaseMovement(packet->movementSpeed);
				newUnit->SetBaseEvade(packet->evade);
				newUnit->SetCreationString(packet->createString);

				delete packet;
			}
			while(packet2 = static_cast<CUpdateUnitPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_UPDATEUNIT)))
			{
				m_ObjectManager.Iterate<CUpdateUnitPacket>(&FindUnitWithID, packet2);
				delete packet2;
			}
			while(packet3 = static_cast<CMoveCommandPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_MOVECOMMAND)))
			{
				m_ObjectManager.Iterate<CMoveCommandPacket>(&ProcMoveCommand, packet3);
				delete packet3;
			}
			while(packet4 = static_cast<CAttackCommandPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_ATTACKCOMMAND)))
			{
				SFindUnit findUnit;
				findUnit.id = packet4->attackID;
				m_ObjectManager.Iterate<SFindUnit>(&FindUnit, &findUnit);
				SProcAttack procAtk;
				procAtk.atkUnit = findUnit.retObj;
				procAtk.id = packet4->shareID;
				m_ObjectManager.Iterate<SProcAttack>(&ProcAttackCommand, &procAtk);
				delete packet4;
			}
			while(packet5 = static_cast<CDealDamagePacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_DAMAGE)))
			{
				m_ObjectManager.Iterate<CDealDamagePacket>(&ProcDamage, packet5);
				delete packet5;
			}
			while(packet6 = static_cast<CUpdateUnitStatsPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_UPDATESTATS)))
			{
				m_ObjectManager.Iterate<CUpdateUnitStatsPacket>(&ProcUnitStats, packet6);
				delete packet6;
			}
			while(packet7 = static_cast<CUseSimpleSkillPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_USESIMPLESKILL)))
			{
				m_ObjectManager.Iterate<CUseSimpleSkillPacket>(&ProcSimpleSkill, packet7);
				delete packet7;
			}
			while(packet8 = static_cast<CSendStatusEffectPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_STATUSEFFECT)))
			{
				m_ObjectManager.Iterate<CSendStatusEffectPacket>(&ProcStatusEffect, packet8);
				delete packet8;
			}
			while(packet9 = static_cast<CCreateProjectilePacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_CREATEPROJECTILE)))
			{
				m_ObjectManager.Iterate<CCreateProjectilePacket>(&ProcCreateProjectile, packet9);
				delete packet9;
			}
			while(packet10 = static_cast<CDisconnectPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_DISCONNECT)))
			{
				//CGame::GetInstance()->ChangeState(new CState_MainMenu);
				delete packet10;
#if _DEBUG
				printf("Disconnect packet recieved\n");
#endif
			}

			m_NetworkUpdateTimer -= Delta;
			if(m_NetworkUpdateTimer < 0)
			{
				m_NetworkUpdateTimer = 3;
				m_ObjectManager.Iterate<void>(&SendUpdatePackets, NULL);
			}
		}
		else
		{
			if(m_bNetworked)
			{
				//We have a purposeful disconnect
#if _DEBUG
				printf("No longer connected\n");
#endif
				CGame::GetInstance()->ChangeState(new CState_MainMenu);
			}
		}
	NW_MANAGER_END

	m_ObjectManager.UpdateAllObjects(Delta);
	CAMERA->Update(Delta);
	if(!m_bNetworked)
		m_EnemyAI.Update(Delta);
	CParticleManager::GetInstance()->Update();
}

void CState_Gameplay::UpdateUnder(float Delta)
{
	if(m_bNetworked)
	{
		Update(Delta);
	}
}

void CState_Gameplay::Draw()
{
	m_bPaused = false;

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	D3DXMatrixTranslation(&Transform,-CAMERA->GetPositionX(),-CAMERA->GetPositionY(),0);
	m_ObjectManager.DrawAllObjects(Transform, CAMERA->GetViewRect());
}

bool FindUnitWithID(IObject* obj, CUpdateUnitPacket* packet)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		if(!testUnit->GetNWOwner() && testUnit->GetSharedID() == packet->shareID)
		{
			testUnit->SetPosition(packet->posx, packet->posy);
			//testUnit->SetVelocity(packet->velx, packet->vely);
			return true;
		}
	}
	return false;
}

bool ProcDamage(IObject* obj, CDealDamagePacket* packet)
{
	IAttackable* testAtk = NULL;
	if(testAtk = dynamic_cast<IAttackable*>(obj))
	{
		if(testAtk->GetNWOwner() && testAtk->GetSharedID() == packet->to)
		{
			testAtk->ApplyDamage(packet->amt, true);
#if _DEBUG
			CUnitObject* testUnit = NULL;
			if(testUnit = dynamic_cast<CUnitObject*>(testAtk))
			{
				printf("Unit \"%s\" took %d damage.\n", testUnit->GetCreationString().c_str(), packet->amt);
			}
#endif
			return true;
		}
	}
	return false;
}

bool ProcMoveCommand(IObject* obj, CMoveCommandPacket* packet)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		if(!testUnit->GetNWOwner() && testUnit->GetSharedID() == packet->shareID)
		{
			CMoveCommand* newCommand = new CMoveCommand(false);
			newCommand->SetPartAttacking(packet->partAttacking);

			CAttackMoveCommand* checkAttack = NULL;
			if(newCommand->GetPartAttacking())
			{
				checkAttack = new CAttackMoveCommand();
				checkAttack->SetMoveCommand(newCommand);
			}
			newCommand->SetDestinationX(packet->posx);
			newCommand->SetDestinationY(packet->posy);
			newCommand->SetProcessPath(false);
			for(int i = 0; i < packet->numPaths; i++)
			{
				newCommand->GetPath()->push_back(CPathNode::FindNodeFromID((int)packet->pathID[i]));		//push_front?
			}
			if(checkAttack)
			{
#if _DEBUG
				//printf("Unit #: %d Type: %s is now attack moving across %d nodes.", packet->shareID, testUnit->GetCreationString().c_str(), checkAttack->GetMoveCommand()->GetPath()->size());
#endif
				testUnit->SetUnitAICommand(checkAttack);
			}
			else
			{
#if _DEBUG
				//printf("Unit #: %d Type: %s is now moving across %d nodes.", packet->shareID, testUnit->GetCreationString().c_str(), newCommand->GetPath()->size());
#endif
				testUnit->SetUnitAICommand(newCommand);
			}
			return true;
		}
	}
	return false;
}

bool ProcAttackCommand(IObject* obj, SProcAttack* _atkUnit)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		if(!testUnit->GetNWOwner() && testUnit->GetSharedID() == _atkUnit->id)
		{
			CAttackCommand* newCommand = new CAttackCommand;
			newCommand->SetTarget(_atkUnit->atkUnit);
			testUnit->SetUnitAICommand(newCommand);
			return true;
		}
	}
	return false;
}

bool FindUnit(IObject* obj, SFindUnit* _find)
{
	CUnitObject* testUnit = NULL;
	_find->retObj = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		if(testUnit->GetNWOwner() && testUnit->GetSharedID() == _find->id)
		{
			_find->retObj = testUnit;
			return true;
		}
	}
	return false;
}

bool ProcDropResource(IObject* obj, CDropResourcePacket* packet)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		if(testUnit->GetNWOwner() != packet->mine && testUnit->GetSharedID() == packet->unitID)
		{
			CCookingResource rsc;
			rsc.SetResourceType((RESOURCE_TYPE)packet->resourceType);
			DropResourceOnUnit(testUnit, &rsc, true);
			return true;
		}
	}
	return false;
}

bool ProcUnitStats(IObject* obj, CUpdateUnitStatsPacket* packet)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		if(testUnit->GetNWOwner() != packet->mine && testUnit->GetSharedID() == packet->shareID)
		{
			testUnit->SetBaseAttack(packet->attack);
			testUnit->SetBaseAttackSpeed(packet->attackSpeed);
			testUnit->SetBaseDefense(packet->defense);
			testUnit->SetBaseMovement(packet->movementSpeed);
			testUnit->SetBaseEvade(packet->evade);
			return true;
		}
	}
	return false;
}

bool ProcSimpleSkill(IObject* obj, CUseSimpleSkillPacket* packet)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		if(testUnit->GetNWOwner() != packet->mine && testUnit->GetSharedID() == packet->shareID)
		{
			(*testUnit->GetSkill(packet->skillNum))();
			return true;
		}
	}
	return false;
}

bool SendUpdatePackets(IObject* obj, void* _ptr)
{
	INetworkable* testUnit = NULL;
	if(testUnit = dynamic_cast<INetworkable*>(obj))
	{
		if(testUnit->GetNWOwner())
			testUnit->SendUpdatePacket();
	}
	return false;
}

bool ProcStatusEffect(IObject* obj, CSendStatusEffectPacket* packet)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		if(testUnit->GetNWOwner() != packet->mine && testUnit->GetSharedID() == packet->shareID)
		{
			CStatusEffect* effect = new CStatusEffect();
			effect->Begin = CStatusEffect::GetStatusEffect(packet->Begin);
			effect->Update = CStatusEffect::GetStatusEffect(packet->Update);
			effect->End = CStatusEffect::GetStatusEffect(packet->End);
			effect->BeginSender = CStatusEffect::GetStatusEffectSender(packet->BeginSender);
			effect->UpdateSender = CStatusEffect::GetStatusEffectSender(packet->UpdateSender);
			effect->EndSender = CStatusEffect::GetStatusEffectSender(packet->EndSender);
			effect->SetTimer(packet->Timer);
			testUnit->AddStatusEffect(effect);
			return true;
		}
	}
	return false;
}

bool ProcCreateProjectile(IObject* obj, CCreateProjectilePacket* packet)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		if(testUnit->GetNWOwner() != packet->mine && testUnit->GetSharedID() == packet->shareID)
		{
			CProjectile* newProj = static_cast<CProjectile*>(CObjectFactory::GetInstance()->Construct(packet->creationString, testUnit->GetObjectManagerOwner()));
			newProj->OnHit = CProjectile::FindHitFunction(packet->hitFunction);
			newProj->SetNWOwner(false);
			newProj->SetupProjectile(D3DXVECTOR2(packet->x, packet->y), testUnit, packet->speed, packet->damage);
			return true;
		}
	}
	return false;
}
