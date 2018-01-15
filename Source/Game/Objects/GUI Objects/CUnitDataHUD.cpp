#include "CUnitDataHUD.h"
#include "../Units/CUnitObject.h"
#include "../Units/CSkilledUnitObject.h"
#include "../Units/CStatusEffect.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../Base/CCamera.h"
#include "CSelectionHUD.h"

CUnitDataHUD* CUnitDataHUD::m_Instance = NULL;

CUnitDataHUD::CUnitDataHUD()
{
	CSprite* Sprite = CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/Button.spr");
	m_UnitTooltip.SetSprite(Sprite);
	m_UnitTooltip.SetSize(192, 128);
	m_UnitTooltip.SetFontFileName("Resources/Data/Font.spr");
	//m_UnitTooltip.SetFontScale(0.8, 0.8);
}

CUnitDataHUD::~CUnitDataHUD()
{
	m_Instance = NULL;
}

void CUnitDataHUD::OnHover()
{
	CUnitObject* UnitObject = CSelectionHUD::GetInstance()->GetSubSelected();
	if(UnitObject)
	{
		m_UnitTooltip.SetPosition(CCursor::GetInstance()->GetPositionX() - CAMERA->GetPositionX(), CCursor::GetInstance()->GetPositionY() - CAMERA->GetPositionY() - m_UnitTooltip.GetSizeY());
		char tooltipText[128];
		sprintf_s(tooltipText, 128, "%s:%d\n%s:%d\n%s:%.1f", CStringTable::GetInstance()->FindString("Attack").c_str(), UnitObject->GetAttack(), CStringTable::GetInstance()->FindString("Defense").c_str(), UnitObject->GetDefense(), CStringTable::GetInstance()->FindString("Evade").c_str(), UnitObject->GetEvade());
		m_UnitTooltip.SetString(tooltipText);
	}
}

CUnitDataHUD* CUnitDataHUD::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CUnitDataHUD();
	return m_Instance;
}

void CUnitDataHUD::DeleteInstance()
{
	delete m_Instance;
}

void CUnitDataHUD::Draw()
{
	D3DXVECTOR2 Size;
	Size.x = Width;
	Size.y = Height;

	DrawGUIFrame(this, GetPosition(), Size);

	CUnitObject* UnitObject = CSelectionHUD::GetInstance()->GetSubSelected();
	if(UnitObject)
	{
		// TODO: Draw the sprite with limbs.
		UnitObject->GetSprite()->Draw(0, 0, GetPosition() + D3DXVECTOR2(Width * 0.5f, Height * 0.5f));

		// Draw LERP health bar.
		float HealthPercentage = static_cast<float>(UnitObject->GetHealth()) / UnitObject->GetMaxHealth();

		D3DCOLOR Color = D3DCOLOR_XRGB(static_cast<unsigned char>((1 - HealthPercentage) * 255), static_cast<unsigned char>(HealthPercentage * 255), 0);
		CSGD_Direct3D::GetInstance()->DrawLine(GetPosition() + D3DXVECTOR2(8, Height - 8), GetPosition() + D3DXVECTOR2(((Width - 8) * HealthPercentage), Height - 8), 4, Color);

		//Draw Status effects
		int nextX = (int)(this->GetPositionX() + 116-4);
		int nextY = (int)(this->GetPositionY() + 112-4-24);
		for(std::list<CStatusEffect*>::iterator Iterator = UnitObject->GetStatusEffects()->begin(); Iterator != UnitObject->GetStatusEffects()->end(); ++Iterator)
		{
			if((*Iterator)->GetSprite())
			{
				nextX -= 27;
				(*Iterator)->GetSprite()->Draw(D3DXVECTOR2((float)nextX, (float)nextY));
				if(nextX <= 4)
				{
					nextX = 116-4;
					nextY -= 27;
				}
			}
		}

		CSkilledUnitObject* SkilledUnitObject = dynamic_cast<CSkilledUnitObject*>(UnitObject);

		if(SkilledUnitObject)
		{
			char buffer[128];
			strcpy_s(buffer, 128, CStringTable::GetInstance()->FindString("Level").c_str());
			sprintf_s(buffer,sizeof(buffer),"%s: %d", buffer, SkilledUnitObject->GetLevel());
			BITMAP->DrawString("Resources/Graphics/Fonts/Font.png", buffer, GetPositionX() + 10, GetPositionY() + 80, 0.4f, 0.4f);

			sprintf_s(buffer,sizeof(buffer),"EXP: %d",SkilledUnitObject->GetExperience());
			BITMAP->DrawString("Resources/Graphics/Fonts/Font.png", buffer, GetPositionX() + 10, GetPositionY() + 100, 0.4f, 0.4f);
		}

		//Draw a tooltip
		if(IsBeingHoveredOver())
			m_UnitTooltip.Draw();
	}
}

RECT CUnitDataHUD::GetDrawRectangle() const
{
	RECT DrawRectangle;
	DrawRectangle.left = (LONG)GetPositionX();
	DrawRectangle.right = DrawRectangle.left + Width;
	DrawRectangle.top = (LONG)GetPositionY();
	DrawRectangle.bottom = DrawRectangle.top + Height;
	return DrawRectangle;
}

RECT CUnitDataHUD::GetCollisionRectangle() const
{
	RECT CollisionRectangle;
	CollisionRectangle.left = (LONG)GetPositionX();
	CollisionRectangle.right = CollisionRectangle.left + Width;
	CollisionRectangle.top = (LONG)GetPositionY();
	CollisionRectangle.bottom = CollisionRectangle.top + Height;
	return CollisionRectangle;
}

IObject* CUnitDataHUD::CreateUnitDataHUD(CObjectManager* ObjectManager)
{
	CSprite* Sprite = CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/Button.spr");
	if(!Sprite)
		return NULL;

	CUnitDataHUD* UnitDataHUD = CUnitDataHUD::GetInstance();
	UnitDataHUD->SetSprite(Sprite);
	UnitDataHUD->SetPosition(128, 640);

	ObjectManager->AddObjectToLayer(4, UnitDataHUD);
	return UnitDataHUD;
}