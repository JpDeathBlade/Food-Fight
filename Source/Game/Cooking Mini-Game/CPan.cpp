//////////////////////////////////////////////////////
// File: "CPan.cpp"
// Creator: MG
// Creation Date: 9/20/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CPan.h"
#include "CCookingUnit.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/CObjectFactory.h"
#include "../../Managers/Bad Chef/Messages/CMessageSystem.h"
#include "../../Managers/Bad Chef/Messages/Messages.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/Bad Chef/Particles/CParticleManager.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"
#include "../Objects/GUI Objects/CCursor.h"
#include "../Objects/Appliances/CStove.h"
#include "../Objects/GUI Objects/CFloatingText.h"

CPan::CPan()
{
	m_StageTimer = 10.0f;
	m_PanStage = PAN_NONE;
	m_ActiveRecipe = NULL;
	m_ActiveAction = NULL;
	m_CookingUnit = NULL;
	m_Stove = NULL;
	m_DrawColor = D3DCOLOR_ARGB(255,255,255,255);
	SetCollideable(false);
	m_pSmoke = CParticleManager::GetInstance()->GetParticleEffect("Smoke");
	m_pSmoke->InitParticles();
}

CPan::~CPan()
{
	if(m_pSmoke)
	{
		delete m_pSmoke;
		m_pSmoke = NULL;
	}

}

void CPan::AddResource(CCookingResource* _type)
{
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/SizzleA.wav", "Effects")->Play(GetPosition());

	_type->SetUsed(true);
	if(_type->GetResourceType() == RESOURCE_SALT ||
		_type->GetResourceType() == RESOURCE_PEPPER ||
		_type->GetResourceType() == RESOURCE_SUGAR ||
		_type->GetResourceType() == RESOURCE_CINNA)
	{
		if(m_PanStage != PAN_NONE)
		{
			m_ActiveSpices.push_back(_type->GetResourceType());
		}
		_type->DoRemove();
	}
	else
	{
		resourcesAdded.push_back(_type);
		if(m_PanStage == PAN_NONE)
		{
			m_PanStage = PAN_COMBINE;
			m_StageTimer = 10.0f;
		}
		else if(m_PanStage != PAN_COMBINE)
		{
			this->GetStove()->AddCookingResource(_type->GetResourceType(), this->GetObjectManagerOwner());
			_type->DoRemove();
		}
	}
}

void CPan::Update(float Delta)
{
	CGameObject::Update(Delta);
	m_StageTimer -= Delta;

	//m_ActiveAction = NULL;

	if(m_PanStage == PAN_COMBINE)
	{
		if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/LightSizzling.wav", "Effects")->IsPlaying())
			CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/LightSizzling.wav", "Effects")->Play(GetPosition(), D3DXVECTOR2(0, 0), -1);

		m_DrawColor = D3DCOLOR_ARGB(255,255,(int)(255+(m_StageTimer*5)-50),(int)(255+(m_StageTimer*5)-50));
		for(unsigned int i = 0; i < resourcesAdded.size(); i++)
		{
			//Distance greater than 40
			if(sqrt(pow(resourcesAdded[i]->GetPositionX() - this->GetPositionX(),2) + pow(resourcesAdded[i]->GetPositionY() - this->GetPositionY(),2)) > 40)
			{
				D3DXVECTOR2 direction = this->GetPosition() - resourcesAdded[i]->GetPosition();
				D3DXVECTOR2 directionNorm;
				D3DXVec2Normalize(&directionNorm, &direction);

				resourcesAdded[i]->SetVelocity(directionNorm.x*500, directionNorm.y*500);
			}
			else
			{
				resourcesAdded[i]->SetVelocity(0, 0);
			}	
		}
	}

	if(m_StageTimer <= 0 && m_PanStage == PAN_COMBINE)
	{
		//Get counts of all resources
		int eggs = 0;
		int meat = 0;
		int wheat = 0;
		int fruit = 0;
		for(unsigned int i = 0; i < resourcesAdded.size(); i++)
		{
			if(resourcesAdded[i]->GetResourceType() == RESOURCE_EGG)
			{
				eggs++;
			}
			else if(resourcesAdded[i]->GetResourceType() == RESOURCE_MEAT)
			{
				meat++;
			}
			else if(resourcesAdded[i]->GetResourceType() == RESOURCE_WHEAT)
			{
				wheat++;
			}
			else if(resourcesAdded[i]->GetResourceType() == RESOURCE_FRUIT)
			{
				fruit++;
			}
			resourcesAdded[i]->DoRemove();
		}
		resourcesAdded.clear();

		//If there is a successful recipe match
		CRecipe* testRecipe = NULL;
		if(testRecipe = CRecipeManager::GetInstance()->TryRecipe(eggs, meat, wheat, fruit, 0, 0, 0))
		{
			m_ActiveRecipe = testRecipe;
			SetPanStage(PAN_COOKING);

			m_StageTimer = m_ActiveRecipe->GetCookTime();

			m_CookingUnit = static_cast<CCookingUnit*>(CObjectFactory::GetInstance()->Construct("CookingUnit", this->GetObjectManagerOwner()));
			m_CookingUnit->SetPosition(this->GetPositionX(), this->GetPositionY());
			m_CookingUnit->SetCreationString(m_ActiveRecipe->GetString());
			m_CookingUnit->SetPanOnwer(this);
			if(m_ActiveRecipe->GetCookActions()->size() == 0)
				m_CookingUnit->SetActionDone();

		}
		else
		{
			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BUG-019
			// BUG FIX START
			/////////////////////////////////////////////////////////////////
			CFloatingText::SpawnFloatingText(this->GetObjectManagerOwner(), "     Bad Recipe!\nCheck the Cookbook", D3DXVECTOR2(this->GetPositionX()-100, this->GetPositionY()-30), D3DXVECTOR2(0,0), 1.6f, 0.8f, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BUG-019
			//////////////////////////////////////////////////////////////////////////
			//SetPanStage(PAN_NONE);
			Reset(); // NOTE: Testing this out. - SA
		}
	}

	if(m_PanStage == PAN_COOKING && m_ActiveRecipe)
	{
		if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/SizzleB.wav", "Effects")->IsPlaying())
			CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/SizzleB.wav", "Effects")->Play(GetPosition(), D3DXVECTOR2(0, 0), -1);
		for(unsigned int i = 0; i < m_ActiveRecipe->GetCookActions()->size(); i++)
		{
			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BUG-005
			// BUG FIX START
			/////////////////////////////////////////////////////////////////
			if(fabs(m_StageTimer - (*m_ActiveRecipe->GetCookActions())[i]->GetActionTime()) < 3 && !m_CookingUnit->GetActionDone() && !m_ActiveAction)
			{
				m_ActiveAction = (*m_ActiveRecipe->GetCookActions())[i];
					//Drawing a button will go here
					char buffer[32];
					sprintf_s(buffer, 32, "%s", m_ActiveAction->GetActionText().c_str());
					CFloatingText::SpawnFloatingText(this->GetObjectManagerOwner(), buffer, D3DXVECTOR2(this->GetPositionX() - 6 - (strlen(buffer)*6), this->GetPositionY()), D3DXVECTOR2(0,-15), m_ActiveAction->GetActionTime()-2, 1, 0, D3DCOLOR_ARGB(255,255,255,255), false);
			}
			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BUG-005
			//////////////////////////////////////////////////////////////////////////
		}

		if(m_StageTimer < 0)
		{
			//Spawn a unit here
			SetPanStage(PAN_DONE);
			CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/TimerDing.wav", "Effects")->Play(GetPosition());

			//Need to pass in the pan pointer so it can notify the pan that the cookingunit was moved from the pan
			//CMessageSystem::GetInstance()->AddMessage((IMessage*)(new CCreateCookingUnitMessage(this->GetObjectManagerOwner(), "CookingUnit", this->GetPositionX(), this->GetPositionY(), m_ActiveRecipe->GetString())));
		}	
	}
	if(m_StageTimer < -5 && m_PanStage == PAN_DONE)
	{
		if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/Fire.wav", "Effects")->IsPlaying())
				CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/Fire.wav", "Effects")->Play(GetPosition(), D3DXVECTOR2(0, 0), -1);

		m_pSmoke->SetPosition(GetPosition() - D3DXVECTOR2((float)(m_pSmoke->GetRectWidth()/2), (float)(m_pSmoke->GetRectHeight()/2)));
		if(!m_pSmoke->GetIsPlaying())
			m_pSmoke->Play();
	}

	m_pSmoke->Update(Delta);
}

void CPan::Draw()
{
	DrawAnimation(GetPosition(), GetRotation(), GetScale(), m_DrawColor);

	if(m_PanStage == PAN_COMBINE || m_PanStage == PAN_COOKING)
	{
		//Drawing a font for a timer will go here
		char buffer[32];
		sprintf_s(buffer, 32, "%.1f", m_StageTimer);
		CTextManager::GetInstance()->DrawStringWorld("Resources/Graphics/Fonts/Font.png", buffer, this->GetPositionX() - 32, this->GetPositionY()+50, 1, 1, 0, D3DXCOLOR(255,255,255,255));
	}

	m_pSmoke->Draw();
}

void CPan::OnClick()
{
	
}

void CPan::Reset()
{
	m_DrawColor = D3DCOLOR_ARGB(255,255,255,255);
	m_ActiveRecipe = NULL;
	m_ActiveAction = NULL;
	resourcesAdded.clear();
	m_ActiveSpices.clear();
	m_CookingUnit = NULL;
	SetPanStage(PAN_NONE);
	m_pSmoke->Stop();
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/LightSizzling.wav", "Effects")->StopPlaying();
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/SizzleB.wav", "Effects")->StopPlaying();
	CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Cooking Mini-Game/Fire.wav", "Effects")->StopPlaying();
}

CCookingUnit* CPan::GetCookingUnit()
{
	return m_CookingUnit;
}

IObject* CreatePanFactoryMethod(CObjectManager* objMan)
{
	// Find resource plate sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/Pan.spr");
	if(!Sprite)
		return NULL;

	CPan* newPan = new CPan();
	newPan->SetSprite(Sprite);
	objMan->AddObjectToLayer(2, newPan);
	return newPan;
}

CStove* CPan::GetStove() const
{
	return m_Stove;

}
void CPan::SetStove(CStove* const Stove)
{
	m_Stove = Stove;
}

void CPan::SetPanStage(EPanStage _stage)
{
	m_PanStage = _stage;
	if(_stage == PAN_DONE)
	{
		m_CookingUnit->SetFinished(true);
		ReachedDoneStage.Trigger(this);
	}
}

void CPan::OnBeginHover()
{
	if(m_Stove->GetTeamValue() == TEAM_PLAYER)
	{
		IHoverable::OnBeginHover();
		CCursor::GetInstance()->SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/SpatulaCursor.spr"));
	}
}

void CPan::OnEndHover()
{
	if(m_Stove->GetTeamValue() == TEAM_PLAYER)
	{
		CCursor::GetInstance()->ResetSprite();
		IHoverable::OnEndHover();
	}
}