//////////////////////////////////////////////////////
// File: "CFlipAction.cpp"
// Creator: MG
// Creation Date: 10/03/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////

#include "CFlipAction.h"
#include "CPan.h"
#include "../../Game/Objects/GUI Objects/CCursor.h"

CFlipAction::CFlipAction()
{
	m_bReleased = false;
}

void CFlipAction::Update(float dt)
{
	if(!m_bReleased)
	{
		m_Unit->SetPosition(CCursor::GetInstance()->GetPosition());
		if(m_StartFlip.y < CCursor::GetInstance()->GetPositionY())
		{
			m_StartFlip = CCursor::GetInstance()->GetPosition();
		}
	}
	else
	{
		if(m_Unit->GetVelocityY() < 200)
		{
			m_Unit->SetVelocityY(m_Unit->GetVelocityY() + (dt*400));
		}
		if(m_Unit->GetPanOwner()->GetPositionY() < m_Unit->GetPositionY())
		{
			m_Unit->EndFlip();
		}
	}
}

void CFlipAction::Draw()
{
	//Draw a spatula before this
	m_Unit->StrictDraw();
}

void CFlipAction::FinishFlip()
{
	m_bReleased = true;

	D3DXVECTOR2 direction;
	m_EndFlip = CCursor::GetInstance()->GetPosition() - m_StartFlip;
	D3DXVec2Normalize(&direction, &m_EndFlip);
	D3DXVec2Scale(&direction, &direction, 250);
	direction.x = direction.x * 2;
	m_Unit->SetVelocity(direction);
}