#include "CSecondaryResource.h"


CSecondaryResource::CSecondaryResource(RESOURCE_TYPE _type)
{
	m_SpawnedResource = _type;
	m_ResourceToDrag = NULL;
}


void CSecondaryResource::OnClick()
{
	
}

/*
IObject* CreateMilkCartonFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/ResourceEgg.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

IObject* CreateButterTrayFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Data/ResourceEgg.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}
*/