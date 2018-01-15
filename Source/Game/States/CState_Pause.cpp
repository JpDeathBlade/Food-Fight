#include "CState_Pause.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../../CGame.h"
#include "CState_MainMenu.h"
#include "CState_Options.h"
#include "CState_HowToPlay.h"
#include "CState_Profile.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../Objects/GUI Objects/CButton.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"

CState_Pause::CState_Pause()
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	CSprite* ButtonSprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");
	SetSprite(ButtonSprite);

	for(unsigned char i = 0; i < 4; ++i)
	{
		m_Buttons[i] = new CButton();
		m_Buttons[i]->SetSprite(ButtonSprite);
		m_Buttons[i]->SetPosition(150.0f, 200.0f);
		m_Buttons[i]->SetFontFileName("Resources/Data/Font.spr");
		m_Buttons[i]->SetFontScale(0.75f, 0.75f);
		m_Buttons[i]->SetStringOffset(10, 10);
	}

	m_Buttons[0]->SetString(STRING->FindString("Options").c_str());
	m_Buttons[0]->SetOnRelease(&OptionsButtonOnClick);

	m_Buttons[1]->SetString(STRING->FindString("How To Play").c_str());
	m_Buttons[1]->SetOnRelease(&HowToPlayButtonOnClick);

	m_Buttons[2]->SetString(STRING->FindString("Resume").c_str());
	m_Buttons[2]->SetOnRelease(&ResumeButtonOnClick);

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-021
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Removed Profiles Button

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-021
	//////////////////////////////////////////////////////////////////////////

	m_Buttons[3]->SetString(STRING->FindString("Exit").c_str());
	m_Buttons[3]->SetOnRelease(&ExitButtonOnClick);

	for(unsigned char i = 0; i < 4; ++i)
	{
		m_Buttons[i]->FitToText();
		if(i > 0)
			m_Buttons[i]->SetPositionY(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
		m_ObjectManager.AddObjectToLayer(0, m_Buttons[i]);
	}

	m_nSelection = static_cast<EMenu>(0);
	SetSelection(MIN);
}

CState_Pause::~CState_Pause()
{

}

bool CState_Pause::Input()
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

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->KeyPressedEx(DIK_PAUSE)|| XBOX->GetController(0)->IsPressed_START())
		ResumeButtonOnClick();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || XBOX->GetController(0)->IsPressed_A())
	{
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/ButtonClick.wav", "User Interface")->Play();
		switch(m_nSelection)
		{
			case OPTIONS:
				OptionsButtonOnClick();
				break;
			case HOWTOPLAY:
				HowToPlayButtonOnClick();
				break;
			case RESUME:
				ResumeButtonOnClick();
				break;
			case EXIT:
				ExitButtonOnClick();
				break;
		}
	}

	return true;
}

void CState_Pause::Draw()
{
	DrawGUIFrame(this, D3DXVECTOR2(140.0f, 140.0f), D3DXVECTOR2(200.0f, 350.0f));

	BITMAP->DrawStringSprite("Resources/Data/Font.spr",STRING->FindString("Paused").c_str(),150.0f,150.0f);

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT ViewRectangle;
	ViewRectangle.left = 0;
	ViewRectangle.right = 1024;
	ViewRectangle.top = 0;
	ViewRectangle.bottom = 768;
	m_ObjectManager.DrawAllObjects(Transform, ViewRectangle);
}

void CState_Pause::COptionsButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PushState(new CState_Options);
}

void CState_Pause::CHowToPlayButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PushState(new CState_HowToPlay);
}

void CState_Pause::CResumeButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PopState();
}

void CState_Pause::CExitButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->ChangeState(new CState_MainMenu);
}

CState_Pause::EMenu CState_Pause::GetSelection() const
{
	return m_nSelection;
}

void CState_Pause::SetSelection(CState_Pause::EMenu Selection)
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

void CState_Pause::OnEnter()
{
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-004
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	for(unsigned char i = 0; i < 4; ++i)
	{
		m_Buttons[i]->SetPosition(150.0f, 200.0f);
	}

	m_Buttons[0]->SetString(STRING->FindString("Options").c_str());

	m_Buttons[1]->SetString(STRING->FindString("How To Play").c_str());

	m_Buttons[2]->SetString(STRING->FindString("Resume").c_str());

	m_Buttons[3]->SetString(STRING->FindString("Exit").c_str());

	for(unsigned char i = 0; i < 4; ++i)
	{
		m_Buttons[i]->FitToText();
		if(i > 0)
			m_Buttons[i]->SetPositionY(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
	}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-004
	//////////////////////////////////////////////////////////////////////////
}