#include "CState_Credits.h"
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

CState_Credits::CState_Credits()
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	CSprite* ButtonSprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");
	SetSprite(ButtonSprite);

	m_BackButton = new CButton();
	m_BackButton->SetSprite(ButtonSprite);
	m_BackButton->SetPosition(200.0f, 625.0f);
	m_BackButton->SetFontFileName("Resources/Data/Font.spr");
	m_BackButton->SetStringOffset(10, 10);
	m_BackButton->SetString(STRING->FindString("Back").c_str());
	m_BackButton->SetFontScale(0.75f, 0.75f);
	m_BackButton->SetTextColor(D3DCOLOR_XRGB(255, 127, 127));
	m_BackButton->SetOnRelease(&BackButtonOnClick);
	m_BackButton->FitToText();
	m_ObjectManager.AddObjectToLayer(0, m_BackButton);
}

CState_Credits::~CState_Credits()
{

}

bool CState_Credits::Input()
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || XBOX->GetController(0)->IsPressed_A())
	{
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/ButtonClick.wav", "User Interface")->Play();
		BackButtonOnClick();
	}
	return true;
}

void CState_Credits::Update(float _delta)
{

}

void CState_Credits::Draw()
{
	CSGD_Direct3D::GetInstance()->Clear();

	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/FoodFight_Menu.png")->Draw(D3DXVECTOR2(0.0f, -250.0f));

	DrawGUIFrame(this, D3DXVECTOR2(50.0f, 100.0f), D3DXVECTOR2(800.0f, 600.0f));

	BITMAP->DrawStringSprite("Resources/Data/Font.spr",STRING->FindString("Credits").c_str(),75.0f,125.0f);

	BITMAP->DrawStringSprite("Resources/Data/Font.spr",
		"Executive Producers:\n  - Dustin Clingman\n  - Keyvan Acosta\nAssociate Producer:\n  - John O'Leske\nTeam Bad Chef:\n  - Andy Bauman\n  - Joe Stuhr\n  - Mike Gassman\n  - Saul Abreu\nArt Team:\n  - Chris Jahosky\n  - Richard Levins",
		150.0f,225.0f,0.8f,0.8f);

	BITMAP->DrawStringSprite("Resources/Data/Font.spr",
		"Test Team: Bad Brass-\n  - Nathan Gillikin\n  - Anthony Edwards\n  - Bryant Bickel\nVoices:\n  - Adam Amos\n  - Andy Bauman\n  - Nathan Gillikin\n  - Eric Mason\n  - Peppy\n  - Joe Stuhr\n",
		500.0f,225.0f,0.8f,0.8f);

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT ViewRectangle;
	ViewRectangle.left = 0;
	ViewRectangle.right = 1024;
	ViewRectangle.top = 0;
	ViewRectangle.bottom = 768;
	m_ObjectManager.DrawAllObjects(Transform, ViewRectangle);
}

void CState_Credits::CBackButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PopState();
}