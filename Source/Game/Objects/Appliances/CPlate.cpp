//////////////////////////////////////////////////////
// File: "CPlate.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: 10/1/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CPlate.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include "../Units/CUnitObject.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../Base/CCamera.h"
#include "../Tiles/CTileStage.h"
#include "CStove.h"
#include "../../../Managers/Bad Chef/Audio/CAudioManager.h"
#include "../GUI Objects/CFloatingText.h"

CPlate::CPlate(void)
{
	m_nWheat			= 0;
	m_nEgg				= 0;
	m_nMeat				= 0;
	m_nFruit			= 0;
	m_unPlayerThreshold	= 0;
	m_unEnemyThreshold	= 0;
	m_unCapturePoints	= 0;
	m_fCaptureRange		= 0.0f;
	m_fAccumulator		= 0.0f;
	m_pCaptureBar		= CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/ff_CaptureBar.png");
	m_pOutline			= CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Appliances/PlateOutline.spr");
	m_ePreviousTeam		= TEAM_NONE;
	m_PlayerCapture		= CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/PlayerCapture.wav", "User Interface");
	m_EnemyCapture		= CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/EnemyCaptured.wav", "User Interface");
	m_PlayerLost		= CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/PlayerPlateLost.wav", "User Interface");;
	m_EnemyLost			= CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/EnemyPlateLost.wav", "User Interface");;
}

CPlate::~CPlate(void)
{
}

void CPlate::Update(float Delta)
{
	CGameObject::Update(Delta);

	m_fAccumulator += Delta;

	if(m_fAccumulator >= 1.0f)
		this->GetObjectManagerOwner()->IterateLayer(1, &CPlate::CheckCapturingUnits, this);

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BUG-017
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	if(m_unCapturePoints < m_unPlayerThreshold)
	{
		if(m_ePreviousTeam != TEAM_PLAYER)
		{
			m_PlayerCapture->Play();
			CFloatingText::SpawnFloatingText(this->GetObjectManagerOwner(), "Plate Captured!", GetPosition(), D3DXVECTOR2(10.0f, -40.0f), 3.0f, 0.7f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), false);
		}

		SetTeamValue(TEAM_PLAYER);

		m_ePreviousTeam = TEAM_PLAYER;
	}
	else if(m_unCapturePoints > m_unEnemyThreshold)
	{
		if(m_ePreviousTeam != TEAM_ENEMY)
		{
			m_EnemyCapture->Play();
			CFloatingText::SpawnFloatingText(this->GetObjectManagerOwner(), "Plate Captured!", GetPosition(), D3DXVECTOR2(10.0f, -40.0f), 3.0f, 0.7f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), false);
		}

		SetTeamValue(TEAM_ENEMY);

		m_ePreviousTeam = TEAM_ENEMY;
	}
	else
	{
		if(m_ePreviousTeam != TEAM_NONE)
		{
			if(m_ePreviousTeam == TEAM_PLAYER)
				m_PlayerLost->Play();
			else if(m_ePreviousTeam == TEAM_ENEMY)
				m_EnemyLost->Play();

			CFloatingText::SpawnFloatingText(this->GetObjectManagerOwner(), "Plate Lost!", GetPosition(), D3DXVECTOR2(10.0f, -40.0f), 3.0f, 0.7f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), false);
		}

		SetTeamValue(TEAM_NONE);

		m_ePreviousTeam = TEAM_NONE;
	}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BBUG-017
	//////////////////////////////////////////////////////////////////////////


	static float fTimer = 0.0f;

	fTimer += Delta;

	if(fTimer >= 45.0f)
	{
		std::vector<CStove*>& vTemp = CTileStage::GetInstance()->GetStoves();

		for(unsigned int i = 0; i < vTemp.size(); ++i)
		{
			if(vTemp[i]->GetTeamValue() == GetTeamValue())
			{
				if(m_nWheat > 0)
				{
					vTemp[i]->AddCookingResource(RESOURCE_WHEAT, this->GetObjectManagerOwner());
					RemoveWheat();
				}
				if(m_nEgg > 0)
				{
					vTemp[i]->AddCookingResource(RESOURCE_EGG, this->GetObjectManagerOwner());
					RemoveEgg();
				}
				if(m_nMeat > 0)
				{
					vTemp[i]->AddCookingResource(RESOURCE_MEAT, this->GetObjectManagerOwner());
					RemoveMeat();
				}
				if(m_nFruit > 0)
				{
					vTemp[i]->AddCookingResource(RESOURCE_FRUIT, this->GetObjectManagerOwner());
					RemoveFruit();
				}
			}
		}
		fTimer = 0.0f;
	}
}

void CPlate::Draw(void)
{
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BUG-018
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	if(GetTeamValue() != TEAM_NONE)
	{
		if(GetTeamValue() == TEAM_PLAYER)
			m_pOutline->Draw(GetPosition() - D3DXVECTOR2(100, 62), 0, D3DXVECTOR2(1, 1), D3DCOLOR_XRGB(0, 0, 255));
		else
			m_pOutline->Draw(GetPosition() - D3DXVECTOR2(100, 62), 0, D3DXVECTOR2(1, 1), D3DCOLOR_XRGB(255, 0, 0));
	}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BUG-018
	//////////////////////////////////////////////////////////////////////////


	CGameObject::Draw();

	m_pCaptureBar->Draw(GetPosition() + D3DXVECTOR2(-48, -64));

	float fPercentage = (float)m_unCapturePoints / ((float)m_unEnemyThreshold + m_unPlayerThreshold);

	float fToAdd;
	
	if(fPercentage >= 0.001f)
		fToAdd = 64 * fPercentage;
	else
		fToAdd = 0.0f;

	CSGD_Direct3D::GetInstance()->DrawLine(GetPosition() + D3DXVECTOR2(-48.0f + fToAdd, -64) - D3DXVECTOR2(CAMERA->GetPositionX(), CAMERA->GetPositionY()), GetPosition() + D3DXVECTOR2(-48.0f + fToAdd, -48) - D3DXVECTOR2(CAMERA->GetPositionX(), CAMERA->GetPositionY()), 5.0f);
	//CSGD_Direct3D::GetInstance()->DrawLine(GetPosition() + D3DXVECTOR2(65.0f + fToAdd, 32) - D3DXVECTOR2(CAMERA->GetPositionX(), CAMERA->GetPositionY()), GetPosition() + D3DXVECTOR2(65.0f + fToAdd, 48) - D3DXVECTOR2(CAMERA->GetPositionX(), CAMERA->GetPositionY()), 5.0f);
}

void CPlate::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{
}


/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BUG-006
// BUG FIX START
/////////////////////////////////////////////////////////////////
float CPlate::GetZDepth() const
{
	return 0;
}
///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BUG-006
//////////////////////////////////////////////////////////////////////////


bool CPlate::CheckCapturingUnits(IObject* pObject, CPlate* pCaller)
{
	CPlate* pPlate = dynamic_cast<CPlate*>(pCaller);

	if(pPlate)
	{
		CUnitObject* pUnit = dynamic_cast<CUnitObject*>(pObject);

		if(pUnit)
		{
			D3DXVECTOR2 vDistance;
			D3DXVec2Subtract(&vDistance, &pPlate->GetPosition(), &pUnit->GetPosition());
			float fDistance = D3DXVec2Length(&vDistance);

			if(fDistance <= pPlate->GetCaptureRange())
			{
				if(pUnit->GetTeamValue() == TEAM_ENEMY)
					pPlate->AddPoints();
				else if(pUnit->GetTeamValue() == TEAM_PLAYER)
					pPlate->RemovePoints();
			}
		}
	}
	return false;
}

void CPlate::AddPoints(int nNumAdd)
{
	if(m_unCapturePoints < (m_unEnemyThreshold + m_unPlayerThreshold))
		m_unCapturePoints += nNumAdd;
}

void CPlate::RemovePoints(int nNumRemove)
{
	if(m_unCapturePoints > 0)
		m_unCapturePoints -= nNumRemove;
}

IObject* CreatePlateFactoryMethod(CObjectManager* objManager)
{
	CSprite* Sprite = CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Appliances/Plate.spr");
	if(!Sprite)
		return NULL;

	CPlate* pNewPlate = new CPlate();
	pNewPlate->SetSprite(Sprite);
	pNewPlate->SetCapturePoints(4000);
	pNewPlate->SetCaptureRange(250.0f);
	pNewPlate->SetEnemyThreshold(6000);
	pNewPlate->SetPlayerThreshold(2000);
	objManager->AddObjectToLayer(1, pNewPlate);
	return pNewPlate;
}