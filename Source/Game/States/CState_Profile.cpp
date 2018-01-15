#include "CState_Profile.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../../CGame.h"
#include "../Objects/GUI Objects/CButton.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"

CState_Profile::CState_Profile()
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	CSprite* ButtonSprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");
	SetSprite(ButtonSprite);

	ProfileAButtonOnClick.SetProfileNumber(0);
	ProfileBButtonOnClick.SetProfileNumber(1);
	ProfileCButtonOnClick.SetProfileNumber(2);

	for(unsigned char i = 0; i < 5; ++i)
	{
		m_Buttons[i] = new CButton();
		m_Buttons[i]->SetSprite(ButtonSprite);
		m_Buttons[i]->SetPosition(125.0f, 375.0f);
		m_Buttons[i]->SetFontFileName("Resources/Data/Font.spr");
		m_Buttons[i]->SetStringOffset(10, 10);
	}

	m_Buttons[0]->SetString(PROFILE_MANAGER->GetProfile(0)->szName);
	m_Buttons[0]->SetFontScale(0.75f, 0.75f);
	m_Buttons[0]->SetOnRelease(&ProfileAButtonOnClick);

	m_Buttons[1]->SetString(PROFILE_MANAGER->GetProfile(1)->szName);
	m_Buttons[1]->SetFontScale(0.75f, 0.75f);
	m_Buttons[1]->SetOnRelease(&ProfileBButtonOnClick);

	m_Buttons[2]->SetString(PROFILE_MANAGER->GetProfile(2)->szName);
	m_Buttons[2]->SetFontScale(0.75f, 0.75f);
	m_Buttons[2]->SetOnRelease(&ProfileCButtonOnClick);

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BUG-016
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	m_Buttons[3]->SetString(STRING->FindString("Set Current Profile to Default").c_str());
	m_Buttons[3]->SetFontScale(0.75f, 0.75f);
	m_Buttons[3]->SetOnRelease(&DefaultButtonOnClick);

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BUG-016
	//////////////////////////////////////////////////////////////////////////

	m_Buttons[4]->SetString(STRING->FindString("Back").c_str());
	m_Buttons[4]->SetFontScale(0.75f, 0.75f);
	m_Buttons[4]->SetOnRelease(&BackButtonOnClick);

	for(unsigned char i = 0; i < 5; ++i)
	{
		m_Buttons[i]->FitToText();
		if(i > 0)
			m_Buttons[i]->SetPositionY(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
		m_ObjectManager.AddObjectToLayer(0, m_Buttons[i]);
	}

	m_nSelection = static_cast<EMenu>(0);
	SetSelection(MIN);
}

CState_Profile::~CState_Profile()
{

}

bool CState_Profile::Input()
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
			case PROFILEA:
				ProfileAButtonOnClick();
				break;
			case PROFILEB:
				ProfileBButtonOnClick();
				break;
			case PROFILEC:
				ProfileCButtonOnClick();
				break;
			case DEFAULT:
				DefaultButtonOnClick();
			case BACK:
				BackButtonOnClick();
				break;
		}
	}
	return true;
}

void CState_Profile::Update(float _delta)
{

}

void CState_Profile::Draw()
{
	CSGD_Direct3D::GetInstance()->Clear();

	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/FoodFight_Menu.png")->Draw(D3DXVECTOR2(0.0f, -250.0f));

	DrawGUIFrame(this, D3DXVECTOR2(50.0f, 300.0f), D3DXVECTOR2(924.0f, 350.0f));

	char buffer[128];

	sprintf_s(buffer,sizeof(buffer),"%s:  %s - %s",STRING->FindString("Profiles").c_str(), STRING->FindString("Current").c_str(),  PROFILE_MANAGER->GetCurrentProfile()->szName);
	BITMAP->DrawStringSprite("Resources/Data/Font.spr",buffer,75.0f, 325.0f);

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT ViewRectangle;
	ViewRectangle.left = 0;
	ViewRectangle.right = 1024;
	ViewRectangle.top = 0;
	ViewRectangle.bottom = 768;
	m_ObjectManager.DrawAllObjects(Transform, ViewRectangle);
}

void CState_Profile::CProfileButtonOnClickFunctor::operator()()
{
	PROFILE_MANAGER->SetCurrentProfile(GetProfileNumber());
}

unsigned char CState_Profile::CProfileButtonOnClickFunctor::GetProfileNumber() const
{
	return m_ProfileNumber;
}

void CState_Profile::CProfileButtonOnClickFunctor::SetProfileNumber(unsigned char ProfileNumber)
{
	m_ProfileNumber = ProfileNumber;
}

void CState_Profile::CBackButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PopState();
}

void CState_Profile::CSetToDefaultOnClickFunctor::operator()()
{
	PROFILE_MANAGER->SetToDefault(PROFILE_MANAGER->GetCurrentProfile()->unSlot);
}

CState_Profile::EMenu CState_Profile::GetSelection() const
{
	return m_nSelection;
}

void CState_Profile::SetSelection(CState_Profile::EMenu Selection)
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