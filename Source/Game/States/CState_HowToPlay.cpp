#include "CState_HowToPlay.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../../CGame.h"
#include "../Objects/GUI Objects/CButton.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"

CState_HowToPlay::CState_HowToPlay()
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	CSprite* ButtonSprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");
	SetSprite(ButtonSprite);

	m_BackButton = new CButton();
	m_BackButton->SetSprite(ButtonSprite);
	m_BackButton->SetPosition(200.0f, 550.0f);
	m_BackButton->SetFontFileName("Resources/Data/Font.spr");
	m_BackButton->SetStringOffset(10, 10);
	m_BackButton->SetString(STRING->FindString("Back").c_str());
	m_BackButton->SetFontScale(0.75f, 0.75f);
	m_BackButton->SetTextColor(D3DCOLOR_XRGB(255, 127, 127));
	m_BackButton->SetOnRelease(&BackButtonOnClick);
	m_BackButton->FitToText();
	m_ObjectManager.AddObjectToLayer(0, m_BackButton);
}

CState_HowToPlay::~CState_HowToPlay()
{

}

bool CState_HowToPlay::Input()
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || XBOX->GetController(0)->IsPressed_A())
	{
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/ButtonClick.wav", "User Interface")->Play();
		BackButtonOnClick();
	}
	return true;
}

void CState_HowToPlay::Update(float _delta)
{

}

void CState_HowToPlay::Draw()
{
	CSGD_Direct3D::GetInstance()->Clear();

	DrawGUIFrame(this, D3DXVECTOR2(50.0f, 100.0f), D3DXVECTOR2(400.0f, 500.0f));

	BITMAP->DrawStringSprite("Resources/Data/Font.spr",STRING->FindString("How To Play").c_str(),75.0f,125.0f);

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT ViewRectangle;
	ViewRectangle.left = 0;
	ViewRectangle.right = 1024;
	ViewRectangle.top = 0;
	ViewRectangle.bottom = 768;
	m_ObjectManager.DrawAllObjects(Transform, ViewRectangle);
}

void CState_HowToPlay::CBackButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PopState();
}