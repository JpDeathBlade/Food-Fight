//////////////////////////////////////////////////////
// File: "CGameObject.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/14/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CGameObject.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"

CGameObject::CGameObject()
{
	SetPosition(0, 0);
	SetRotation(0);
	SetScale(1, 1);
	SetVelocity(0, 0);
}

CGameObject::CGameObject(const CGameObject& Source) : IObject(Source), IAnimable(Source)
{
	SetPosition(Source.GetPosition());
	SetRotation(Source.GetRotation());
	SetScale(Source.GetScale());
	SetVelocity(Source.GetVelocity());
}

CGameObject& CGameObject::operator=(const CGameObject& Source)
{
	IObject::operator=(Source);
	IAnimable::operator=(Source);
	SetPosition(Source.GetPosition());
	SetRotation(Source.GetRotation());
	SetScale(Source.GetScale());
	SetVelocity(Source.GetVelocity());
	return *this;
}

CGameObject::~CGameObject()
{

}

void CGameObject::Update(float Delta)
{
	Translate(GetVelocity() * Delta);
	UpdateAnimation(Delta);
}

void CGameObject::Draw()
{
	DrawAnimation(GetPosition(), GetRotation(), GetScale());
}

RECT CGameObject::GetDrawRectangle() const
{
	CFrame* Frame = GetCurrentFrame();
	if(!Frame)
		return IObject::GetDrawRectangle();

	RECT DrawRect = Frame->GetDrawRectangle();
	POINT AnchorPoint = Frame->GetAnchorPoint(0)->GetAnchorPoint();
	DrawRect.left += (LONG)(GetPositionX() - AnchorPoint.x);
	DrawRect.right += (LONG)(GetPositionX() - AnchorPoint.x);
	DrawRect.top += (LONG)(GetPositionY() - AnchorPoint.y);
	DrawRect.bottom += (LONG)(GetPositionY() - AnchorPoint.y);
	return DrawRect;
}

RECT CGameObject::GetCollisionRectangle() const
{
	CFrame* Frame = GetCurrentFrame();
	if(!Frame)
		return IObject::GetCollisionRectangle();

	RECT CollisionRect = Frame->GetCollisionRectangle();
	POINT AnchorPoint = Frame->GetAnchorPoint(0)->GetAnchorPoint();
	CollisionRect.left += (LONG)(GetPositionX() - AnchorPoint.x);
	CollisionRect.right += (LONG)(GetPositionX() - AnchorPoint.x);
	CollisionRect.top += (LONG)(GetPositionY() - AnchorPoint.y);
	CollisionRect.bottom += (LONG)(GetPositionY() - AnchorPoint.y);
	return CollisionRect;
}

void CGameObject::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{
	Translate(Overlap);
}

float CGameObject::GetZDepth() const
{
	return GetPositionY();
}

D3DXVECTOR2 CGameObject::GetPosition() const
{
	return m_Position;
}

float CGameObject::GetPositionX() const
{
	return m_Position.x;
}

float CGameObject::GetPositionY() const
{
	return m_Position.y;
}

void CGameObject::SetPosition(D3DXVECTOR2 Position)
{
	m_Position = Position;
}

void CGameObject::SetPosition(float X, float Y)
{
	SetPositionX(X);
	SetPositionY(Y);
}

void CGameObject::SetPositionX(float X)
{
	m_Position.x = X;
}

void CGameObject::SetPositionY(float Y)
{
	m_Position.y = Y;
}

void CGameObject::Translate(D3DXVECTOR2 Distance)
{
	SetPosition(GetPosition() + Distance);
}

void CGameObject::Translate(float X, float Y)
{
	TranslateX(X);
	TranslateY(Y);
}

void CGameObject::TranslateX(float X)
{
	SetPositionX(GetPositionX() + X);
}

void CGameObject::TranslateY(float Y)
{
	SetPositionY(GetPositionY() + Y);
}

D3DXVECTOR2 CGameObject::GetVelocity() const
{
	return m_Velocity;
}

float CGameObject::GetVelocityX() const
{
	return m_Velocity.x;
}

float CGameObject::GetVelocityY() const
{
	return m_Velocity.y;
}

void CGameObject::SetVelocity(D3DXVECTOR2 Velocity)
{
	m_Velocity = Velocity;
}

void CGameObject::SetVelocity(float X, float Y)
{
	SetVelocityX(X);
	SetVelocityY(Y);
}

void CGameObject::SetVelocityX(float X)
{
	m_Velocity.x = X;
}

void CGameObject::SetVelocityY(float Y)
{
	m_Velocity.y = Y;
}

void CGameObject::Accelerate(D3DXVECTOR2 Velocity)
{
	SetVelocity(GetVelocity() + Velocity);
}

void CGameObject::Accelerate(float X, float Y)
{
	AccelerateX(X);
	AccelerateY(Y);
}

void CGameObject::AccelerateX(float X)
{
	SetVelocityX(GetVelocityX() + X);
}

void CGameObject::AccelerateY(float Y)
{
	SetVelocityY(GetVelocityY() + Y);
}

float CGameObject::GetRotation() const
{
	return m_Rotation;
}

void CGameObject::SetRotation(float Angle)
{
	m_Rotation = Angle;
}

D3DXVECTOR2 CGameObject::GetScale() const
{
	return m_Scale;
}

float CGameObject::GetScaleX() const
{
	return m_Scale.x;	
}

float CGameObject::GetScaleY() const
{
	return m_Scale.y;
}

void CGameObject::SetScale(D3DXVECTOR2 Scale)
{
	m_Scale = Scale;
}

void CGameObject::SetScale(float X, float Y)
{
	SetScaleX(X);
	SetScaleY(Y);
}

void CGameObject::SetScaleX(float X)
{
	m_Scale.x = X;
}

void CGameObject::SetScaleY(float Y)
{
	m_Scale.y = Y;
}