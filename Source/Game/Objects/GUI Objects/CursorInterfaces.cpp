#include "CursorInterfaces.h"
#include "CCursor.h"

// IClickable //

IClickable::IClickable()
{
	m_Clicked = false;
}

IClickable::IClickable(const IClickable& Source)
{
	m_Clicked = Source.IsClicked();
}

IClickable& IClickable::operator=(const IClickable& Source)
{
	m_Clicked = Source.IsClicked();
	return *this;
}

IClickable::~IClickable()
{

}

void IClickable::OnClick()
{
	m_Clicked = true;
}

bool IClickable::IsClicked() const
{
	return m_Clicked;
}

void IClickable::OnRelease()
{
	m_Clicked = false;
}

// IDraggable //

IDraggable::IDraggable()
{
	m_IsDragging = false;
}

IDraggable::IDraggable(const IDraggable& Source) : IClickable(Source)
{
	m_IsDragging = false;
}

IDraggable& IDraggable::operator=(const IDraggable& Source)
{
	IClickable::operator=(Source);
	m_IsDragging = false;
	return *this;
}

IDraggable::~IDraggable()
{

}

void IDraggable::OnBeginDrag()
{
	m_IsDragging = true;
}

void IDraggable::OnEndDrag()
{
	m_IsDragging = false;
}

bool IDraggable::IsBeingDragged() const
{
	return m_IsDragging;
}

// IHoverable //

IHoverable::IHoverable()
{
	m_IsHovering = false;
}

IHoverable::IHoverable(const IHoverable& Source)
{
	m_IsHovering = false;
}

IHoverable& IHoverable::operator=(const IHoverable& Source)
{
	m_IsHovering = false;
	return *this;
}

IHoverable::~IHoverable()
{

}

void IHoverable::OnBeginHover()
{
	m_IsHovering = true;
}


void IHoverable::OnHover()
{

}

void IHoverable::OnEndHover()
{
	m_IsHovering = false;
}

bool IHoverable::IsBeingHoveredOver() const
{
	return m_IsHovering;
}

// ISelectable //

ISelectable::ISelectable()
{
	m_IsSelected = false;
	m_IsSubSelected = false;
}

ISelectable::ISelectable(const ISelectable& Source)
{
	m_IsSelected = false;
	m_IsSubSelected = false;
}

ISelectable& ISelectable::operator=(const ISelectable& Source)
{
	return *this;
}

ISelectable::~ISelectable()
{

}

bool ISelectable::IsSelected() const
{
	return m_IsSelected;
}

void ISelectable::SetSelected(bool Selected)
{
	m_IsSelected = Selected;
	if(!m_IsSelected)
	{
		SetSubSelected(false);
	}
}

bool ISelectable::IsSubSelected() const
{
	return m_IsSubSelected;
}

void ISelectable::SetSubSelected(bool SubSelected)
{
	m_IsSubSelected = SubSelected;
}

ETeamValue ISelectable::GetTeamValue() const
{
	return m_eTeamValue;
}

void ISelectable::SetTeamValue(ETeamValue eTeamValue)
{
	m_eTeamValue = eTeamValue;
}

void ISelectable::ContextSensitiveClick()
{

}