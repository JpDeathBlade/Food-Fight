//////////////////////////////////////////////////////
// File: "CCoffeeMaker.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: 9/21/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CCoffeeMaker.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"

CCoffeeMaker::CCoffeeMaker(void)
{
}

CCoffeeMaker::~CCoffeeMaker(void)
{
}

void CCoffeeMaker::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{
}

IObject* CreateCoffeeMakerFactoryMethod(CObjectManager* pManager)
{
	CSprite* Sprite = CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Appliances/CoffeeMaker.spr");
	if(!Sprite)
		return NULL;

	CCoffeeMaker* pNewCoffeeMaker = new CCoffeeMaker();
	pNewCoffeeMaker->SetSprite(Sprite);
	pManager->AddObjectToLayer(1, pNewCoffeeMaker);
	return pNewCoffeeMaker;
}