//////////////////////////////////////////////////////
// File: "CStove.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CStove.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../../Managers/Bad Chef/CObjectFactory.h"
#include "../../Cooking Mini-Game/CCookingResource.h"
#include "../../Cooking Mini-Game/CPan.h"
#include "../../Cooking Mini-Game/CCookingUnit.h"
#include "../../Cooking Mini-Game/CSpiceHolder.h"
#include "../../../CGame.h"
#include "../../States/CState_EndGame.h"
#include "../GUI Objects/CCursor.h"

CStove::CStove()
{
	m_FruitCount = 0;
	m_MeatCount = 0;
	m_EggCount = 0;
	m_WheatCount = 0;

	m_EggPlate = NULL;
	m_MeatPlate = NULL;
	m_FruitPlate = NULL;
	m_WheatPlate = NULL;

	m_TopLeftPan = NULL;
	m_TopRightPan = NULL;
	m_BottomLeftPan = NULL;
	m_BottomRightPan = NULL;

	m_SpawnRect.top = 0;
	m_SpawnRect.bottom = 0;
	m_SpawnRect.left = 0;
	m_SpawnRect.right = 0;

	SetMaxHealth(1000);
	SetHealth(1000);

	SetSharedID(0);
	SetNWOwner(true);

	m_bAlive = true;
}

CStove::~CStove()
{
	m_TopLeftPan->ReleaseReference();
	m_TopRightPan->ReleaseReference();
	m_BottomRightPan->ReleaseReference();
	m_BottomLeftPan->ReleaseReference();
}

void CStove::InitializeCookingMinigame(bool _facingRight, CObjectManager* objMan)
{
	m_EggPlate = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct("ResourcePlate", objMan));
	m_MeatPlate = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct("ResourcePlate", objMan));
	m_FruitPlate = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct("ResourcePlate", objMan));
	m_WheatPlate = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct("ResourcePlate", objMan));
	CSpiceHolder* newObjs = NULL;
	if(_facingRight)
	{
		this->Translate(128, 184);
		//Placing plates from left to right
		m_FruitPlate->SetPosition(this->GetPositionX() - 128 + 80, this->GetPositionY() - 184 + 59);
		m_MeatPlate->SetPosition(this->GetPositionX() + 32 + 80, this->GetPositionY() - 184 + 59);
		m_EggPlate->SetPosition(this->GetPositionX() + 192 + 80, this->GetPositionY() - 184 + 59);
		m_WheatPlate->SetPosition(this->GetPositionX() + 352 + 80, this->GetPositionY() - 184 + 59);
		newObjs = static_cast<CSpiceHolder*>(CObjectFactory::GetInstance()->Construct("SaltHolder", objMan));
		newObjs->SetPosition(this->GetPositionX() - 128, this->GetPositionY() + 192);
		newObjs->SetStoveOwner(this);
		newObjs = static_cast<CSpiceHolder*>(CObjectFactory::GetInstance()->Construct("PepperHolder", objMan));
		newObjs->SetPosition(this->GetPositionX() - 64, this->GetPositionY() + 192);
		newObjs->SetStoveOwner(this);
		newObjs = static_cast<CSpiceHolder*>(CObjectFactory::GetInstance()->Construct("SugarHolder", objMan));
		newObjs->SetPosition(this->GetPositionX() - 128, this->GetPositionY() + 256);
		newObjs->SetStoveOwner(this);
		newObjs = static_cast<CSpiceHolder*>(CObjectFactory::GetInstance()->Construct("CinnaHolder", objMan));
		newObjs->SetPosition(this->GetPositionX() - 64, this->GetPositionY() + 256);
		newObjs->SetStoveOwner(this);
		this->Translate(0, -32);
	}
	else
	{
		this->Translate(0, 184);
		//Placing plates from left to right
		m_FruitPlate->SetPosition(this->GetPositionX() + 80, this->GetPositionY() - 184 + 59);
		m_MeatPlate->SetPosition(this->GetPositionX() + 160 + 80, this->GetPositionY() - 184 + 59);
		m_EggPlate->SetPosition(this->GetPositionX() + 320 + 80, this->GetPositionY() - 184 + 59);
		m_WheatPlate->SetPosition(this->GetPositionX() + 480 + 80, this->GetPositionY() - 184 + 59);
		newObjs = static_cast<CSpiceHolder*>(CObjectFactory::GetInstance()->Construct("SaltHolder", objMan));
		newObjs->SetPosition(this->GetPositionX() + 512, this->GetPositionY() + 192);
		newObjs->SetStoveOwner(this);
		newObjs = static_cast<CSpiceHolder*>(CObjectFactory::GetInstance()->Construct("PepperHolder", objMan));
		newObjs->SetPosition(this->GetPositionX() + 512 + 64, this->GetPositionY() + 192);
		newObjs->SetStoveOwner(this);
		newObjs = static_cast<CSpiceHolder*>(CObjectFactory::GetInstance()->Construct("SugarHolder", objMan));
		newObjs->SetPosition(this->GetPositionX() + 512, this->GetPositionY() + 256);
		newObjs->SetStoveOwner(this);
		newObjs = static_cast<CSpiceHolder*>(CObjectFactory::GetInstance()->Construct("CinnaHolder", objMan));
		newObjs->SetPosition(this->GetPositionX() + 512 + 64, this->GetPositionY() + 256);
		newObjs->SetStoveOwner(this);
		this->Translate(0, -32);
	}

	m_TopLeftPan = static_cast<CPan*>(CObjectFactory::GetInstance()->Construct("Pan", objMan));
	m_TopLeftPan->SetStove(this);
	m_TopLeftPan->SetPosition(this->GetCurrentFrame()->GetAnchorPoint("topLeft")->GetAnchorPoint().x + this->GetPositionX(), this->GetCurrentFrame()->GetAnchorPoint("topLeft")->GetAnchorPoint().y + this->GetPositionY());
	m_TopLeftPan->AcquireReference();

	m_TopRightPan = static_cast<CPan*>(CObjectFactory::GetInstance()->Construct("Pan", objMan));
	m_TopRightPan->SetStove(this);
	m_TopRightPan->SetPosition(this->GetCurrentFrame()->GetAnchorPoint("topRight")->GetAnchorPoint().x + this->GetPositionX(), this->GetCurrentFrame()->GetAnchorPoint("topRight")->GetAnchorPoint().y + this->GetPositionY());
	m_TopRightPan->AcquireReference();

	m_BottomRightPan = static_cast<CPan*>(CObjectFactory::GetInstance()->Construct("Pan", objMan));
	m_BottomRightPan->SetStove(this);
	m_BottomRightPan->SetPosition(this->GetCurrentFrame()->GetAnchorPoint("bottomRight")->GetAnchorPoint().x + this->GetPositionX(), this->GetCurrentFrame()->GetAnchorPoint("bottomRight")->GetAnchorPoint().y + this->GetPositionY());
	m_BottomRightPan->AcquireReference();

	m_BottomLeftPan = static_cast<CPan*>(CObjectFactory::GetInstance()->Construct("Pan", objMan));
	m_BottomLeftPan->SetStove(this);
	m_BottomLeftPan->SetPosition(this->GetCurrentFrame()->GetAnchorPoint("bottomLeft")->GetAnchorPoint().x + this->GetPositionX(), this->GetCurrentFrame()->GetAnchorPoint("bottomLeft")->GetAnchorPoint().y + this->GetPositionY());
	m_BottomLeftPan->AcquireReference();

	//Give starter resources
	for(int i = 0; i < 2; i++)
	{
		this->AddCookingResource(RESOURCE_WHEAT, objMan);
		this->AddCookingResource(RESOURCE_EGG, objMan);
	}

	this->AddCookingResource(RESOURCE_WHEAT, objMan);
	this->AddCookingResource(RESOURCE_WHEAT, objMan);
}

void CStove::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{/*
	CCookingUnit* testCook = NULL;
	if(testCook = dynamic_cast<CCookingUnit*>(Object))
	{
		//DO NOTHING :)
	}
	else
	{
		//Also do nothing?
		//CGameObject::OnCollision(Object, Overlap);
	}*/
}

void CStove::AddCookingResource(RESOURCE_TYPE _type, CObjectManager* objMan)
{
	CCookingResource* cookingRsc = NULL;
	switch(_type)
	{
	case RESOURCE_EGG:
		if(m_EggResources.size() < 15)
		{
			cookingRsc = static_cast<CCookingResource*>(CObjectFactory::GetInstance()->Construct("ResourceEgg", objMan));
			cookingRsc->SetPosition(m_EggPlate->GetPositionX() + (rand()%120)-60, m_EggPlate->GetPositionY() + (rand()%100)-50);
			m_EggResources.push_back(cookingRsc);
			m_EggCount++;
		}
		break;
	case RESOURCE_MEAT:
		if(m_MeatResources.size() < 15)
		{
			cookingRsc = static_cast<CCookingResource*>(CObjectFactory::GetInstance()->Construct("ResourceMeat", objMan));
			cookingRsc->SetPosition(m_MeatPlate->GetPositionX() + (rand()%120)-60, m_MeatPlate->GetPositionY() + (rand()%100)-50);
			m_MeatResources.push_back(cookingRsc);
			m_MeatCount++;
		}
		break;
	case RESOURCE_WHEAT:
		if(m_WheatResources.size() < 15)
		{
			cookingRsc = static_cast<CCookingResource*>(CObjectFactory::GetInstance()->Construct("ResourceWheat", objMan));
			cookingRsc->SetPosition(m_WheatPlate->GetPositionX() + (rand()%120)-60, m_WheatPlate->GetPositionY() + (rand()%100)-50);
			m_WheatResources.push_back(cookingRsc);
			m_WheatCount++;
		}
		break;
	case RESOURCE_FRUIT:
		if(m_FruitResources.size() < 15)
		{
			cookingRsc = static_cast<CCookingResource*>(CObjectFactory::GetInstance()->Construct("ResourceFruit", objMan));
			cookingRsc->SetPosition(m_FruitPlate->GetPositionX() + (rand()%120)-60, m_FruitPlate->GetPositionY() + (rand()%100)-50);
			m_FruitResources.push_back(cookingRsc);
			m_FruitCount++;
		}
		break;
	}
	if(cookingRsc)
		cookingRsc->SetStoveOwner(this);
}

void CStove::SubtractCookingResource(RESOURCE_TYPE _type)
{
	switch(_type)
	{
	case RESOURCE_EGG:
		m_EggCount--;
		break;
	case RESOURCE_MEAT:
		m_MeatCount--;
		break;
	case RESOURCE_WHEAT:
		m_WheatCount--;
		break;
	case RESOURCE_FRUIT:
		m_FruitCount--;
		break;
	}
}

CGameObject* CStove::GetPlate(RESOURCE_TYPE _type)
{
	switch(_type)
	{
	case RESOURCE_EGG:
		return m_EggPlate;
		break;
	case RESOURCE_MEAT:
		return m_MeatPlate;
		break;
	case RESOURCE_WHEAT:
		return m_WheatPlate;
		break;
	case RESOURCE_FRUIT:
		return m_FruitPlate;
		break;
	}
	return NULL;
}

void CStove::Update(float dt)
{
	if(m_bAlive && GetHealth() <= 0)
	{
		m_bAlive = false;
		if(this->GetTeamValue() == TEAM_ENEMY)
			CGame::GetInstance()->PushState(new CState_EndGame(true));
		else
			CGame::GetInstance()->PushState(new CState_EndGame(false));
	}
}

IObject* CreateStoveFactoryMethod(CObjectManager* objManager)
{
	// Find stove sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/Stove.spr");
	if(!Sprite)
		return NULL;

	CStove* newStove = new CStove();
	newStove->SetSprite(Sprite);
	objManager->AddObjectToLayer(1, newStove);
	return newStove;
}

IObject* CreateResourcePlateFactoryMethod(CObjectManager* objManager)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourcePlate.spr");
	if(!Sprite)
		return NULL;

	CGameObject* newPlate = new CGameObject();
	newPlate->SetSprite(Sprite);
	newPlate->SetCollideable(false);
	objManager->AddObjectToLayer(1, newPlate);
	return newPlate;
}

CCookingResource* CStove::GetEgg()
{
	if(m_EggResources.empty())
		return NULL;
	CCookingResource* CookingResource = m_EggResources.front();
	m_EggResources.pop_front();
	return CookingResource;
}

CCookingResource* CStove::GetFruit()
{
	if(m_FruitResources.empty())
		return NULL;
	CCookingResource* CookingResource = m_FruitResources.front();
	m_FruitResources.pop_front();
	return CookingResource;
}

CCookingResource* CStove::GetMeat()
{
	if(m_MeatResources.empty())
		return NULL;
	CCookingResource* CookingResource = m_MeatResources.front();
	m_MeatResources.pop_front();
	return CookingResource;
}

CCookingResource* CStove::GetWheat()
{
	if(m_WheatResources.empty())
		return NULL;
	CCookingResource* CookingResource = m_WheatResources.front();
	m_WheatResources.pop_front();
	return CookingResource;
}

void CStove::OnBeginHover()
{
	if(GetTeamValue() == TEAM_PLAYER)
	{
		IHoverable::OnBeginHover();
		CCursor::GetInstance()->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/SpatulaCursor.spr"));
	}
}

void CStove::OnEndHover()
{
	if(GetTeamValue() == TEAM_PLAYER)
	{
		CCursor::GetInstance()->ResetSprite();
		IHoverable::OnEndHover();
	}
}

RECT CStove::GetSpawnRect()
{
	RECT spawnRect;
	switch(m_SpawnRect.left)
	{
	case -1:
		spawnRect.left = (LONG)(this->GetPositionX() - 150);
		spawnRect.right = (LONG)(this->GetPositionX() - 20);
		break;
	case 0:
		spawnRect.left = (LONG)(this->GetPositionX() + 200);
		spawnRect.right = (LONG)(this->GetPositionX() + 330);
		break;
	case 1:
		spawnRect.right = (LONG)(this->GetPositionX() + 680);
		spawnRect.left = (LONG)(this->GetPositionX() + 550);
		break;
	}

	switch(m_SpawnRect.top)
	{
	case -1:
		spawnRect.top = (LONG)(this->GetPositionY() - 300);
		spawnRect.bottom = (LONG)(this->GetPositionY() - 170);
		break;
	case 0:
		spawnRect.top = (LONG)(this->GetPositionY() + 150);
		spawnRect.bottom = (LONG)(this->GetPositionY() + 280);
		break;
	case 1:
		spawnRect.bottom = (LONG)(this->GetPositionY() + 560);
		spawnRect.top = (LONG)(this->GetPositionY() + 430);
		break;
	}

	return spawnRect;
}