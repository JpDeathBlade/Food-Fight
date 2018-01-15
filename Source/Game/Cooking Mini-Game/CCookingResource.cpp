#include "CCookingResource.h"
#include "../Objects/GUI Objects/CCursor.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../Objects/Appliances/CStove.h"
#include "CPan.h"
#include "../Objects/Units/CStatusEffect.h"
#include "../Objects/Units/CUnitObject.h"
#include "../Objects/Units/CSkilledUnitObject.h"
#include "../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include "../../Managers/Bad Chef/Networking/NetworkUpdateClasses.h"
#include "../Objects/GUI Objects/CCursor.h"

bool StartSlowEffect(CUnitObject* _unit)
{
	//Slow the unit down
	_unit->AddMovementMultiply(-0.5);
	return true;
}

bool EndSlowEffect(CUnitObject* _unit)
{
	//Slow the unit down
	_unit->AddMovementMultiply(0.5);
	return true;
}

bool StartStunEffect(CUnitObject* _unit)
{
	//Slow the unit down
	_unit->AddMovementMultiply(-1.0);
	return true;
}

bool EndStunEffect(CUnitObject* _unit)
{
	//Slow the unit down
	_unit->AddMovementMultiply(1.0);
	return true;
}

bool DropResourceOnUnit(IObject* obj, CCookingResource* _resource)
{
	return DropResourceOnUnit(obj, _resource, false);
}

bool DropResourceOnUnit(IObject* obj, CCookingResource* _resource, bool fromPacket)
{
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(obj))
	{
		RECT overlap;
		if(fromPacket || IntersectRect(&overlap, &testUnit->GetCollisionRectangle(), &_resource->GetCollisionRectangle()))
		{
			switch(_resource->GetResourceType())
			{
			case RESOURCE_EGG:
				{
					CStatusEffect* slowEffect = new CStatusEffect;
					slowEffect->Begin = StartSlowEffect;
					slowEffect->End = EndSlowEffect;
					slowEffect->SetTimer(10);
					testUnit->AddStatusEffect(slowEffect);

					//Pin an alpha'd egg sprite


					NW_MANAGER_START
						if(CNetworkManager::GetInstance()->IsConnected())
						{
							CNetworkManager::GetInstance()->SendDropResourcePacket((int)_resource->GetResourceType(), testUnit->GetSharedID(), testUnit->GetNWOwner());
						}
						NW_MANAGER_END
				}
				break;
			case RESOURCE_MEAT:
				{
					CSkilledUnitObject* skilledObj = NULL;
					if(skilledObj = dynamic_cast<CSkilledUnitObject*>(testUnit))
					{
						skilledObj->LevelUp();
						//Play particle effect

						NW_MANAGER_START
							if(CNetworkManager::GetInstance()->IsConnected())
							{
								CNetworkManager::GetInstance()->SendDropResourcePacket((int)_resource->GetResourceType(), skilledObj->GetSharedID(), skilledObj->GetNWOwner());
							}
							NW_MANAGER_END
					}
					return true;
				}
				break;
			case RESOURCE_FRUIT:
				{
					testUnit->SetHealth(testUnit->GetMaxHealth());
					//Play particle effect

					NW_MANAGER_START
						if(CNetworkManager::GetInstance()->IsConnected())
						{
							CNetworkManager::GetInstance()->SendDropResourcePacket((int)_resource->GetResourceType(), testUnit->GetSharedID(), testUnit->GetNWOwner());
						}
						NW_MANAGER_END
				}
				break;
			case RESOURCE_WHEAT:
				{
					CStatusEffect* stunEffect = new CStatusEffect;
					stunEffect->Begin = StartStunEffect;
					stunEffect->End = EndStunEffect;
					stunEffect->SetTimer(4);
					testUnit->AddStatusEffect(stunEffect);
					//Play particle effect

					NW_MANAGER_START
						if(CNetworkManager::GetInstance()->IsConnected())
						{
							CNetworkManager::GetInstance()->SendDropResourcePacket((int)_resource->GetResourceType(), testUnit->GetSharedID(), testUnit->GetNWOwner());
						}
						NW_MANAGER_END
				}
				break;
			}
		}
		//do not return true :) (unless it is one unit ONLY!)
	}
	return false;
}

CCookingResource::CCookingResource()
{
	m_bUsed = false;
	m_StoveOwner = NULL;
	m_bDropped = false;
	SetCollideable(false);
}

CCookingResource::CCookingResource(const CCookingResource& Source) : CGameObject(Source), IDraggable(Source)
{

}

CCookingResource& CCookingResource::operator=(const CCookingResource& Source)
{
	CGameObject::operator=(Source);
	IDraggable::operator=(Source);
	return *this;
}

CCookingResource::~CCookingResource()
{

}

void CCookingResource::OnBeginDrag()
{
	switch(GetResourceType())
	{
	case RESOURCE_SUGAR:
		CCursor::GetInstance()->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/SugarSpoon.spr"));
		break;
	case RESOURCE_CINNA:
		CCursor::GetInstance()->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/CinnamonSpoon.spr"));
		break;
	default:
		return;
	}
}

void CCookingResource::OnDrag()
{
	if(!m_bUsed)
	{
		SetPosition(CCursor::GetInstance()->GetPosition());
	}
}

void CCookingResource::OnEndDrag()
{
	if(!m_bUsed)
	{
		IDraggable::OnEndDrag();

		RECT catchRect;
		RECT panRect;
		RECT thisRect = this->GetCollisionRectangle();


		if(m_RscType == RESOURCE_FRUIT || 
			m_RscType == RESOURCE_MEAT ||
			m_RscType == RESOURCE_WHEAT ||
			m_RscType == RESOURCE_EGG)
		{
			panRect = m_StoveOwner->GetPlate(this->GetResourceType())->GetCollisionRectangle();
			if(IntersectRect(&catchRect, &panRect, &thisRect))
			{
				//Do nothing, it was placed back on the plate
				return;
			}
		}

		this->SetAnimationType(IAnimable::PLAY_ONCE);

		panRect = m_StoveOwner->GetTopLeftPan()->GetCollisionRectangle();
		if(IntersectRect(&catchRect, &panRect, &thisRect))
		{
			m_StoveOwner->GetTopLeftPan()->AddResource(this);
			m_StoveOwner->SubtractCookingResource(this->GetResourceType());
			return;
		}

		panRect = m_StoveOwner->GetTopRightPan()->GetCollisionRectangle();
		if(IntersectRect(&catchRect, &panRect, &thisRect))
		{
			m_StoveOwner->GetTopRightPan()->AddResource(this);
			m_StoveOwner->SubtractCookingResource(this->GetResourceType());
			return;
		}

		panRect = m_StoveOwner->GetBottomLeftPan()->GetCollisionRectangle();
		if(IntersectRect(&catchRect, &panRect, &thisRect))
		{
			m_StoveOwner->GetBottomLeftPan()->AddResource(this);
			m_StoveOwner->SubtractCookingResource(this->GetResourceType());
			return;
		}

		panRect = m_StoveOwner->GetBottomRightPan()->GetCollisionRectangle();
		if(IntersectRect(&catchRect, &panRect, &thisRect))
		{
			m_StoveOwner->GetBottomRightPan()->AddResource(this);
			m_StoveOwner->SubtractCookingResource(this->GetResourceType());
			return;
		}

		//If we reach here, the player is dropping the resource on the field
		m_StoveOwner->SubtractCookingResource(this->GetResourceType());		//This line will not do anything to any resources that are not primary

		//This function will find any colliding units with the drop to apply an effect
		this->GetObjectManagerOwner()->IterateLayer<CCookingResource>(1 , &DropResourceOnUnit, this);

		if(this->GetResourceType() == RESOURCE_EGG)
		{
			this->SetAnimationType(IAnimable::PLAY_ONCE);
			m_bDropped = true;
		}
		else
		{
			this->DoRemove();
		}
	}

	CCursor::GetInstance()->ResetSprite();
}

void CCookingResource::OnEndAnimate()
{
	if(m_bDropped)
		this->DoRemove();
}

//Creating the FRUIT resource
IObject* CreateFruitFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceFruit.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetResourceType(RESOURCE_FRUIT);
	if(rand() % 2 == 0)
	{
		newRsc->SetCurrentAnimation("Orange");
	}
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

//Creating the MEAT resource
IObject* CreateMeatFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceMeat.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetResourceType(RESOURCE_MEAT);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

//Creating the WHEAT resource
IObject* CreateWheatFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceWheat.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetResourceType(RESOURCE_WHEAT);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

//Creating the EGG resource
IObject* CreateEggFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceEgg.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetAnimationType(IAnimable::PLAY_STATIC);
	newRsc->SetResourceType(RESOURCE_EGG);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

//Creating the MILK resource?
IObject* CreateMilkFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceEgg.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetResourceType(RESOURCE_MILK);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

//Creating the BUTTER resource?
IObject* CreateButterFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceWheat.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetResourceType(RESOURCE_BUTTER);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

//Creating the SALT resource
IObject* CreateSaltFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceEgg.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetResourceType(RESOURCE_SALT);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

//Creaing the PEPPER resource
IObject* CreatePepperFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceWheat.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetResourceType(RESOURCE_PEPPER);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

//Creating the SUGAR resource
IObject* CreateSugarFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceEgg.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetResourceType(RESOURCE_SUGAR);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

//Creating the CINNA resource
IObject* CreateCinnaFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceWheat.spr");
	if(!Sprite)
		return NULL;

	CCookingResource* newRsc = new CCookingResource();
	newRsc->SetSprite(Sprite);
	newRsc->SetResourceType(RESOURCE_CINNA);
	objManager->AddObjectToLayer(3, newRsc);
	return newRsc;
}

void CCookingResource::OnBeginHover()
{
	if(this->GetStoveOwner()->GetTeamValue() == TEAM_PLAYER)
	{
		IHoverable::OnBeginHover();
		CCursor::GetInstance()->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/SpatulaCursor.spr"));
	}
}

void CCookingResource::OnEndHover()
{
	if(this->GetStoveOwner()->GetTeamValue() == TEAM_PLAYER)
	{
		CCursor::GetInstance()->ResetSprite();
		IHoverable::OnEndHover();
	}
}

float CCookingResource::GetZDepth() const
{
	if(GetResourceType() == RESOURCE_CINNA || GetResourceType() == RESOURCE_SUGAR || GetResourceType() == RESOURCE_SALT || GetResourceType() == RESOURCE_PEPPER)
		return FLT_MAX;
	else
		return CGameObject::GetZDepth();
}