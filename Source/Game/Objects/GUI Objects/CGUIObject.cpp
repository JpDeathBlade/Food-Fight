//////////////////////////////////////////////////////
// File: "CGUIObject.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/14/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CGUIObject.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../../Managers/SGD/CSGD_Direct3D.h"

CGUIObject::CGUIObject()
{
	SetCollideable(false);
	SetPosition(0, 0);
}

CGUIObject::CGUIObject(const CGUIObject& Source) : IObject(Source), IAnimable(Source)
{
	SetCollideable(Source.IsCollideable());
	SetPosition(Source.GetPosition());
}

CGUIObject& CGUIObject::operator=(const CGUIObject& Source)
{
	IObject::operator=(Source);
	IAnimable::operator=(Source);
	SetCollideable(Source.IsCollideable());
	SetPosition(Source.GetPosition());
	return *this;
}

CGUIObject::~CGUIObject()
{

}

void CGUIObject::Update(float Delta)
{
	UpdateAnimation(Delta);
}

void CGUIObject::Draw()
{
	DrawAnimation(GetPosition());
}

RECT CGUIObject::GetDrawRectangle() const
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

RECT CGUIObject::GetCollisionRectangle() const
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

float CGUIObject::GetZDepth() const
{
	return GetPositionY();
}

D3DXVECTOR2 CGUIObject::GetPosition() const
{
	return m_Position;
}

float CGUIObject::GetPositionX() const
{
	return m_Position.x;
}

float CGUIObject::GetPositionY() const
{
	return m_Position.y;
}

void CGUIObject::SetPosition(D3DXVECTOR2 Position)
{
	m_Position = Position;
}

void CGUIObject::SetPosition(float X, float Y)
{
	SetPositionX(X);
	SetPositionY(Y);
}

void CGUIObject::SetPositionX(float X)
{
	m_Position.x = X;
}

void CGUIObject::SetPositionY(float Y)
{
	m_Position.y = Y;
}

void CGUIObject::Translate(D3DXVECTOR2 Distance)
{
	SetPosition(GetPosition() + Distance);
}

void CGUIObject::Translate(float X, float Y)
{
	TranslateX(X);
	TranslateY(Y);
}

void CGUIObject::TranslateX(float X)
{
	SetPositionX(GetPositionX() + X);
}

void CGUIObject::TranslateY(float Y)
{
	SetPositionY(GetPositionY() + Y);
}

void DrawGUIFrame(IAnimable* Animable, D3DXVECTOR2 Position, D3DXVECTOR2 Size, D3DCOLOR Color)
{
	CSprite* Sprite = Animable->GetSprite();

	//RECT CornerFrame = GetCurrentAnimation()->GetFrame("Corner")->GetDrawRectangle();
	//RECT VerticalFrame = GetCurrentAnimation()->GetFrame("Vertical")->GetDrawRectangle();
	//RECT InteriorFrame = GetCurrentAnimation()->GetFrame("Interior")->GetDrawRectangle();

	RECT CornerFrame = Animable->GetCurrentAnimation()->GetFrame(0)->GetDrawRectangle();
	RECT VerticalFrame = Animable->GetCurrentAnimation()->GetFrame(1)->GetDrawRectangle();
	RECT InteriorFrame = Animable->GetCurrentAnimation()->GetFrame(2)->GetDrawRectangle();	

	D3DXVECTOR2 InteriorArea;
	InteriorArea.x = Size.x - (2 * (CornerFrame.right - CornerFrame.left));
	InteriorArea.y = Size.y - (2 * (CornerFrame.bottom - CornerFrame.top));

	DWORD MinFilterValue;
	DWORD MagFilterValue;

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetSamplerState(0, D3DSAMP_MINFILTER, &MinFilterValue);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetSamplerState(0, D3DSAMP_MAGFILTER, &MagFilterValue);

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	// Draw the top left corner.
	Sprite->DrawBitmap(Position.x, Position.y, 1, 1, &CornerFrame, 0, 0, 0, Color);

	// Draw the top right corner.
	Sprite->DrawBitmap(Position.x + Size.x, Position.y, 1, 1, &CornerFrame, 0, 0, D3DXToRadian(90), Color);

	// Draw the bottom left corner.
	Sprite->DrawBitmap(Position.x, Position.y + Size.y, 1, 1, &CornerFrame, 0, 0, D3DXToRadian(270), Color);

	// Draw the bottom right corner.
	Sprite->DrawBitmap(Position.x + Size.x, Position.y + Size.y, 1, 1, &CornerFrame, 0, 0, D3DXToRadian(180), Color);

	// Draw the left side.
	Sprite->DrawBitmap(Position.x, Position.y + Size.y - (CornerFrame.bottom - CornerFrame.top), 1, InteriorArea.y, &VerticalFrame, 0, 0, D3DXToRadian(-90), Color);

	// Draw the right side.
	Sprite->DrawBitmap(Position.x + Size.x, Position.y + (CornerFrame.bottom - CornerFrame.top), 1, InteriorArea.y, &VerticalFrame, 0, 0, D3DXToRadian(90), Color);

	// Draw the top side.
	Sprite->DrawBitmap(Position.x + (CornerFrame.right - CornerFrame.left), Position.y, InteriorArea.x, 1, &VerticalFrame, 0, 0, 0, Color);

	// Draw the bottom side.
	Sprite->DrawBitmap(Position.x + Size.x - (CornerFrame.right - CornerFrame.left), Position.y + Size.y, InteriorArea.x, 1, &VerticalFrame, 0, 0, D3DXToRadian(180), Color);

	// Draw the inside.
	Sprite->DrawBitmap(Position.x + (CornerFrame.right - CornerFrame.left), Position.y + (CornerFrame.bottom - CornerFrame.top), InteriorArea.x, InteriorArea.y, &InteriorFrame, 0, 0, 0, Color);

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, MinFilterValue);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, MagFilterValue);
}