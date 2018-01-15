#include "CSelectionHUD.h"
#include "CCursor.h"
#include "../Units/CUnitObject.h"
#include "../../Cooking Mini-Game/CRecipe.h"
#include "../Base/CCamera.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"

// CUnitIcon //

CUnitIcon::CUnitIcon()
{
	m_UnitObject = NULL;
}

CUnitIcon::CUnitIcon(const CUnitIcon& Source) : CGUIObject(Source)
{
	SetSelectedUnit(Source.GetSelectedUnit());
}

CUnitIcon& CUnitIcon::operator=(const CUnitIcon& Source)
{
	CGUIObject::operator=(Source);
	SetSelectedUnit(Source.GetSelectedUnit());
	return *this;
}

CUnitIcon::~CUnitIcon()
{

}

void CUnitIcon::Draw()
{
	if(m_UnitObject)
	{
		CGUIObject::Draw();
		m_UnitObject->GetSprite()->Draw(0, 0, GetPosition() + D3DXVECTOR2(44, 64), 0, D3DXVECTOR2(0.5,0.5));

		// Draw health bars.
		float HealthPercentage = static_cast<float>(m_UnitObject->GetHealth()) / m_UnitObject->GetMaxHealth();

		D3DCOLOR Color = D3DCOLOR_XRGB(static_cast<unsigned char>((1 - HealthPercentage) * 255), static_cast<unsigned char>(HealthPercentage * 255), 0);
		CSGD_Direct3D::GetInstance()->DrawLine(GetPosition() + D3DXVECTOR2(5, Height - 5), GetPosition() + D3DXVECTOR2((Width - 5) * HealthPercentage, Height - 5), 2.0f, Color);
	}
}

CUnitObject* CUnitIcon::GetSelectedUnit() const
{
	return m_UnitObject;
}

void CUnitIcon::SetSelectedUnit(CUnitObject* const UnitObject)
{
	if(m_UnitObject)
		m_UnitObject->ReleaseReference();
	m_UnitObject = UnitObject;
	if(m_UnitObject)
		m_UnitObject->AcquireReference();
}

// CSelectionHUD //

CSelectionHUD* CSelectionHUD::m_Instance = NULL;

CSelectionHUD::CSelectionHUD()
{
	m_SubSelected = NULL;
}

CSelectionHUD::~CSelectionHUD()
{
	if(m_SubSelected)
		m_SubSelected->ReleaseReference();

	CCursor::GetInstance()->SelectionChanged -= &OnSelectionChange;

	m_Instance = NULL;
}

CSelectionHUD* CSelectionHUD::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CSelectionHUD();
	return m_Instance;
}

void CSelectionHUD::DeleteInstance()
{
	delete m_Instance;
}

void CSelectionHUD::Update(float Delta)
{
	CGUIObject::Update(Delta);
}

void CSelectionHUD::Draw()
{
	D3DXVECTOR2 Size;
	Size.x = 300;
	Size.y = 128;

	DrawGUIFrame(this, GetPosition(), Size);
	for(unsigned char i = 0; i < CCursor::MaxSelectables; ++i)
	{
		m_UnitIcons[i].Draw();
	}
}

RECT CSelectionHUD::GetDrawRectangle() const
{
	RECT DrawRectangle;
	DrawRectangle.left = (LONG)GetPositionX();
	DrawRectangle.right = DrawRectangle.left + 300;
	DrawRectangle.top = (LONG)GetPositionY();
	DrawRectangle.bottom = DrawRectangle.top + 128;
	return DrawRectangle;
}

RECT CSelectionHUD::GetCollisionRectangle() const
{
	RECT CollisionRectangle;
	CollisionRectangle.left = (LONG)GetPositionX();
	CollisionRectangle.right = CollisionRectangle.left + 300;
	CollisionRectangle.top = (LONG)GetPositionY();
	CollisionRectangle.bottom = CollisionRectangle.top + 128;
	return CollisionRectangle;
}

CUnitObject* CSelectionHUD::GetSubSelected() const
{
	return m_SubSelected;
}

void CSelectionHUD::SetSubSelected(CUnitObject* const UnitObject)
{
	if(m_SubSelected)
	{
		m_SubSelected->SetSubSelected(false);
		m_SubSelected->ReleaseReference();
	}
	m_SubSelected = UnitObject;
	if(m_SubSelected)
	{
		m_SubSelected->AcquireReference();
		m_SubSelected->SetSubSelected(true);
	}
	SubSelectionChanged.Trigger(this);
}

void CSelectionHUD::OnClick()
{
	RECT CursorRectangle = CCursor::GetInstance()->GetCollisionRectangle();
	CursorRectangle.left -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.right -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.top -= (LONG)CAMERA->GetPositionY();
	CursorRectangle.bottom -= (LONG)CAMERA->GetPositionY();

	for(unsigned char i = 0; i < CCursor::MaxSelectables; ++i)
	{
		RECT IntersectionRectangle;
		if(IntersectRect(&IntersectionRectangle, &CursorRectangle, &m_UnitIcons[i].GetCollisionRectangle()))
		{
			CUnitObject* UnitObject = m_UnitIcons[i].GetSelectedUnit();
			if(UnitObject)
				SetSubSelected(UnitObject);
			return;
		}
	}
}

IObject* CSelectionHUD::CreateSelectionHUD(CObjectManager* ObjectManager)
{
	CSprite* Sprite = CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/Button.spr");
	if(!Sprite)
		return NULL;

	CSelectionHUD* SelectionHUD = CSelectionHUD::GetInstance();
	SelectionHUD->SetSprite(Sprite);
	SelectionHUD->SetPosition(244, 640);

	Sprite = CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/EmptyPortrait.spr");
	if(!Sprite)
	{
		CSelectionHUD::DeleteInstance();
		return NULL;
	}

	for(unsigned char i = 0; i < CCursor::MaxSelectables; ++i)
	{
		unsigned char Row = i / 6;
		unsigned char Column = i % 6;
		SelectionHUD->m_UnitIcons[i].SetSprite(Sprite);
		SelectionHUD->m_UnitIcons[i].SetPosition(SelectionHUD->GetPositionX() + 4 + (Column * CUnitIcon::Width), SelectionHUD->GetPositionY() + 4 + (Row * CUnitIcon::Height));
	}

	CCursor::GetInstance()->SelectionChanged += &(SelectionHUD->OnSelectionChange);

	ObjectManager->AddObjectToLayer(4, SelectionHUD);
	return SelectionHUD;
}

void CSelectionHUD::OnSelectionChangeFunctor::operator()()
{
	CCursor* Sender = GetSender();
	CSelectionHUD* This = CSelectionHUD::GetInstance();

	// Iterate through selected objects for units and assign them to the unit icons.
	unsigned char index = 0;
	for(std::list<ISelectable*>::iterator i = CCursor::GetInstance()->GetSelectedObjects().begin(); i != CCursor::GetInstance()->GetSelectedObjects().end(); ++i)
	{
		CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(*i);
		if(AsUnitObject)
		{
			This->m_UnitIcons[index].SetSelectedUnit(AsUnitObject);
			++index;
		}
	}
	// Assign null to remaining unit icons.
	while(index < CCursor::MaxSelectables)
	{
		This->m_UnitIcons[index].SetSelectedUnit(NULL);
		++index;
	}
	This->SetSubSelected(This->m_UnitIcons[0].GetSelectedUnit());
}