#include "CActionsHUD.h"
#include "CSelectionHUD.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../Units/CUnitObject.h"
#include "../Units/CSkilledUnitObject.h"
#include "CCursor.h"
#include "../Base/CCamera.h"
#include "../../Unit Commands/CMoveCommand.h"
#include "../../Unit Commands/CAttackCommand.h"
#include "../../Unit Commands/CAttackMoveCommand.h"
#include "../Units/IAbility.h"

// CMoveCursorFunctor //

CActionsHUD::CMoveCursorFunctor::CMoveCursorFunctor()
{
	Cursor = CCursor::GetInstance();
}

CActionsHUD::CMoveCursorFunctor::~CMoveCursorFunctor()
{

}

void CActionsHUD::CMoveCursorFunctor::operator()()
{
	std::list<ISelectable*>& SelectedObjects = Cursor->GetSelectedObjects();
	for(std::list<ISelectable*>::iterator i = SelectedObjects.begin(); i != SelectedObjects.end(); ++i)
	{
		CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(*i);
		if(AsUnitObject)
		{
			CMoveCommand* MoveCommand = new CMoveCommand;
			MoveCommand->SetDestination(Cursor->GetPosition());
			AsUnitObject->SetUnitAICommand(MoveCommand);
		}
	}
	Cursor->OverrideClick(NULL);
	Cursor->ResetSprite();
	delete this;
}

// CAttackCursorFunctor //

CActionsHUD::CAttackCursorFunctor::CAttackCursorFunctor()
{
	Cursor = CCursor::GetInstance();
}

CActionsHUD::CAttackCursorFunctor::~CAttackCursorFunctor()
{

}

void CActionsHUD::CAttackCursorFunctor::operator()()
{
	bool IsAttackMove = true;
	IAttackable* AsAttackable = dynamic_cast<IAttackable*>(CCursor::GetInstance()->GetHoveredOverObject());
	if(AsAttackable)
	{
		ISelectable* AsSelectable = dynamic_cast<ISelectable*>(CCursor::GetInstance()->GetHoveredOverObject());
		if(AsSelectable && AsSelectable->GetTeamValue() != TEAM_PLAYER)
		{
			IsAttackMove = false;
		}
	}


	std::list<ISelectable*>& SelectedObjects = Cursor->GetSelectedObjects();
	for(std::list<ISelectable*>::iterator i = SelectedObjects.begin(); i != SelectedObjects.end(); ++i)
	{
		CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(*i);
		if(AsUnitObject)
		{
			if(IsAttackMove)
			{
				CAttackMoveCommand* AttackMoveCommand = new CAttackMoveCommand(CCursor::GetInstance()->GetPosition(), AsUnitObject->GetObjectManagerOwner());
				AsUnitObject->SetUnitAICommand(AttackMoveCommand);
			}
			else
			{
				CAttackCommand* AttackCommand = new CAttackCommand(AsAttackable);
				AsUnitObject->SetUnitAICommand(AttackCommand);
			}
		}
	}
	Cursor->OverrideClick(NULL);
	Cursor->ResetSprite();
	delete this;
}

// CActionButton //

CActionButton::CActionButton()
{
	m_Icon = NULL;
	m_DrawBack = true;
}

CActionButton::~CActionButton()
{

}

void CActionButton::Draw()
{
	if(m_DrawBack)
		CButton::Draw();
	if(m_Icon)
	{
		m_Icon->Draw(0, 0, GetPosition() + D3DXVECTOR2(Width * 0.5f, Height * 0.5f), 0, D3DXVECTOR2(1, 1), IsEnabled() ? D3DCOLOR_XRGB(255, 255, 255) : D3DCOLOR_XRGB(127, 127, 127));
	}
}

CSprite* CActionButton::GetIcon() const
{
	return m_Icon;
}

void CActionButton::SetIcon(CSprite* const Icon)
{
	m_Icon = Icon;
}

// CActionsHUD //

CActionsHUD* CActionsHUD::m_Instance = NULL;

CActionsHUD::CActionsHUD()
{

}

CActionsHUD::~CActionsHUD()
{
	m_Instance = NULL;
}

CActionsHUD* CActionsHUD::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CActionsHUD;
	return m_Instance;
}

void CActionsHUD::DeleteInstance()
{
	delete m_Instance;
}

void CActionsHUD::Update(float Delta)
{
	for(unsigned char i = 0; i < 6; ++i)
	{
		m_ActionButtons[i].Update(Delta);
	}
	for(unsigned char i = 3; i < 6; ++i)
	{
		if(m_ActionButtons[i].GetOnClick())
		{
			if(((IAbility*)m_ActionButtons[i].GetOnClick())->GetCooldown() <= GetTickCount())
			{
				m_ActionButtons[i].Enable();
			}
			else
			{
				m_ActionButtons[i].Disable();
			}
		}
	}
}

void CActionsHUD::Draw()
{
	D3DXVECTOR2 Size;
	Size.x = Width;
	Size.y = Height;

	DrawGUIFrame(this, GetPosition(), Size);

	for(unsigned char i = 0; i < 6; ++i)
	{
		m_ActionButtons[i].Draw();
	}

	for(unsigned char i = 3; i < 6; ++i)
	{
		if(m_ActionButtons[i].IsBeingHoveredOver())
		{
			IAbility* skill = NULL;
			if(skill = dynamic_cast<IAbility*>(m_ActionButtons[i].GetOnClick()))
			{
				skill->GetFrame()->Draw();
			}
		}
	}
}

RECT CActionsHUD::GetDrawRectangle() const
{
	RECT DrawRectangle;
	DrawRectangle.left = (LONG)GetPositionX();
	DrawRectangle.right = DrawRectangle.left + Width;
	DrawRectangle.top = (LONG)GetPositionY();
	DrawRectangle.bottom = DrawRectangle.top + Height;
	return DrawRectangle;
}

RECT CActionsHUD::GetCollisionRectangle() const
{
	RECT CollisionRectangle;
	CollisionRectangle.left = (LONG)GetPositionX();
	CollisionRectangle.right = CollisionRectangle.left + Width;
	CollisionRectangle.top = (LONG)GetPositionY();
	CollisionRectangle.bottom = CollisionRectangle.top + Height;
	return CollisionRectangle;
}

void CActionsHUD::OnBeginHover()
{
	IHoverable::OnBeginHover();
}

void CActionsHUD::OnHover()
{
	IHoverable::OnHover();
	RECT CursorRectangle = CCursor::GetInstance()->GetCollisionRectangle();
	CursorRectangle.left -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.right -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.top -= (LONG)CAMERA->GetPositionY();
	CursorRectangle.bottom -= (LONG)CAMERA->GetPositionY();

	for(unsigned char i = 0; i < 6; ++i)
	{
		RECT IntersectionRectangle;
		if(IntersectRect(&IntersectionRectangle, &CursorRectangle, &m_ActionButtons[i].GetCollisionRectangle()))
		{
			m_ActionButtons[i].OnBeginHover();
		}
		else
		{
			m_ActionButtons[i].OnEndHover();
		}
	}
}

void CActionsHUD::OnEndHover()
{
	IHoverable::OnEndHover();
	for(unsigned char i = 0; i < 6; ++i)
	{
		m_ActionButtons[i].OnEndHover();
	}
}

void CActionsHUD::OnClick()
{
	IClickable::OnClick();
	RECT CursorRectangle = CCursor::GetInstance()->GetCollisionRectangle();
	CursorRectangle.left -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.right -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.top -= (LONG)CAMERA->GetPositionY();
	CursorRectangle.bottom -= (LONG)CAMERA->GetPositionY();

	for(unsigned char i = 0; i < 6; ++i)
	{
		RECT IntersectionRectangle;
		if(IntersectRect(&IntersectionRectangle, &CursorRectangle, &m_ActionButtons[i].GetCollisionRectangle()))
		{
#if _DEBUG
			CSkill_Waffle_BelgianSurprise* test = NULL;
			if(test = dynamic_cast<CSkill_Waffle_BelgianSurprise*>(m_ActionButtons[i].GetOnClick()))
			{
				printf("Waffle Belgain Surprise Clicked!\n");
				printf("String contained: %s", test->GetFrame()->GetString().c_str());
			}
#endif
			m_ActionButtons[i].OnClick();
			return;
		}
	}
}

void CActionsHUD::OnRelease()
{
	RECT CursorRectangle = CCursor::GetInstance()->GetCollisionRectangle();
	CursorRectangle.left -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.right -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.top -= (LONG)CAMERA->GetPositionY();
	CursorRectangle.bottom -= (LONG)CAMERA->GetPositionY();

	for(unsigned char i = 0; i < 6; ++i)
	{
		RECT IntersectionRectangle;
		if(IntersectRect(&IntersectionRectangle, &CursorRectangle, &m_ActionButtons[i].GetCollisionRectangle()))
		{
			m_ActionButtons[i].OnRelease();
			return;
		}
	}
	IClickable::OnRelease();
}

IObject* CActionsHUD::CreateActionsHUD(CObjectManager* ObjectManager)
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");
	if(!Sprite)
		return NULL;

	CActionsHUD* ActionsHUD = CActionsHUD::GetInstance();
	ActionsHUD->SetSprite(Sprite);
	ActionsHUD->SetPosition(800 - Width, 768 - Height);

	for(unsigned char i = 0; i < 6; ++i)
	{
		unsigned char row = i / 3;
		unsigned char column = i % 3;
		ActionsHUD->m_ActionButtons[i].SetSprite(Sprite);
		ActionsHUD->m_ActionButtons[i].SetSize(60, 60);
		ActionsHUD->m_ActionButtons[i].SetPosition(ActionsHUD->GetPositionX() + 4 + (column * CActionButton::Width), ActionsHUD->GetPositionY() + 4 + (row * CActionButton::Height));
		ActionsHUD->m_ActionButtons[i].Disable();
	}

	ActionsHUD->m_ActionButtons[0].SetIcon(SpriteManager->GetSprite("Resources/Graphics/User Interface/MoveIcon.spr"));
	ActionsHUD->MoveButtonOnClick.SetActionButton(&ActionsHUD->m_ActionButtons[0]);
	ActionsHUD->m_ActionButtons[0].SetOnClick(&ActionsHUD->MoveButtonOnClick);
	ActionsHUD->m_ActionButtons[1].SetIcon(SpriteManager->GetSprite("Resources/Graphics/User Interface/AttackIcon.spr"));
	ActionsHUD->AttackButtonOnClick.SetActionButton(&ActionsHUD->m_ActionButtons[1]);
	ActionsHUD->m_ActionButtons[1].SetOnClick(&ActionsHUD->AttackButtonOnClick);
	ActionsHUD->m_ActionButtons[2].SetIcon(SpriteManager->GetSprite("Resources/Graphics/User Interface/StopIcon.spr"));
	ActionsHUD->m_ActionButtons[2].SetOnClick(&ActionsHUD->StopButtonOnClick);

	CSelectionHUD::GetInstance()->SubSelectionChanged += &(ActionsHUD->OnSubSelectionChange);

	ObjectManager->AddObjectToLayer(4, ActionsHUD);
	return ActionsHUD;
}

void CActionsHUD::OnSubSelectionChangeFunctor::operator()()
{
	CSelectionHUD* Sender = GetSender();

	CActionsHUD* This = CActionsHUD::GetInstance();
	CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(CSelectionHUD::GetInstance()->GetSubSelected());
	if(AsUnitObject)
	{
		// Enable/Disable Move Button
		if(AsUnitObject->GetMovementSpeed())
			This->m_ActionButtons[0].Enable();
		else
			This->m_ActionButtons[0].Disable();

		// Enable/Disable Attack Button
		if(AsUnitObject->GetAttack() || AsUnitObject->GetAttackSpeed())
			This->m_ActionButtons[1].Enable();
		else
			This->m_ActionButtons[1].Disable();

		// Enable/Disable Stop Button
		This->m_ActionButtons[2].Enable();

		// Set Skill Buttons
		CSkilledUnitObject* AsSkilledUnitObject = dynamic_cast<CSkilledUnitObject*>(AsUnitObject);
		if(AsSkilledUnitObject)
		{
			for(unsigned char i = 3; i < 6; ++i)
			{
				IAbility* Skill = AsSkilledUnitObject->GetSkill(i - 3);
				if(Skill)
				{
					This->m_ActionButtons[i].SetDrawBack(false);
					This->m_ActionButtons[i].Enable();
					This->m_ActionButtons[i].SetIcon(Skill->GetIcon());
					This->m_ActionButtons[i].SetOnClick(Skill);
				}
				else
				{
					This->m_ActionButtons[i].SetDrawBack(true);
					This->m_ActionButtons[i].Disable();
					This->m_ActionButtons[i].SetIcon(NULL);
					This->m_ActionButtons[i].SetOnClick(NULL);
				}
			}
		}
	}
	else
	{
		// Disable Move Button
		This->m_ActionButtons[0].Disable();

		// Disable Attack Button
		This->m_ActionButtons[1].Disable();

		// Disable Stop Button
		This->m_ActionButtons[2].Disable();

		// Set Skill Buttons
		for(unsigned char i = 3; i < 6; ++i)
		{
			This->m_ActionButtons[i].Disable();
			This->m_ActionButtons[i].SetIcon(NULL);
			This->m_ActionButtons[i].SetOnClick(NULL);
		}
	}
}

void CActionsHUD::CMoveButtonOnClickFunctor::operator()()
{
	CCursor::GetInstance()->SetSprite(m_ActionButton->GetIcon());
	CCursor::GetInstance()->OverrideClick(new CMoveCursorFunctor);
}

void CActionsHUD::CMoveButtonOnClickFunctor::SetActionButton(CActionButton* ActionButton)
{
	m_ActionButton = ActionButton;
}

void CActionsHUD::CAttackButtonOnClickFunctor::operator()()
{
	CCursor::GetInstance()->SetSprite(m_ActionButton->GetIcon());
	CCursor::GetInstance()->OverrideClick(new CAttackCursorFunctor);
}

void CActionsHUD::CAttackButtonOnClickFunctor::SetActionButton(CActionButton* ActionButton)
{
	m_ActionButton = ActionButton;
}

void CActionsHUD::CStopButtonOnClickFunctor::operator()()
{
	std::list<ISelectable*>& SelectedObjects = CCursor::GetInstance()->GetSelectedObjects();
	for(std::list<ISelectable*>::iterator i = SelectedObjects.begin(); i != SelectedObjects.end(); ++i)
	{
		CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(*i);
		if(AsUnitObject)
		{
			AsUnitObject->SetUnitAICommand(NULL);
		}
	}
}