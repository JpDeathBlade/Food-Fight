#include "CState_KeyBinding.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../../CGame.h"
#include "../Objects/GUI Objects/CButton.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"

#define SPRITEMANAGER CSpriteManager::GetInstance()

CState_KeyBinding::CState_KeyBinding()
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	CSprite* ButtonSprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");
	SetSprite(ButtonSprite);

	for(unsigned char i = 0; i < 10; ++i)
	{
		KeyBindingOnClick[i].SetKeyBinding(static_cast<eKeyBindings>(i));
		KeyBindingOnClick[i].SetState(this);
	}

	float ColumnX = 150.0f;

	for(unsigned char i = 0; i < 11; ++i)
	{
		m_Buttons[i] = new CButton();
		m_Buttons[i]->SetSprite(ButtonSprite);
		m_Buttons[i]->SetPosition(ColumnX, 200.0f);
		m_Buttons[i]->SetFontFileName("Resources/Data/Font.spr");
		m_Buttons[i]->SetStringOffset(10, 10);
		m_Buttons[i]->SetFontScale(0.75f, 0.75f);
	}

	m_Buttons[0]->SetString(STRING->FindString("Up").c_str());
	m_Buttons[0]->SetOnRelease(&KeyBindingOnClick[0]);

	m_Buttons[1]->SetString(STRING->FindString("Down").c_str());
	m_Buttons[1]->SetOnRelease(&KeyBindingOnClick[1]);

	m_Buttons[2]->SetString(STRING->FindString("Left").c_str());
	m_Buttons[2]->SetOnRelease(&KeyBindingOnClick[2]);

	m_Buttons[3]->SetString(STRING->FindString("Right").c_str());
	m_Buttons[3]->SetOnRelease(&KeyBindingOnClick[3]);

	m_Buttons[4]->SetString(STRING->FindString("Move").c_str());
	m_Buttons[4]->SetOnRelease(&KeyBindingOnClick[4]);

	m_Buttons[5]->SetString(STRING->FindString("Attack").c_str());
	m_Buttons[5]->SetOnRelease(&KeyBindingOnClick[5]);

	m_Buttons[6]->SetString(STRING->FindString("Stop").c_str());
	m_Buttons[6]->SetOnRelease(&KeyBindingOnClick[6]);

	m_Buttons[7]->SetString(STRING->FindString("Stove").c_str());
	m_Buttons[7]->SetOnRelease(&KeyBindingOnClick[7]);

	m_Buttons[8]->SetString(STRING->FindString("Rally Point").c_str());
	m_Buttons[8]->SetOnRelease(&KeyBindingOnClick[8]);

	m_Buttons[9]->SetString(STRING->FindString("Cookbook").c_str());
	m_Buttons[9]->SetOnRelease(&KeyBindingOnClick[9]);

	m_Buttons[10]->SetString(STRING->FindString("Back").c_str());
	m_Buttons[10]->SetOnRelease(&BackButtonOnClick);

	for(unsigned char i = 0; i < 11; ++i)
	{
		m_Buttons[i]->FitToText();
		if(i > 0)
		{
			if(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10 + m_Buttons[i]->GetSizeY() > 668.0f)
			{
				ColumnX += 300;
				m_Buttons[i]->SetPosition(ColumnX, 200.0f);
			}
			else
				m_Buttons[i]->SetPosition(ColumnX, m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
		}
		m_ObjectManager.AddObjectToLayer(0, m_Buttons[i]);
	}

	m_nSelection = static_cast<EMenu>(0);
	SetSelection(MIN);

	m_bIsChangingBind = false;
}

CState_KeyBinding::~CState_KeyBinding()
{

}

bool CState_KeyBinding::Input()
{
	if(m_bIsChangingBind)
	{
		unsigned char nKeyPressed = 0;

		nKeyPressed = CSGD_DirectInput::GetInstance()->GetDIKCode();

		if(nKeyPressed != 0 && nKeyPressed != 28)
		{
			KeyBindingOnClick[GetSelection()].SetKeyPressed(nKeyPressed);
			KeyBindingOnClick[GetSelection()]();
			m_bIsChangingBind = false;
		}
	}
	else
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
			case BACK:
				BackButtonOnClick();
				break;
			default:
				KeyBindingOnClick[GetSelection()]();
				break;
			}
		}
	}

	return true;
}

void CState_KeyBinding::Update(float _delta)
{

}

void CState_KeyBinding::Draw()
{
	CSGD_Direct3D::GetInstance()->Clear();

	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/FoodFight_Menu.png")->Draw(D3DXVECTOR2(0.0f, -250.0f));

	DrawGUIFrame(this, D3DXVECTOR2(125.0f, 100.0f), D3DXVECTOR2(600.0f, 568.0f));

	BITMAP->DrawStringSprite("Resources/Data/Font.spr",CStringTable::GetInstance()->FindString("Set Key Bindings").c_str(), 150.0f, 125.0f, 1.5f, 1.5f);

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT ViewRectangle;
	ViewRectangle.left = 0;
	ViewRectangle.right = 1024;
	ViewRectangle.top = 0;
	ViewRectangle.bottom = 768;
	m_ObjectManager.DrawAllObjects(Transform, ViewRectangle);

	for(unsigned char i = 0; i < 10; ++i)
	{
		BITMAP->DrawStringSprite("Resources/Data/Font.spr", PROFILE_MANAGER->GetStringFromDIK(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[static_cast<eKeyBindings>(i)]), m_Buttons[i]->GetPositionX() + m_Buttons[i]->GetSizeX() + 25.0f, m_Buttons[i]->GetPositionY(), 0.75f, 0.75f, 0, i == GetSelection() && IsBinding() ? D3DCOLOR_XRGB(255, 127, 127) : D3DCOLOR_XRGB(255, 255, 255));
	}
}

CState_KeyBinding::EMenu CState_KeyBinding::GetSelection() const
{
	return m_nSelection;
}

void CState_KeyBinding::SetSelection(CState_KeyBinding::EMenu Selection)
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

void CState_KeyBinding::CKeyBindingOnClickFunctor::operator()()
{
	m_State->SetSelection(static_cast<EMenu>(m_Key));
	if(m_State->IsBinding())
		PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[m_Key] = m_KeyPressed;
	m_State->ToggleBinding();
}

void CState_KeyBinding::CKeyBindingOnClickFunctor::SetState(CState_KeyBinding* State)
{
	m_State = State;
}

void CState_KeyBinding::CKeyBindingOnClickFunctor::SetKeyPressed(unsigned char KeyPressed)
{
	m_KeyPressed = KeyPressed;
}

void CState_KeyBinding::CKeyBindingOnClickFunctor::SetKeyBinding(eKeyBindings Key)
{
	m_Key = Key;
}

void CState_KeyBinding::CBackButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PopState();
}

bool CState_KeyBinding::IsBinding()
{
	return m_bIsChangingBind;
}

void CState_KeyBinding::ToggleBinding()
{
	m_bIsChangingBind = !m_bIsChangingBind;
}