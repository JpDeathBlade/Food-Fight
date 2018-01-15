//////////////////////////////////////////////////////
// File: "CCookingUnit.cpp"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#include "CCookingUnit.h"
#include "CPan.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include "../../Game/Objects/Appliances/CStove.h"
#include "../../Game/Objects/Units/CUnitObject.h"
#include "../../Game/Objects/Units/CStatusEffect.h"
#include "../../Game/Objects/GUI Objects/CCursor.h"
#include "CFlipAction.h"
#include "../../Game/Objects/GUI Objects/CFloatingText.h"

bool StartSpeedEffect(CUnitObject* _unit);
bool EndSpeedEffect(CUnitObject* _unit);

CCookingUnit::CCookingUnit()
{
	bDone = false;
	bActionDone = false;
	m_FlipAction = NULL;
	m_ScrambleTimer = 0.0f;
}

void CCookingUnit::OnClick()
{

}

void CCookingUnit::OnBeginDrag()
{
	if(m_PanOwner->GetActiveAction() && !this->GetActionDone())
	{
		if(m_PanOwner->GetActiveAction()->GetActionText() == "Flip")
		{
			IDraggable::OnBeginDrag();
			StartFlip();
		}
		else if(m_PanOwner->GetActiveAction()->GetActionText() == "Scramble")
		{
			m_ScrambleTimer = 1.0f;
			SetActionDone();
		}
	}
	if(bDone)
	{
		SpawnUnit();
	}
}

void CCookingUnit::OnEndDrag()
{
	IDraggable::OnEndDrag();
	if(m_FlipAction)
	{
		m_FlipAction->FinishFlip();
	}
}

void CCookingUnit::OnBeginHover()
{
	if(this->GetPanOwner()->GetStove()->GetTeamValue() == TEAM_PLAYER)
	{
		IHoverable::OnBeginHover();
		CCursor::GetInstance()->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/SpatulaCursor.spr"));
	}
}

void CCookingUnit::OnEndHover()
{
	if(this->GetPanOwner()->GetStove()->GetTeamValue() == TEAM_PLAYER)
	{
		CCursor::GetInstance()->ResetSprite();
		IHoverable::OnEndHover();
	}
}

void CCookingUnit::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{
	// No collision.
}

void CCookingUnit::EndFlip()
{
	if(m_FlipAction)
	{
		delete m_FlipAction;
		m_FlipAction = NULL;
	}
	RECT overlap;
	if(IntersectRect(&overlap, &this->GetCollisionRectangle(), &m_PanOwner->GetCollisionRectangle()))
	{
		this->SetActionDone();
		this->SetVelocity(0,0);
	}
	else
	{
/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BUG-022
// BUG FIX START
/////////////////////////////////////////////////////////////////
		m_PanOwner->Reset();
		this->DoRemove();
///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BUG-022
//////////////////////////////////////////////////////////////////////////
	}
}

void CCookingUnit::Update(float dt)
{
	if(m_FlipAction)
	{
		m_FlipAction->Update(dt);
	}

	if(m_ScrambleTimer > 0.0f)
	{
		m_ScrambleTimer -= dt;
		this->SetPosition(m_PanOwner->GetPosition());
		this->Translate((float)(rand()%60) - 30, (float)(rand()%40) - 20);
	}

	CGameObject::Update(dt);
}

void CCookingUnit::Draw()
{
	if(m_FlipAction)
	{
		m_FlipAction->Draw();
	}
	else
	{
		StrictDraw();
	}
}

void CCookingUnit::SetCreationString(std::string _create)
{
	m_CreationString = _create;
	this->SetSprite(GetImageFromCreationString(_create));
}

void CCookingUnit::StrictDraw()
{
	CGameObject::Draw();
}

void CCookingUnit::StartFlip()
{
	m_FlipAction = new CFlipAction;
	m_FlipAction->SetCookingUnit(this);
	m_FlipAction->SetStartFlip(CCursor::GetInstance()->GetPositionX(), CCursor::GetInstance()->GetPositionY());
}

CUnitObject* CCookingUnit::SpawnUnit()
{
	RECT spawnRect = m_PanOwner->GetStove()->GetSpawnRect();
	int x = spawnRect.left + (rand()%(spawnRect.right - spawnRect.left));
	int y = spawnRect.top + (rand()%(spawnRect.bottom - spawnRect.top));

	CUnitObject* SpawnedUnit = static_cast<CUnitObject*>(CObjectFactory::GetInstance()->Construct(this->GetCreationString(), this->GetObjectManagerOwner()));
	SpawnedUnit->SetPosition((float)x, (float)y);
	SpawnedUnit->SetTeamValue(m_PanOwner->GetStove()->GetTeamValue());
	SpawnedUnit->SetCreationString(GetCreationString());
	float modValue = m_PanOwner->GetStageTimer() - 5;
	modValue = modValue / 150;

	modValue = 1.0f + modValue;

	if(bActionDone == false)
	{
		modValue -= 0.2f;
	}

	modValue += 0.03f;

	if(modValue > 1.0f)
		modValue = 1.0f;

	if(modValue < 0.9)
		SpawnedUnit->ChangeAvailableSkills(2);
	if(modValue < 0.75)
		SpawnedUnit->ChangeAvailableSkills(1);
	if(modValue < 0.5)
	{
		SpawnedUnit->ChangeAvailableSkills(0);
		modValue = 0.5;
	}
#if _DEBUG
	printf("Unit %s created with modValue of: %.2f\n", SpawnedUnit->GetCreationString().c_str(), modValue);
#endif
	if(modValue < 0.95f)
		SpawnedUnit->ScaleStats(modValue);

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BUG-023
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	//Change stats based on spices
	if(m_PanOwner->GetActiveRecipe()->GetMaxSpices() < m_PanOwner->GetActiveSpices()->size())
	{
		CStatusEffect* overspiceEffect = new CStatusEffect();
		overspiceEffect->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Data/Status Effects/OverspiceStatus.spr"));
		SpawnedUnit->AddStatusEffect(overspiceEffect);
		CFloatingText::SpawnFloatingText(this->GetObjectManagerOwner(), "Overspiced!", D3DXVECTOR2(this->GetPositionX()-100, this->GetPositionY()-32), D3DXVECTOR2(0,0), 1.6f, 1, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	else
	{
		if(m_PanOwner->GetActiveRecipe()->GetMaxSpices() == m_PanOwner->GetActiveSpices()->size())
		{
			CFloatingText::SpawnFloatingText(this->GetObjectManagerOwner(), "Perfectly Spiced!", D3DXVECTOR2(this->GetPositionX()-130, this->GetPositionY()-50), D3DXVECTOR2(0,0), 1.6f, 0.8f, 0, D3DCOLOR_ARGB(255, 30, 255, 30), false);
		}
		for(unsigned int i = 0; i < m_PanOwner->GetActiveSpices()->size(); i++)
		{
			switch((*m_PanOwner->GetActiveSpices())[i])
			{
			case RESOURCE_SALT:
				SpawnedUnit->SetDefense(SpawnedUnit->GetDefense() + 1);
				break;
			case RESOURCE_PEPPER:
				SpawnedUnit->SetEvade(SpawnedUnit->GetEvade() * 1.1f);
				break;
			case RESOURCE_SUGAR:
				SpawnedUnit->SetMovementSpeed((unsigned short)(SpawnedUnit->GetMovementSpeed() * 1.2f));
				break;
			case RESOURCE_CINNA:
				SpawnedUnit->SetAttackSpeed((unsigned short)(SpawnedUnit->GetAttackSpeed() * 1.2f));
				break;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BUG-023
	//////////////////////////////////////////////////////////////////////////


	NW_MANAGER_START
		if(CNetworkManager::GetInstance()->IsConnected())
		{
			CNetworkManager::GetInstance()->SendCreateUnitPacket(SpawnedUnit, (char*)this->GetCreationString().c_str());
		}
		NW_MANAGER_END

			m_PanOwner->Reset();

		this->DoRemove();

		return SpawnedUnit;
}

IObject* CreateCookingUnitFactoryMethod(CObjectManager* objMan)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceFruit.spr");
	if(!Sprite)
		return NULL;

	CCookingUnit* newUnit = new CCookingUnit();
	newUnit->SetSprite(Sprite);
	objMan->AddObjectToLayer(3, newUnit);
	return newUnit;
}