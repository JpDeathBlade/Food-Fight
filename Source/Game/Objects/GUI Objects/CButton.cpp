#include "CButton.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../../Managers/SGD/CSGD_Direct3D.h"
#include "../../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../Base/IFunctor.h"
#include "../../../Managers/Bad Chef/Audio/CAudioManager.h"

CButton::CButton()
{
	SetCurrentAnimation("Up");
	SetSize(0, 0);
	SetFontScale(1, 1);
	SetStringOffset(0, 0);
	SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	Enable();
	SetOnClick(NULL);
	SetOnRelease(NULL);
	SetOnHover(NULL);
	m_FitTextToSize = false;
}

CButton::CButton(const CButton& Source) : CGUIObject(Source), IClickable(Source), IHoverable(Source)
{
	SetString(Source.GetString());
	SetSize(Source.GetSize());
	SetString(Source.GetFontFileName());
	SetFontScale(Source.GetFontScale());
	SetStringOffset(Source.GetStringOffset());
	SetTextColor(Source.GetTextColor());
	m_Enabled = Source.IsEnabled();
	SetOnClick(Source.m_OnClick);
	SetOnRelease(Source.m_OnRelease);
	SetOnHover(Source.m_OnHover);
	m_FitTextToSize = Source.m_FitTextToSize;
}

CButton& CButton::operator=(const CButton& Source)
{
	CGUIObject::operator=(Source);
	IClickable::operator=(Source);
	IHoverable::operator=(Source);
	SetString(Source.GetString());
	SetSize(Source.GetSize());
	SetString(Source.GetFontFileName());
	SetFontScale(Source.GetFontScale());
	SetStringOffset(Source.GetStringOffset());
	SetTextColor(Source.GetTextColor());
	m_Enabled = Source.IsEnabled();
	SetOnClick(Source.m_OnClick);
	SetOnRelease(Source.m_OnRelease);
	SetOnHover(Source.m_OnHover);
	m_FitTextToSize = Source.m_FitTextToSize;
	return *this;
}

CButton::~CButton()
{

}

void CButton::Draw()
{
	D3DCOLOR Color = IsEnabled() ? (IsBeingHoveredOver() ? D3DCOLOR_XRGB(127, 255, 127) : D3DCOLOR_XRGB(255, 255, 255)) : D3DCOLOR_XRGB(127, 127, 127);

	DrawGUIFrame(this, GetPosition(), GetSize(), Color);

	if(!m_String.empty())
		BITMAP->DrawStringSprite(GetFontFileName().c_str(), m_String.c_str(), GetPositionX() + GetStringOffsetX(), GetPositionY() + GetStringOffsetY(), GetFontScaleX(), GetFontScaleY(), 0, Color + GetTextColor(), m_FitTextToSize ? &(GetSize() - GetStringOffset() * 2) : NULL);
}

RECT CButton::GetDrawRectangle() const
{
	RECT DrawRectangle;
	DrawRectangle.left = (LONG)GetPositionX();
	DrawRectangle.right = DrawRectangle.left + (LONG)GetSizeX();
	DrawRectangle.top = (LONG)GetPositionY();
	DrawRectangle.bottom = DrawRectangle.top + (LONG)GetSizeY();
	return DrawRectangle;
}

RECT CButton::GetCollisionRectangle() const
{
	RECT CollisionRectangle;
	CollisionRectangle.left = (LONG)GetPositionX();
	CollisionRectangle.right = CollisionRectangle.left + (LONG)GetSizeX();
	CollisionRectangle.top = (LONG)GetPositionY();
	CollisionRectangle.bottom = CollisionRectangle.top + (LONG)GetSizeY();
	return CollisionRectangle;
}

D3DXVECTOR2 CButton::GetSize() const
{
	return m_Size;
}

float CButton::GetSizeX() const
{
	return m_Size.x;
}

float CButton::GetSizeY() const
{
	return m_Size.y;
}

void CButton::SetSize(D3DXVECTOR2 Size)
{
	m_Size = Size;
}

void CButton::SetSize(float X, float Y)
{
	SetSizeX(X);
	SetSizeY(Y);
}

void CButton::SetSizeX(float X)
{
	m_Size.x = X;
}

void CButton::SetSizeY(float Y)
{
	m_Size.y = Y;
}

D3DXVECTOR2 CButton::GetFontScale() const
{
	return m_FontScale;
}

float CButton::GetFontScaleX() const
{
	return m_FontScale.x;
}

float CButton::GetFontScaleY() const
{
	return m_FontScale.y;
}

void CButton::SetFontScale(D3DXVECTOR2 FontScale)
{
	m_FontScale = FontScale;
}

void CButton::SetFontScale(float X, float Y)
{
	SetFontScaleX(X);
	SetFontScaleY(Y);
}

void CButton::SetFontScaleX(float X)
{
	m_FontScale.x = X;
}

void CButton::SetFontScaleY(float Y)
{
	m_FontScale.y = Y;
}

std::string CButton::GetString() const
{
	return m_String;
}

void CButton::SetString(std::string String)
{
	m_String = CStringTable::GetInstance()->FindString(String);
}

std::string CButton::GetFontFileName() const
{
	return m_FontFileName;
}

void CButton::SetFontFileName(std::string FontFileName)
{
	m_FontFileName = FontFileName;
}

D3DXVECTOR2 CButton::GetStringOffset() const
{
	return m_StringOffset;
}

float CButton::GetStringOffsetX() const
{
	return m_StringOffset.x;
}

float CButton::GetStringOffsetY() const
{
	return m_StringOffset.y;
}

void CButton::SetStringOffset(D3DXVECTOR2 StringOffset)
{
	m_StringOffset = StringOffset;
}

void CButton::SetStringOffset(float X, float Y)
{
	SetStringOffsetX(X);
	SetStringOffsetY(Y);
}

void CButton::SetStringOffsetX(float X)
{
	m_StringOffset.x = X;
}

void CButton::SetStringOffsetY(float Y)
{
	m_StringOffset.y = Y;
}

D3DCOLOR CButton::GetTextColor() const
{
	return m_TextColor;
}

void CButton::SetTextColor(D3DCOLOR Color)
{
	m_TextColor = Color;
}

bool CButton::IsEnabled() const
{
	return m_Enabled;
}

void CButton::Enable()
{
	m_Enabled = true;
}

void CButton::Disable()
{
	m_Enabled = false;
}

void CButton::OnClick()
{
	IClickable::OnClick();
	if(IsEnabled())
	{
		if(m_OnClick)
			(*m_OnClick)();
		SetCurrentAnimation("Down");
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/ButtonClick.wav", "User Interface")->Play();
	}
}

void CButton::SetOnClick(IFunctor* OnClick)
{
	m_OnClick = OnClick;
}

void CButton::OnRelease()
{
	IClickable::OnRelease();
	if(IsEnabled())
	{
		if(m_OnRelease)
			(*m_OnRelease)();
		SetCurrentAnimation("Up");
	}
}

void CButton::SetOnRelease(IFunctor* OnRelease)
{
	m_OnRelease = OnRelease;
}

void CButton::OnHover()
{
	if(IsEnabled())
		if(m_OnHover)
			(*m_OnHover)();
}

void CButton::SetOnHover(IFunctor* OnHover)
{
	m_OnHover = OnHover;
}

void CButton::FitToText(int border)
{
	D3DXVECTOR2 Size = BITMAP->CalculateSizeSprite(GetFontFileName().c_str(), GetString().c_str(), GetFontScaleX(), GetFontScaleY());
	Size.x += GetStringOffsetX() * 2;
	Size.y += GetStringOffsetY() * 2;
	Size.x += border*2;
	Size.y += border*2;
	SetSize(Size);
}

void CButton::FitTextToSize(bool Fit)
{
	m_FitTextToSize = Fit;
}