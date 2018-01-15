//////////////////////////////////////////////////////
// File: "CCursorInterfaces.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/21/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef _CursorInterfaces_H_
#define _CursorInterfaces_H_

enum ETeamValue{TEAM_PLAYER, TEAM_ENEMY, TEAM_NONE};

class IClickable
{
	bool m_Clicked;
public:
	IClickable();
	IClickable(const IClickable& Source);
	virtual IClickable& operator=(const IClickable& Source);
	virtual ~IClickable();

	virtual void OnClick();
	bool IsClicked() const;
	virtual void OnRelease();
};

class IDraggable : public IClickable
{
	bool m_IsDragging;
public:
	IDraggable();
	IDraggable(const IDraggable& Source);
	virtual IDraggable& operator=(const IDraggable& Source);
	virtual ~IDraggable();

	virtual void OnBeginDrag();
	virtual void OnDrag() = 0;
	virtual void OnEndDrag();

	bool IsBeingDragged() const;
};

class IHoverable
{
	bool m_IsHovering;
public:
	IHoverable();
	IHoverable(const IHoverable& Source);
	virtual IHoverable& operator=(const IHoverable& Source);
	virtual ~IHoverable() = 0;

	virtual void OnBeginHover();
	virtual void OnHover();
	virtual void OnEndHover();

	bool IsBeingHoveredOver() const;
};

class ISelectable
{
	bool m_IsSelected;
	bool m_IsSubSelected;
	ETeamValue m_eTeamValue;

public:
	ISelectable();
	ISelectable(const ISelectable& Source);
	virtual ISelectable& operator=(const ISelectable& Source);
	virtual ~ISelectable() = 0;

	///////////////Accessors and Mutators///////////////
	bool IsSelected() const;
	void SetSelected(bool Selected);
	bool IsSubSelected() const;
	void SetSubSelected(bool SubSelected);

	ETeamValue GetTeamValue() const;
	void SetTeamValue(ETeamValue eTeamValue);

	virtual void ContextSensitiveClick();
};
#endif