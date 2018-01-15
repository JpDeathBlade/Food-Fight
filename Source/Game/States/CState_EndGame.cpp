//////////////////////////////////////////////////////
// File: "CState_EndGame.cpp"
// Creator: MG
// Creation Date: 10/05/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////

#include "CState_EndGame.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"
#include "CState_MainMenu.h"
#include "../../CGame.h"
#include "../Objects/GUI Objects/CButton.h"

CState_EndGame::CState_EndGame(bool _win)
{
	m_bWinner = _win;

	if(m_bWinner)
		sprintf_s(m_Text, 24, STRING->FindString("You Win!").c_str());
	else
		sprintf_s(m_Text, 24, STRING->FindString("You Lose...").c_str());

	m_EndTimer = 5;
	CAudioManager::GetInstance()->LoadSound("Resources/Sounds/Music/Finale.wav", "Music", false, true);

	CSprite* ButtonSprite = CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/Button.spr");
	SetSprite(ButtonSprite);

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	m_ExitButton = new CButton;
	m_ExitButton->SetPosition(512.0f, 400.0f);
	m_ExitButton->SetFontFileName("Resources/Data/Font.spr");
	m_ExitButton->SetFontScale(0.75f, 0.75f);
	m_ExitButton->SetString("Exit");
	m_ExitButton->SetSprite(ButtonSprite);
	m_ExitButton->SetStringOffset(10, 10);
	m_ExitButton->FitToText();
	m_ExitButton->TranslateX(-m_ExitButton->GetSizeX() * 0.5f);
	m_ExitButton->SetOnClick(&ExitButtonOnClick);

	m_ObjectManager.AddObjectToLayer(0, m_ExitButton);
}

CState_EndGame::~CState_EndGame()
{

}

void CState_EndGame::Update(float dt)
{
	m_EndTimer -= dt;
	if(m_EndTimer < 0)
		ExitButtonOnClick();

	m_ObjectManager.UpdateAllObjects(dt);
}

void CState_EndGame::Draw()
{
	DrawGUIFrame(this, D3DXVECTOR2(300, 300), D3DXVECTOR2(424, 180));

	CTextManager::GetInstance()->DrawStringSprite("Resources/Data/Font.spr", m_Text, 350, 310, 2, 2);

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT ViewRectangle;
	ViewRectangle.left = 0;
	ViewRectangle.right = 1024;
	ViewRectangle.top = 0;
	ViewRectangle.bottom = 768;
	m_ObjectManager.DrawAllObjects(Transform, ViewRectangle);
}

void CState_EndGame::OnEnter()
{
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Music/Finale.wav", "Music")->Play();
}

void CState_EndGame::OnExit()
{
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Music/Finale.wav", "Music")->StopPlaying();
}

void CState_EndGame::CExitButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->ChangeState(new CState_MainMenu);
}