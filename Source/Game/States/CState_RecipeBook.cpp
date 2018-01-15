#include "CState_RecipeBook.h"
#include "../../CGame.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Game/Cooking Mini-Game/CRecipeManager.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include "../../Game/Objects/GUI Objects/CCursor.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"

CState_RecipeBook::CState_RecipeBook()
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/Cooking Mini-Game/RecipeBook.spr");

	m_RecipeManager = CRecipeManager::GetInstance();
	m_RecipeManager->SetSprite(SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/RecipeBook.spr"));
	m_RecipeManager->SetPosition(512, 384);
}

CState_RecipeBook::~CState_RecipeBook()
{

}

void CState_RecipeBook::OnEnter()
{
	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	m_ObjectManager.AddObjectToLayer(0, m_RecipeManager);
}

bool CState_RecipeBook::Input()
{
	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[LEFT]) ||
		XBOX->GetController(0)->IsPressed_DPAD_LEFT())
		m_RecipeManager->FlipLeft();
	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[RIGHT]) ||
		XBOX->GetController(0)->IsPressed_DPAD_RIGHT())
		m_RecipeManager->FlipRight();
	if(CSGD_DirectInput::GetInstance()->KeyReleasedEx(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[COOKBOOK]) ||
		XBOX->GetController(0)->IsPressed_Y())
		CGame::GetInstance()->PopState();
	return true;
}

void CState_RecipeBook::Update(float Delta)
{
	m_ObjectManager.UpdateAllObjects(Delta);
}

void CState_RecipeBook::Draw()
{
	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT View;
	SetRect(&View,0, 0, 1024, 768);
	m_ObjectManager.DrawAllObjects(Transform, View);
}
