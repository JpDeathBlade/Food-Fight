//////////////////////////////////////////////////////
// File: "CGame.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include <time.h>
#include "CGame.h"
#include "Game/States/IGameState.h"
#include "Game/States/CState_MainMenu.h"
#include "Managers/Bad Chef/CObjectFactory.h"
#include "Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "Managers/Bad Chef/Text/CTextManager.h"
#include "Managers/SGD/CSGD_Direct3D.h"
#include "Managers/SGD/CSGD_DirectInput.h"
#include "Managers/Bad Chef/Messages/CMessageSystem.h"
#include "Managers/Bad Chef/Profiles/CProfileManager.h"
#include "Game/Cooking Mini-Game/CRecipeManager.h"
#include "Utilities/CCodeProfiler.h"
#include "Managers/Bad Chef/String Table/CStringTable.h"
#include "Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "Managers/Bad Chef/Networking/CNetworkManager.h"
#include "Managers/Bad Chef/Audio/CAudioManager.h"
#include "Game/Objects/GUI Objects/CCursor.h"
#include "Game/Objects/Base/CCamera.h"

CGame* CGame::m_Instance = NULL;

CGame::CGame()
{
	m_ObjectFactory = NULL;
	m_Direct3D = NULL;
	m_DirectInput = NULL;
	m_NumStatesToPop = 0;
	m_bClearStates = false;
	m_Cursor = NULL;
}

CGame::~CGame()
{
	ShutDown();
	m_Instance = NULL;
}

bool CGame::Input()
{
	//m_DirectInput->AcquireAll();
	// Refresh input state (should only be done ONCE a frame)
	m_DirectInput->ReadDevices();

	// Input
	m_Cursor->Input();
	if(!m_States.empty())
		if(!m_States.back()->Input())
			return false;

	return true;
}

void CGame::Update(float Delta)
{
	// Update states
	if(!m_States.empty())
	{
		for(unsigned int i = 0; i < m_States.size()-1; i++)
			m_States[i]->UpdateUnder(Delta);

		m_States.back()->Update(Delta);
	}

	CAudioManager::GetInstance()->Update();

	m_Cursor->Update(Delta);


	XBOX->Update();

	CMessageSystem::GetInstance()->ProcessMessages();
}

void CGame::Draw()
{
	m_Direct3D->Clear();
	m_Direct3D->DeviceBegin();
	m_Direct3D->SpriteBegin();

	// Render states
	if(!m_States.empty())
	{
		for(unsigned int i = 0; i < m_States.size()-1; i++)
		{
			m_States[i]->DrawUnder();
			m_Direct3D->GetSprite()->Flush();
		}

		m_States.back()->Draw();
		m_Direct3D->GetSprite()->Flush();
	}

	m_Cursor->Draw();

	m_Direct3D->SpriteEnd();
	m_Direct3D->DeviceEnd();
	m_Direct3D->Present();
}

CGame* CGame::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CGame;
	return m_Instance;
}

void CGame::DeleteInstance()
{
	delete m_Instance;
}

bool CGame::Initialize(SGameParameters GameParameters)
{
	bool ReturnValue = false;

	//PROFILER->InitCodeProfiler();

	// Initialize miscellaneous systems.
	//CSettings* Settings = CSettings::GetInstance();
	m_ObjectFactory = CObjectFactory::GetInstance();

	PROFILE_MANAGER->InitProfileManager();
	GameParameters.Windowed = !PROFILE_MANAGER->GetCurrentProfile()->bFullscreen;

	// Initialize DirectX wrappers.
	m_Direct3D = CSGD_Direct3D::GetInstance();
	m_Direct3D->InitDirect3D(GameParameters.Window, GameParameters.ClientWidth, GameParameters.ClientHeight, GameParameters.Windowed, GameParameters.VSync);

	m_DirectInput = CSGD_DirectInput::GetInstance();
	m_DirectInput->InitDirectInput(GameParameters.Window, GameParameters.ApplicationInstance, DI_KEYBOARD | DI_MOUSE, DI_MOUSE);

	ReturnValue = CAudioManager::GetInstance()->Initialize();
#if _DEBUG
	if(ReturnValue)
		printf("Audio Manager Initialized\n");
	else
		printf("ERROR: Audio Manager Failed To Initialize\n");
#endif

	CAudioManager::GetInstance()->AddCategory("Music");
	CAudioManager::GetInstance()->GetCategory("Music")->SetVolume(PROFILE_MANAGER->GetCurrentProfile()->MusicVolume / static_cast<float>(UCHAR_MAX));
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Music/ElevatorStyle.wav", "Music", false, true);

	CAudioManager::GetInstance()->AddCategory("Effects");
	CAudioManager::GetInstance()->GetCategory("Effects")->SetVolume(PROFILE_MANAGER->GetCurrentProfile()->EffectVolume / static_cast<float>(UCHAR_MAX));
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Units/Punch.wav", "Effects", true);

	CAudioManager::GetInstance()->AddCategory("Voices");
	CAudioManager::GetInstance()->GetCategory("Voices")->SetVolume(PROFILE_MANAGER->GetCurrentProfile()->VoiceVolume / static_cast<float>(UCHAR_MAX));
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/Voices/BarrelRoll.wav", "Voices", true);

	CAudioManager::GetInstance()->AddCategory("User Interface");
	CAudioManager::GetInstance()->GetCategory("User Interface")->SetVolume(PROFILE_MANAGER->GetCurrentProfile()->UserInterfaceVolume / static_cast<float>(UCHAR_MAX));
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/User Interface/ButtonClick.wav", "User Interface");
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/User Interface/BeepA.wav", "User Interface");
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Effects/User Interface/BeepB.wav", "User Interface");

	srand((unsigned int)time(0));

	CRecipeManager::GetInstance()->InitializeRecipes();

	BITMAP->InitTextManager();
	CSpriteManager::GetInstance()->LoadSpriteFromImage("Resources/Graphics/Fonts/Font.png");
	BITMAP->AddFont("Resources/Graphics/Fonts/Font.png",32,32,0,16,16);

	CSpriteManager::GetInstance()->LoadSpriteFromSprite("Resources/Data/Font.spr");
	BITMAP->AddFontSprite("Resources/Data/Font.spr");
	
	CSpriteManager::GetInstance()->LoadSpriteFromSprite("Resources/Graphics/User Interface/OvenMittCursor.spr");
	CObjectFactory::GetInstance()->AddMethod("Cursor", &CCursor::CursorFactoryMethod);
	CObjectFactory::GetInstance()->Construct("Cursor", NULL);
	m_Cursor = CCursor::GetInstance();

	CSpriteManager::GetInstance()->LoadSpriteFromImage("Resources/Graphics/User Interface/FoodFight_Menu.png");

	XBOX->InitCXBoxManager(1);

	if(PROFILE_MANAGER->GetCurrentProfile()->Language == EL_English)
		CStringTable::GetInstance()->LoadDictionary("Resources/Data/English.txt");
	else if(PROFILE_MANAGER->GetCurrentProfile()->Language == EL_Spanish)
		CStringTable::GetInstance()->LoadDictionary("Resources/Data/Spanish.txt");

	CStatusEffect::LoadStatusEffectTable();

#if _DEBUG
	printf("Game Initialized\n");
#endif

	// Create initial state.
	PushState(new CState_MainMenu);

	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Music/ElevatorStyle.wav", "Music")->Play(-1);

	return true;
}

void CGame::ShutDown()
{
	CSpriteManager::GetInstance()->UnloadSprite("Resources/Graphics/User Interface/FoodFight_Menu.png");

	// Remove all states.
	while(!m_States.empty())
		PopStateNow();

	XBOX->ShutdownCXBoxManager();

	CCursor::DeleteInstance();

	// Shut down String Table
	CStringTable::DeleteInstance();

	// Shut down Profile Manager
	PROFILE_MANAGER->ShutdownProfileManager();

	// Shut down Sprite Manager.
	CSpriteManager::DeleteInstance();

	//Shut down the Recipe Manager
	CRecipeManager::DeleteInstance();

	//Shut down the Message System
	CMessageSystem::DeleteInstance();

	CAudioManager::GetInstance()->UnloadSound("Resources/Sounds/Effects/Voices/BarrelRoll.wav", "Voices");
	CAudioManager::DeleteInstance();

	// Shut down DirectX wrappers.
	m_DirectInput->ShutdownDirectInput();
	m_DirectInput = NULL;

	m_Direct3D->ShutdownDirect3D();
	m_Direct3D = NULL;

	// Shut down miscellaneous systems.
	CObjectFactory::DeleteInstance();
	//CSettings::DeleteInstance();

	//PROFILER->ShutdownCodeProfiler();
	CAMERA->DeleteInstance();
}

bool CGame::Run()
{
	if(m_bClearStates)
	{
		m_bClearStates = false;
		while(!m_States.empty())
			PopStateNow();
	}

	for(int i = 0; i < m_NumStatesToPop; i++)
	{
		PopStateNow();
	}
	m_NumStatesToPop = 0;

	for(std::list<IGameState*>::iterator Iterator = m_StatesToPush.begin(); Iterator != m_StatesToPush.end(); ++Iterator)
	{
		PushStateNow((*Iterator));
	}
	m_StatesToPush.clear();

	// Delta time calculations.
	static float PreviousTime = static_cast<float>(GetTickCount());
	float CurrentTime = static_cast<float>(GetTickCount());
	float Delta = min((CurrentTime - PreviousTime) * 0.001f, 0.1f);
	PreviousTime = CurrentTime;

	// Core loop.
	if (!Input())
		return false;

	Update(Delta);
	Draw();

	return true;
}

void CGame::ChangeState(IGameState* State)
{
	m_bClearStates = true;
	PushState(State);
}

void CGame::PushState(IGameState* State)
{
	m_StatesToPush.push_back(State);
}

void CGame::PushStateNow(IGameState* State)
{
	if(State)
	{
		if(!m_States.empty())
		{
			m_States.back()->OnExit();
		}
		m_States.push_back(State);
		m_States.back()->OnPush();
		m_States.back()->OnEnter();
		m_Cursor->SetObjectManagerOwner(State->GetObjectManager());
	}
}

void CGame::PopState()
{
	m_NumStatesToPop++;
}

void CGame::PopStateNow()
{
	if(!m_States.empty())
	{
		m_States.back()->OnExit();
		m_States.back()->OnPop();
		m_States.pop_back();
		if(!m_States.empty())
		{
			m_Cursor->SetObjectManagerOwner(m_States.back()->GetObjectManager());
			m_States.back()->OnEnter();
		}
	}
}


void CGame::ClearAllStates()
{
	m_bClearStates = true;
}

void CGame::ChangeDisplayParameters(int ClientWidth, int ClientHeight, bool Windowed)
{
	CSGD_Direct3D::GetInstance()->ChangeDisplayParam(ClientWidth, ClientHeight, Windowed);
	PROFILE_MANAGER->GetCurrentProfile()->bFullscreen = !Windowed;
}
