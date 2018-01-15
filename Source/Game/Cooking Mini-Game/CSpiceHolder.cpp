//////////////////////////////////////////////////////
// File: "CSpiceHolder.h"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#include "CSpiceHolder.h"
#include "../../Managers/Bad Chef/CObjectFactory.h"
#include "../Objects/GUI Objects/CCursor.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../Objects/Appliances/CStove.h"

CSpiceHolder::CSpiceHolder(RESOURCE_TYPE _type)
{
	m_SpawnedResource = _type;
}

void CSpiceHolder::OnClick()
{
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BUG-022
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	if(m_StoveOwner->GetTeamValue() == TEAM_PLAYER)
	{
		CCookingResource* newRsc = NULL;
		switch(m_SpawnedResource)
		{
		case RESOURCE_SALT:
			newRsc = static_cast<CCookingResource*>(CObjectFactory::GetInstance()->Construct("Salt", this->GetObjectManagerOwner()));
			break;
		case RESOURCE_PEPPER:
			newRsc = static_cast<CCookingResource*>(CObjectFactory::GetInstance()->Construct("Pepper", this->GetObjectManagerOwner()));
			break;
		case RESOURCE_SUGAR:
			newRsc = static_cast<CCookingResource*>(CObjectFactory::GetInstance()->Construct("Sugar", this->GetObjectManagerOwner()));
			break;
		case RESOURCE_CINNA:
			newRsc = static_cast<CCookingResource*>(CObjectFactory::GetInstance()->Construct("Cinna", this->GetObjectManagerOwner()));
			break;
		}

		newRsc->SetStoveOwner(m_StoveOwner);
		CCursor::GetInstance()->OverrideDragObject(newRsc);
	}
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BUG-022
	//////////////////////////////////////////////////////////////////////////
}


IObject* CreateSaltHolderFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/WhiteShaker.spr");
	if(!Sprite)
		return NULL;

	CSpiceHolder* newRsc = new CSpiceHolder(RESOURCE_SALT);
	newRsc->SetSprite(Sprite);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

IObject* CreatePepperHolderFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/BlackShaker.spr");
	if(!Sprite)
		return NULL;

	CSpiceHolder* newRsc = new CSpiceHolder(RESOURCE_PEPPER);
	newRsc->SetSprite(Sprite);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

IObject* CreateSugarHolderFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/WhiteShaker.spr");
	if(!Sprite)
		return NULL;

	CSpiceHolder* newRsc = new CSpiceHolder(RESOURCE_SUGAR);
	newRsc->SetSprite(Sprite);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

IObject* CreateCinnaHolderFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/BlackShaker.spr");
	if(!Sprite)
		return NULL;

	CSpiceHolder* newRsc = new CSpiceHolder(RESOURCE_CINNA);
	newRsc->SetSprite(Sprite);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

void CSpiceHolder::OnBeginHover()
{
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BUG-022
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	if(m_StoveOwner->GetTeamValue() == TEAM_PLAYER)
	{
		IHoverable::OnBeginHover();
		CCursor::GetInstance()->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/EmptySpoon.spr"));
	}
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BUG-022
	//////////////////////////////////////////////////////////////////////////
}

void CSpiceHolder::OnEndHover()
{
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BUG-022
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	if(m_StoveOwner->GetTeamValue() == TEAM_PLAYER)
	{
		CCursor::GetInstance()->ResetSprite();
		IHoverable::OnEndHover();
	}
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BUG-022
	//////////////////////////////////////////////////////////////////////////
}