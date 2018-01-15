#include "CState_MainMenu.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../../CGame.h"
#include "CState_Gameplay.h"
#include "CState_LevelSelect.h"
#include "CState_MLobby.h"
#include "CState_Options.h"
#include "CState_HowToPlay.h"
#include "CState_Profile.h"
#include "CState_Credits.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../Objects/GUI Objects/CButton.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"

CState_MainMenu::CState_MainMenu()
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	CSprite* ButtonSprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");

	for(unsigned char i = 0; i < 6; ++i)
	{
		m_Buttons[i] = new CButton();
		m_Buttons[i]->SetSprite(ButtonSprite);
		m_Buttons[i]->SetPosition(75.0f, 300.0f);
		m_Buttons[i]->SetFontFileName("Resources/Data/Font.spr");
		m_Buttons[i]->SetStringOffset(10, 10);
	}

	m_Buttons[0]->SetString(STRING->FindString("Campaign").c_str());
	m_Buttons[0]->SetOnRelease(&CampaignButtonOnClick);
	m_Buttons[1]->SetString(STRING->FindString("Multiplayer").c_str());
	m_Buttons[1]->SetOnRelease(&MultiplayerButtonOnClick);
	m_Buttons[2]->SetString(STRING->FindString("Options").c_str());
	m_Buttons[2]->SetOnRelease(&OptionsButtonOnClick);
	m_Buttons[3]->SetString(STRING->FindString("Profiles").c_str());
	m_Buttons[3]->SetOnRelease(&ProfilesButtonOnClick);
	m_Buttons[4]->SetString(STRING->FindString("Credits").c_str());
	m_Buttons[4]->SetOnRelease(&CreditsButtonOnClick);
	m_Buttons[5]->SetString(STRING->FindString("Exit").c_str());
	m_Buttons[5]->SetOnRelease(&ExitButtonOnClick);

	for(unsigned char i = 0; i < 6; ++i)
	{
		m_Buttons[i]->FitToText();
		if(i > 0)
			m_Buttons[i]->SetPositionY(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
		m_ObjectManager.AddObjectToLayer(0, m_Buttons[i]);
	}

	m_nSelection = static_cast<EMenu>(0);
	SetSelection(static_cast<EMenu>(0));
}

CState_MainMenu::~CState_MainMenu()
{

}

bool CState_MainMenu::Input()
{
	CSGD_DirectInput* DirectInput = CSGD_DirectInput::GetInstance();
	if(DirectInput->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[UP]) || XBOX->GetController(0)->IsPressed_DPAD_UP())
	{
		SetSelection(static_cast<EMenu>(GetSelection() - 1));

		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepA.wav", "User Interface")->Play();
	}
	if(DirectInput->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[DOWN])|| XBOX->GetController(0)->IsPressed_DPAD_DOWN())
	{
		SetSelection(static_cast<EMenu>(GetSelection() + 1));

		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepA.wav", "User Interface")->Play();
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || XBOX->GetController(0)->IsPressed_A())
	{
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/ButtonClick.wav", "User Interface")->Play();
		m_Buttons[GetSelection()]->OnRelease();
	}
	return true;
}

void CState_MainMenu::Update(float Delta)
{
	m_ObjectManager.UpdateAllObjects(Delta);
}

void CState_MainMenu::Draw()
{
	CSGD_Direct3D::GetInstance()->Clear();

	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/FoodFight_Menu.png")->Draw(D3DXVECTOR2(0.0f, -250.0f));

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT ViewRectangle;
	ViewRectangle.left = 0;
	ViewRectangle.right = 1024;
	ViewRectangle.top = 0;
	ViewRectangle.bottom = 768;
	m_ObjectManager.DrawAllObjects(Transform, ViewRectangle);

	BITMAP->DrawStringSprite("Resources/Data/Font.spr",STRING->FindString("Food Fight:\n\tThe Breakfast Wars").c_str(),150.0f,100.0f, 1.5f, 1.5f, 0.0f, D3DCOLOR_XRGB(0, 0, 0));
}

CState_MainMenu::EMenu CState_MainMenu::GetSelection() const
{
	return m_nSelection;
}

void CState_MainMenu::SetSelection(CState_MainMenu::EMenu Selection)
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

void CState_MainMenu::CCampaignButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PushState(new CState_LevelSelect);
}

void CState_MainMenu::CMultiplayerButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PushState(new CState_MLobby);
}

void CState_MainMenu::COptionsButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PushState(new CState_Options);
}

void CState_MainMenu::CProfilesButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PushState(new CState_Profile);
}

void CState_MainMenu::CCreditsButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PushState(new CState_Credits);
}

void CState_MainMenu::CExitButtonOnClickFunctor::operator()()
{
	PostQuitMessage(0);
}

void CState_MainMenu::OnEnter()
{
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-004
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	for(unsigned char i = 0; i < 6; ++i)
	{
		m_Buttons[i]->SetPosition(75.0f, 300.0f);
	}

	m_Buttons[0]->SetString(STRING->FindString("Campaign").c_str());
	m_Buttons[1]->SetString(STRING->FindString("Multiplayer").c_str());
	m_Buttons[2]->SetString(STRING->FindString("Options").c_str());
	m_Buttons[3]->SetString(STRING->FindString("Profiles").c_str());
	m_Buttons[4]->SetString(STRING->FindString("Credits").c_str());
	m_Buttons[5]->SetString(STRING->FindString("Exit").c_str());

	for(unsigned char i = 0; i < 6; ++i)
	{
		m_Buttons[i]->FitToText();
		if(i > 0)
			m_Buttons[i]->SetPositionY(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
	}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-004
	//////////////////////////////////////////////////////////////////////////
}