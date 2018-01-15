//////////////////////////////////////////////////////
// File: "CState_LevelSelect.h"
// Creator: MG
// Creation Date: 10/12/09
// Last Modified: 10/12/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#include "CState_LevelSelect.h"
#include "CState_Gameplay.h"
#include "../../CGame.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../Objects/Base/CGameObject.h"
#include "../Objects/GUI Objects/CButton.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"

CState_LevelSelect::CState_LevelSelect()
{
	m_FileString = NULL;

	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	CSprite* ButtonSprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");

	for(unsigned char i = 0; i < 4; ++i)
	{
		m_Buttons[i] = new CButton();
		m_Buttons[i]->SetSprite(ButtonSprite);
		m_Buttons[i]->SetPosition(180.0f + (i * 220.0f), 400.0f);
		m_Buttons[i]->SetFontFileName("Resources/Data/Font.spr");
		m_Buttons[i]->SetStringOffset(10.0f, 10.0f);
		m_Buttons[i]->SetFontScale(0.75f, 0.75f);
	}

	m_Buttons[0]->SetString("Airborne");
	m_Level1Load.filename = "Resources/Data/Airborne.FFM";
	m_Buttons[0]->SetOnClick(&m_Level1Load);

	m_Buttons[1]->SetString("Dining Hall");
	m_Level2Load.filename = "Resources/Data/DiningRoom.FFM";
	m_Buttons[1]->SetOnClick(&m_Level2Load);

	m_Buttons[2]->SetString("U-Turn");
	m_Level3Load.filename = "Resources/Data/UTurn.FFM";
	m_Buttons[2]->SetOnClick(&m_Level3Load);

	m_Buttons[3]->SetString(STRING->FindString("Back").c_str());
	m_Buttons[3]->SetOnClick(&BackButtonOnClick);

	for(unsigned char i = 0; i < 4; ++i)
	{
		m_Buttons[i]->FitToText();
		m_ObjectManager.AddObjectToLayer(0, m_Buttons[i]);
	}

	m_Buttons[3]->SetPosition(512, 512);

	m_nSelection = static_cast<EMenu>(0);
	SetSelection(MIN);

	CSpriteManager::GetInstance()->LoadSpriteFromImage("Resources/Graphics/User Interface/Airborne_minimap.png");
	CSpriteManager::GetInstance()->LoadSpriteFromImage("Resources/Graphics/User Interface/Dining Hall_minimap.png");
	CSpriteManager::GetInstance()->LoadSpriteFromImage("Resources/Graphics/User Interface/U-Turn_minimap.png");
}

CState_LevelSelect::~CState_LevelSelect()
{

}

bool CState_LevelSelect::Input()
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[UP]) || XBOX->GetController(0)->IsPressed_DPAD_UP())
	{
		SetSelection(static_cast<EMenu>(GetSelection() - 1));
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepA.wav", "User Interface")->Play();
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[DOWN]) || XBOX->GetController(0)->IsPressed_DPAD_DOWN())
	{
		SetSelection(static_cast<EMenu>(GetSelection() + 1));
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepA.wav", "User Interface")->Play();
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || XBOX->GetController(0)->IsPressed_A())
	{
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/ButtonClick.wav", "User Interface")->Play();
		switch(m_nSelection)
		{
			case LEVELA:
				m_Level1Load();
				break;
			case LEVELB:
				m_Level2Load();
				break;
			case LEVELC:
				m_Level3Load();
				break;
			case BACK:
				BackButtonOnClick();
				break;
		}
	}
	return true;
}

void CState_LevelSelect::Update(float _delta)
{
	m_ObjectManager.UpdateAllObjects(_delta);
}

void CState_LevelSelect::Draw()
{
	CSGD_Direct3D::GetInstance()->Clear();

	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/FoodFight_Menu.png")->Draw(D3DXVECTOR2(0.0f, -250.0f));

	D3DXMATRIX ident;
	D3DXMatrixIdentity(&ident);
	RECT viewport;
	viewport.left = 0;
	viewport.top = 0;
	viewport.right = 1024;
	viewport.bottom = 768;
	m_ObjectManager.DrawAllObjects(ident, viewport);

	DrawGUIFrame(m_Buttons[0], D3DXVECTOR2(170, 240), D3DXVECTOR2(148, 148));
	DrawGUIFrame(m_Buttons[1], D3DXVECTOR2(395, 240), D3DXVECTOR2(148, 148));
	DrawGUIFrame(m_Buttons[2], D3DXVECTOR2(600, 240), D3DXVECTOR2(148, 148));

	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/Airborne_minimap.png")->Draw(0, 0, D3DXVECTOR2(180, 250));
	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/Dining Hall_minimap.png")->Draw(0, 0, D3DXVECTOR2(405, 250));
	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/U-Turn_minimap.png")->Draw(0, 0, D3DXVECTOR2(610, 250));
}

void CState_LevelSelect::CLoadLevelFunctor::operator()()
{
	CGame::GetInstance()->PushState(new CState_Gameplay(1, false, filename));
}

CState_LevelSelect::EMenu CState_LevelSelect::GetSelection() const
{
	return m_nSelection;
}

void CState_LevelSelect::SetSelection(CState_LevelSelect::EMenu Selection)
{
	if(m_Buttons[m_nSelection])
		m_Buttons[m_nSelection]->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));

	if(Selection > MAX)
		Selection = MIN;
	else if(Selection < MIN)
		Selection = MAX;

	m_nSelection = Selection;

	if(m_Buttons[m_nSelection])
		m_Buttons[m_nSelection]->SetTextColor(D3DCOLOR_XRGB(255, 127, 127));
}

void CState_LevelSelect::CBackButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PopState();
}