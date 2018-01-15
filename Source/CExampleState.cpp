//////////////////////////////////////////////////////
// File: "CExampleState.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/21/09
// Last Modified By: JS
//////////////////////////////////////////////////////
#include "CExampleState.h"
#include "Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "Managers/Bad Chef/CObjectFactory.h"
#include "Game/Objects/Units/CUnitObject.h"
#include "Game/Objects/GUI Objects/CCursor.h"
#include "Game/Objects/GUI Objects/CButton.h"
#include "Game/Objects/Base/CCamera.h"
#include "Game/Cooking Mini-Game/CCookingResource.h"
#include "Managers/SGD/CSGD_DirectInput.h"
#include "Managers/Bad Chef/Profiles/CProfileManager.h"
#include "Game/Objects/Tiles/CTileStage.h"
#include "Game/Objects/Path Nodes/CPathNode.h"
#include "Game/Objects/Appliances/CStove.h"
#include "Game/Objects/Appliances/CPlate.h"
#include "Game/Objects/Appliances/CCoffeeMaker.h"
#include "Game/Objects/GUI Objects/CSelectionHUD.h"
#include "Game/Objects/GUI Objects/CActionsHUD.h"
#include "Game/Objects/GUI Objects/CUnitDataHUD.h"
#include "Game/Objects/GUI Objects/CMiniMap.h"
#include "Game/Cooking Mini-Game/CPan.h"
#include "Game/Cooking Mini-Game/CCookingUnit.h"
#include "Game/Cooking Mini-Game/CSpiceHolder.h"
#include "Managers/Bad Chef/Text/CTextManager.h"
#include "Game/Objects/Units/CSkilledUnitObject.h"
#include "Game/Enemy AI/CEnemyAI.h"
#include "Game/Enemy AI/Tasks/CAttackStoveTask.h"

CExampleState::CExampleState()
{
	// Load sprites.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromImage("Resources/Graphics/Units/ff_TestPlate.png");
	SpriteManager->LoadSpriteFromImage("Resources/Graphics/Units/ff_TestCoffeeMaker.png");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/ExampleSprite.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Button.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/ExampleCursor.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/SelectionBar.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/EmptyPortrait.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/ActionsBar.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/ActionButton.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/MoveIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/AttackIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/StopIcon.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/UnitData.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Stove.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Egg.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/ResourcePlate.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/ResourceFruit.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/ResourceMeat.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/ResourceWheat.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/ResourceEgg.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Pan.spr");	
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Toast_Test.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Arms.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/MiniMap.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/WhiteShaker.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/BlackShaker.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Status Effects/OverspiceStatus.spr");
	SpriteManager->LoadSpriteFromSprite("Resources/Data/Status Effects/FastStatus.spr");

	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Banana.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_BlueBerryMuffin.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_FrenchToast.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_GrapeFruit.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Ham.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Pancake.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Sausage.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_ScrambledEgg.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Strawberry.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_SunnySideUpEgg.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Data/FF_Unit_Waffle.PNG");
	SpriteManager->LoadSpriteFromImage("Resources/Graphics/User Interface/ff_CaptureBar.png");

	BITMAP->InitTextManager();
	SpriteManager->LoadSpriteFromImage("Resources/Graphics/Fonts/Font.png");
	BITMAP->AddFont("Resources/Graphics/Fonts/Font.png",32,32,0,16,16);


	// Register factory methods with object factory.
	m_ObjectFactory = CObjectFactory::GetInstance();
	m_ObjectFactory->AddMethod("Button", &ExampleButtonFactoryMethod);
	m_ObjectFactory->AddMethod("Cursor", &CCursor::CursorFactoryMethod);
	m_ObjectFactory->AddMethod("SelectionBar", &CSelectionHUD::CreateSelectionHUD);
	m_ObjectFactory->AddMethod("ActionsBar", &CActionsHUD::CreateActionsHUD);
	m_ObjectFactory->AddMethod("UnitData", &CUnitDataHUD::CreateUnitDataHUD);
	
	m_ObjectFactory->AddMethod("MiniMap", &CMiniMap::CreateMiniMap);
	m_ObjectFactory->AddMethod("Stove", &CreateStoveFactoryMethod);
	m_ObjectFactory->AddMethod("ResourcePlate", &CreateResourcePlateFactoryMethod);
	m_ObjectFactory->AddMethod("ResourceEgg", &CreateEggFactoryMethod);
	m_ObjectFactory->AddMethod("ResourceMeat", &CreateMeatFactoryMethod);
	m_ObjectFactory->AddMethod("ResourceWheat", &CreateWheatFactoryMethod);
	m_ObjectFactory->AddMethod("ResourceFruit", &CreateFruitFactoryMethod);
	m_ObjectFactory->AddMethod("Pan", &CreatePanFactoryMethod);	
	m_ObjectFactory->AddMethod("Toast", &CreateToast);
	m_ObjectFactory->AddMethod("SunnySideUpEgg", &CreateSunnySideUpEgg);
	m_ObjectFactory->AddMethod("ScrambledEgg", &CreateScrambledEgg);
	m_ObjectFactory->AddMethod("Sausage", &CreateSausage);
	m_ObjectFactory->AddMethod("Ham", &CreateHam);
	m_ObjectFactory->AddMethod("Pancake", &CreatePancake);
	m_ObjectFactory->AddMethod("Waffle", &CreateWaffle);
	m_ObjectFactory->AddMethod("BluberryMuffin", &CreateBluberryMuffin);
	m_ObjectFactory->AddMethod("FrenchToast", &CreateFrenchToast);
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

	CAMERA->InitCamera(1024.0f,768.0f,2000.0f,2000.0f,0.0f,0.0f,5.0f);
	CAMERA->SetCameraSpeed(1000.0f);

	PROFILE_MANAGER->InitProfileManager();

	// Create objects.
	m_ObjectManager.Initialize(6);
	m_ObjectManager.SetLayerCoordinateSpace(4, CObjectLayer::ECoordinateSpace::CS_Screen);

	//CButton* Button = static_cast<CButton*>(m_ObjectFactory->Construct("Button", &m_ObjectManager));
	//Button->SetPosition(100, 50);
	
	CTileStage::GetInstance()->InitMap("Resources/Data/FinalTest.FFM", &m_ObjectManager);		//Example on how to create a stove...	//Example on how to create a stove...	//It will need to be different

	//CStove* Stove = static_cast<CStove*>(m_ObjectFactory->Construct("Stove", &m_ObjectManager));
	//Stove->SetPosition(128,184);
	//Stove->InitializeCookingMinigame(true, &m_ObjectManager);
	//Stove->AddCookingResource(RESOURCE_EGG, &m_ObjectManager);

	CAttackStoveTask* AttackStoveTask = new CAttackStoveTask;
	CSkilledUnitObject* unitToast;

	for(int i = 0; i < 10; i++)
	{
		unitToast = static_cast<CSkilledUnitObject*>(m_ObjectFactory->Construct("Toast", &m_ObjectManager));
		unitToast->SetPosition(800 + (i*65), 150);
		unitToast->SetTeamValue(ETeamValue::TEAM_PLAYER);
	}

	for(int i = 0; i < 10; i++)
	{
		unitToast = static_cast<CSkilledUnitObject*>(m_ObjectFactory->Construct("Toast", &m_ObjectManager));
		unitToast->SetPosition(1500 + i, 150);
		unitToast->SetTeamValue(ETeamValue::TEAM_ENEMY);
		AttackStoveTask->AssignUnit(unitToast);
		m_EnemyAI.AssignUnit(unitToast);
	}

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
	m_ObjectFactory->Construct("Cursor", &m_ObjectManager);

	std::vector<CStove*>& Stoves = CTileStage::GetInstance()->GetStoves();
	for(unsigned char i = 0; i < Stoves.size(); ++i)
	{
		if(Stoves[i]->GetTeamValue() == ETeamValue::TEAM_PLAYER)
		{
			AttackStoveTask->SetTargetStove(Stoves[i]);
			m_EnemyAI.AssignTask(AttackStoveTask);
			m_EnemyAI.SetEnemyStove(Stoves[i]);
			continue;
		}
		else if(Stoves[i]->GetTeamValue() == ETeamValue::TEAM_ENEMY)
		{
			m_EnemyAI.SetFriendlyStove(Stoves[i]);
			continue;
		}
	}
}

CExampleState::~CExampleState()
{
	CAMERA->ShutdownCamera();

	PROFILE_MANAGER->ShutdownProfileManager();

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

	// Unload sprites.
	BITMAP->ShutdownTextManager();
	CSpriteManager::GetInstance()->UnloadAllSprites();
}

void CExampleState::OnEnter()
{

}

void CExampleState::OnExit()
{
	delete this;
}

bool CExampleState::Input()
{
	CCursor::GetInstance()->Input();

	if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[UP]))
		CAMERA->MoveCameraY(-CAMERA->GetCameraSpeed());
	if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[DOWN]))
		CAMERA->MoveCameraY(CAMERA->GetCameraSpeed());

	if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[LEFT]))
		CAMERA->MoveCameraX(-CAMERA->GetCameraSpeed());
	if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[RIGHT]))
		CAMERA->MoveCameraX(CAMERA->GetCameraSpeed());

	return true;
}

void CExampleState::Update(float Delta)
{
	m_ObjectManager.UpdateAllObjects(Delta);
	CAMERA->UpdateCamera(Delta);
	m_EnemyAI.Update(Delta);
}

void CExampleState::Draw()
{
	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	D3DXMatrixTranslation(&Transform,-CAMERA->GetCameraPosX(),-CAMERA->GetCameraPosY(),0);
	m_ObjectManager.DrawAllObjects(Transform, CAMERA->GetViewRect());
}

void ButtonOnClick(CButton* This)
{
	PostQuitMessage(0);
}

IObject* ExampleButtonFactoryMethod(CObjectManager* ObjectManager)
{
	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/Button.spr");
	if(!Sprite)
		return NULL;

	// Create object.
	CButton* Button = new CButton;
	Button->SetSprite(Sprite);
	Button->SetOnClick(&ButtonOnClick);	
	ObjectManager->AddObjectToLayer(4, Button);
	return Button;
}