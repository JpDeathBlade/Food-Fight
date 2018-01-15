//////////////////////////////////////////////////////
// File: "CCursor.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/17/09
// Last Modified By: SA
//////////////////////////////////////////////////////
#include "CCursor.h"
#include "../../../Managers/SGD/CSGD_DirectInput.h"
#include "../../../Managers/SGD/CSGD_Direct3D.h"
#include "CursorInterfaces.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include "../Base/CCamera.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../Base/IAttackable.h"
#include "../../Cooking Mini-Game/CCookingResource.h"
#include "../../Cooking Mini-Game/CCookingUnit.h"
#include "../Appliances/CStove.h"
#include "../../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../Units/CUnitObject.h"

#define MOUSE_MOVEMENT_SPEED 300

CCursor* CCursor::m_Instance = NULL;

CCursor::CCursor()
{
	m_DirectInput = CSGD_DirectInput::GetInstance();

	// Functors
	m_ClickFunctor = NULL;
	m_DragFunctor = NULL;
	m_ReleaseFunctor = NULL;

	// Hover Data
	m_CurrentlyHoveredOver = NULL;
	m_PreviouslyHoveredOver = NULL;

	// Dragging Data
	m_BeingDragged = NULL;
	m_CanDragSelect = false;
	m_DragRectangle = NULL;
}

CCursor::~CCursor()
{
	IReferenceable* AsReferenceable = NULL;

	// Dragging Data
	if(m_DragRectangle)
		delete m_DragRectangle;

	AsReferenceable = dynamic_cast<IReferenceable*>(m_BeingDragged);
	if(m_BeingDragged)
		AsReferenceable->ReleaseReference();
	m_BeingDragged = NULL;

	// Hover Data
	if(m_PreviouslyHoveredOver)
		m_PreviouslyHoveredOver->ReleaseReference();
	if(m_CurrentlyHoveredOver != m_PreviouslyHoveredOver && m_CurrentlyHoveredOver)
		m_CurrentlyHoveredOver->ReleaseReference();

	m_PreviouslyHoveredOver = NULL;
	m_CurrentlyHoveredOver = NULL;

	if(!m_SelectedObjects.empty())
	{
		for(std::list<ISelectable*>::iterator iter = m_SelectedObjects.begin(); iter != m_SelectedObjects.end(); ++iter)
		{
			IObject* pObject = dynamic_cast<IObject*>(*iter);

			if(pObject)
			{
				pObject->ReleaseReference();
				(*iter) = NULL;
			}
		}

		m_SelectedObjects.clear();
	}

	// Functors
	delete m_ReleaseFunctor;
	m_ReleaseFunctor = NULL;
	delete m_DragFunctor;
	m_DragFunctor = NULL;
	delete m_ClickFunctor;
	m_ClickFunctor = NULL;

	m_Instance = NULL;
}

CCursor* CCursor::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CCursor;
	return m_Instance;
}

void CCursor::DeleteInstance()
{
	delete m_Instance;
}

void CCursor::Input()
{
	Translate((float)m_DirectInput->MouseMovementX(), (float)m_DirectInput->MouseMovementY());

	if(m_PreviouslyHoveredOver)
		m_PreviouslyHoveredOver->ReleaseReference(); // The last previously hovered over object is released.
	m_PreviouslyHoveredOver = m_CurrentlyHoveredOver; // What was once current has become previous.
	m_CurrentlyHoveredOver = NULL;

	GetObjectManagerOwner()->IterateReversed<CCursor>(&HoverDetection, this); // Iterate through all objects to see what is being hovered over.

	IHoverable* PreviousHoverable = dynamic_cast<IHoverable*>(m_PreviouslyHoveredOver); // Determine if the previously hovered over object reacts to being hovered over.
	IHoverable* CurrentHoverable = dynamic_cast<IHoverable*>(m_CurrentlyHoveredOver); // Determine if the currently hovered over object reacts to being hovered over.

	if(m_CurrentlyHoveredOver != m_PreviouslyHoveredOver) // A new object has been hovered over.
	{
		IClickable* AsClickable = NULL;
		if(AsClickable = dynamic_cast<IClickable*>(m_PreviouslyHoveredOver))
			if(AsClickable->IsClicked())
				AsClickable->OnRelease();

		if(PreviousHoverable)
			PreviousHoverable->OnEndHover();
		if(CurrentHoverable)
			CurrentHoverable->OnBeginHover();
	}
	else // Call "OnHover" if the currently hovered over object is the same object that was being hovered over previously.
	{
		if(CurrentHoverable)
			CurrentHoverable->OnHover();
	}

	bool Clicked = false;
	bool Dragged = false;
	bool Released = false;

	for(int i = 0; i < m_DirectInput->MouseGetNumButtons(); ++i) // Detect mouse clicks, holds, and releases for all buttons.
	{
		if( (m_DirectInput->MouseButtonPressedEx(i) || (XBOX->GetController(0)->IsPressed_A() || XBOX->GetController(0)->IsPressed_B()) ) && !Clicked) // Only call "OnClick" once if any mouse button was just pressed.
		{
			OnClick();
			Clicked = true;
		}

		if( (m_DirectInput->MouseButtonDown(i) || XBOX->GetController(0)->IsDown_A()) && !Dragged) // Only call "OnDrag" once if any mouse button is down and the mouse has moved.
		{
			OnDrag();
			Dragged = true;
		}

		if( (m_DirectInput->MouseButtonReleasedEx(i) || XBOX->GetController(0)->IsReleased_A()) && !Released) // Only call "OnRelease" once if any mouse button was just released.
		{
			OnRelease();
			Released = true;
		}
	}

	if(XBOX->GetController(0)->IsDown_THUMB_LEFT_X() < 0.0f)
		TranslateX((float)XBOX->GetController(0)->GetThumbLeft_X() / MOUSE_MOVEMENT_SPEED);
	if(XBOX->GetController(0)->IsDown_THUMB_LEFT_X() > 0.0f)
		TranslateX((float)XBOX->GetController(0)->GetThumbLeft_X() / MOUSE_MOVEMENT_SPEED);

	if(XBOX->GetController(0)->IsDown_THUMB_LEFT_Y() < 0.0f)
		TranslateY((float)XBOX->GetController(0)->GetThumbLeft_Y() / MOUSE_MOVEMENT_SPEED);
	if(XBOX->GetController(0)->IsDown_THUMB_LEFT_Y() > 0.0f)
		TranslateY((float)XBOX->GetController(0)->GetThumbLeft_Y() / MOUSE_MOVEMENT_SPEED);
}

void CCursor::Update(float Delta)
{
	CGUIObject::Update(Delta);

	for(std::list<ISelectable*>::iterator i = m_SelectedObjects.begin(); i != m_SelectedObjects.end();)
	{
		IObject* AsObject = dynamic_cast<IObject*>(*i);
		if(AsObject && AsObject->CheckRemove())
		{
			AsObject->ReleaseReference();
			(*i)->SetSelected(false);

			i = m_SelectedObjects.erase(i);
			SelectionChanged.Trigger(this);
		}
		else
		{
			++i;
		}
	}
}

void CCursor::Draw()
{
	if(m_DragRectangle)
	{
		POINT Offset;
		Offset.x = (LONG)CAMERA->GetPositionX();
		Offset.y = (LONG)CAMERA->GetPositionY();

		CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2((float)(m_DragRectangle->left - Offset.x), (float)(m_DragRectangle->top - Offset.y)), D3DXVECTOR2((float)(m_DragRectangle->right - Offset.x), (float)(m_DragRectangle->top - Offset.y))); // NW to NE
		CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2((float)(m_DragRectangle->right - Offset.x), (float)(m_DragRectangle->top - Offset.y)), D3DXVECTOR2((float)(m_DragRectangle->right - Offset.x), (float)(m_DragRectangle->bottom - Offset.y))); // NE to SE
		CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2((float)(m_DragRectangle->right - Offset.x), (float)(m_DragRectangle->bottom - Offset.y)), D3DXVECTOR2((float)(m_DragRectangle->left - Offset.x), (float)(m_DragRectangle->bottom - Offset.y))); // SE to SW
		CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2((float)(m_DragRectangle->left - Offset.x), (float)(m_DragRectangle->bottom - Offset.y)), D3DXVECTOR2((float)(m_DragRectangle->left - Offset.x), (float)(m_DragRectangle->top - Offset.y))); // SW to NW
	}

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	D3DXMatrixTranslation(&Transform, -CAMERA->GetPositionX(), -CAMERA->GetPositionY(), 0);
	CSGD_Direct3D::GetInstance()->GetSprite()->SetTransform(&Transform);
	CGUIObject::Draw();
}

void CCursor::SetPosition(D3DXVECTOR2 Position)
{
	RECT Bounds = CAMERA->GetViewRect();
	Position.x = max(Bounds.left, min(Position.x, Bounds.right)); // Clamps to the camera's bounds.
	Position.y = max(Bounds.top, min(Position.y, Bounds.bottom)); // Clamps to the camera's bounds.
	CGUIObject::SetPosition(Position);
}

void CCursor::SetPosition(float X, float Y)
{
	SetPositionX(X);
	SetPositionY(Y);
}

void CCursor::SetPositionX(float X)
{
	RECT Bounds = CAMERA->GetViewRect();
	X = max(Bounds.left, min(X, Bounds.right)); // Clamps to the camera's bounds.
	CGUIObject::SetPositionX(X);
}

void CCursor::SetPositionY(float Y)
{
	RECT Bounds = CAMERA->GetViewRect();
	Y = max(Bounds.top, min(Y, Bounds.bottom)); // Clamps to the camera's bounds.
	CGUIObject::SetPositionY(Y);
}

void CCursor::ResetSprite()
{
	SetSprite(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/OvenMittCursor.spr"));
}

void CCursor::Select(ISelectable* const Selectable)
{
	if(!Selectable->IsSelected()) // Do not select the object again if it is already selected.
	{
		if(m_SelectedObjects.size() < MaxSelectables) // Select only if we have not already maxed our selection.
		{
			m_SelectedObjects.push_back(const_cast<ISelectable*>(Selectable));
			IReferenceable* AsReferenceable = dynamic_cast<IReferenceable*>(Selectable);
			if(AsReferenceable)
				AsReferenceable->AcquireReference();
			Selectable->SetSelected(true);
			SelectionChanged.Trigger(this);
		}
	}
}

void CCursor::Deselect(ISelectable* const Selectable)
{
	m_SelectedObjects.remove(const_cast<ISelectable*>(Selectable));
	IReferenceable* AsReferenceable = dynamic_cast<IReferenceable*>(Selectable);
	if(AsReferenceable)
		AsReferenceable->ReleaseReference();
	Selectable->SetSelected(false);
	SelectionChanged.Trigger(this);
}

void CCursor::ToggleSelection(ISelectable* const  Selectable)
{
	if(Selectable->IsSelected())
		Deselect(Selectable);
	else
		Select(Selectable);
}

void CCursor::DeselectAll()
{
	for(std::list<ISelectable*>::iterator Iterator = m_SelectedObjects.begin(); Iterator != m_SelectedObjects.end(); ++Iterator)
	{
		IReferenceable* AsReferenceable = dynamic_cast<IReferenceable*>((*Iterator));
		if(AsReferenceable)
			AsReferenceable->ReleaseReference();
		(*Iterator)->SetSelected(false);
	}
	m_SelectedObjects.clear();
	SelectionChanged.Trigger(this);
}

void CCursor::OnClick()
{
	if(m_ClickFunctor) // Overriding click function exists.
	{
		(*m_ClickFunctor)();
	}
	else // No override present.
	{
		if(m_DirectInput->MouseButtonPressedEx(0) || XBOX->GetController(0)->IsDown_A()) // Left Click
		{
			if(m_CurrentlyHoveredOver) // If the cursor is clocked over something.
			{
				ISelectable* Selectable = dynamic_cast<ISelectable*>(m_CurrentlyHoveredOver);
				if(Selectable) // If the hovered over object is selectable.
				{
					if(m_DirectInput->KeyDown(DIK_LSHIFT) || m_DirectInput->KeyDown(DIK_RSHIFT)) // If the toggle-selection modifier is down.
					{
						if(Selectable->GetTeamValue() != TEAM_ENEMY) // If the selectable is not hostile.
							ToggleSelection(Selectable); // Togle selection.
					}
					else // A selectable was clicked.
					{
						DeselectAll();
						if(Selectable->GetTeamValue() != TEAM_ENEMY) // If the selectable is not hostile.
							Select(Selectable);
					}
				}

				IClickable* Clickable = dynamic_cast<IClickable*>(m_CurrentlyHoveredOver);
				if(Clickable)
				{
					Clickable->OnClick(); // Trigger the clickable object.
				}
				else if(!Selectable)
				{
					DeselectAll();
				}
			}
			else if (!(m_DirectInput->KeyDown(DIK_LSHIFT) || m_DirectInput->KeyDown(DIK_RSHIFT))) // If the cursor was clicked over nothing and was not meant to toggle.
			{
				DeselectAll();
			}
		}
		else if(m_DirectInput->MouseButtonPressedEx(1) || XBOX->GetController(0)->IsDown_B()) // Right Click
		{
			for(std::list<ISelectable*>::iterator Iterator = m_SelectedObjects.begin(); Iterator != m_SelectedObjects.end(); ++Iterator)
				(*Iterator)->ContextSensitiveClick(); // Context sensitive command to all selected objects.
		}
	}
}

void CCursor::OnDrag()
{
	if(m_DragFunctor) // Overriding drag function exists.
	{
		(*m_DragFunctor)();
	}
	else if(m_BeingDragged) // Continue dragging object.
	{
		m_BeingDragged->OnDrag();
	}
	else if(m_DragRectangle) // Continue marquee selection.
	{
		m_DragRectangle->right = (LONG)GetPositionX();
		m_DragRectangle->bottom = (LONG)GetPositionY();
	}
	else // Beginning of a drag action.
	{
		if(m_DirectInput->MouseButtonDown(0) || XBOX->GetController(0)->IsDown_A()) // Left mouse button drag.
		{
			IDraggable* AsDraggable = dynamic_cast<IDraggable*>(m_CurrentlyHoveredOver);
			if(AsDraggable) // Hovering over something draggable.
			{
				CCookingResource* testCR = NULL;
				testCR = dynamic_cast<CCookingResource*>(AsDraggable);
				if(testCR && testCR->GetStoveOwner()->GetTeamValue() == TEAM_ENEMY)	
				{				
				}
				else
				{
					IReferenceable* AsReferenceable = dynamic_cast<IReferenceable*>(AsDraggable);
					if(AsReferenceable)
						AsReferenceable->AcquireReference();
					m_BeingDragged = AsDraggable;
					AsDraggable->OnBeginDrag(); // Start dragging an object.
				}
			}
			else if(!dynamic_cast<CGUIObject*>(m_CurrentlyHoveredOver) && m_CanDragSelect) // Begin drag selection.
			{
				m_DragRectangle = new RECT;
				m_DragRectangle->left = (LONG)GetPositionX();
				m_DragRectangle->top = (LONG)GetPositionY();
				m_DragRectangle->right = (LONG)GetPositionX();
				m_DragRectangle->bottom = (LONG)GetPositionY();
			}
		}
	}
}

void CCursor::OnRelease()
{
	if(m_ReleaseFunctor) // Overriding release function exists.
	{
		(*m_ReleaseFunctor)();
	}
	IClickable* AsClickable = NULL;
	if(m_BeingDragged) // End dragging object.
	{
		m_BeingDragged->OnEndDrag();
		IReferenceable* AsReferenceable = dynamic_cast<IReferenceable*>(m_BeingDragged);
		if(AsReferenceable)
			AsReferenceable->ReleaseReference();
		m_BeingDragged = NULL;
	}
	else if(m_DragRectangle) // End marquee selection.
	{
		if(m_DragRectangle->left > m_DragRectangle->right) // Normalize drag rectangle's width.
		{
			long Swap = m_DragRectangle->left;
			m_DragRectangle->left = m_DragRectangle->right;
			m_DragRectangle->right = Swap;
		}
		if(m_DragRectangle->top > m_DragRectangle->bottom) // Normalize drag rectangle's height.
		{
			long Swap = m_DragRectangle->top;
			m_DragRectangle->top = m_DragRectangle->bottom;
			m_DragRectangle->bottom = Swap;
		}

		GetObjectManagerOwner()->Iterate<CCursor>(&DragDetection, this); // Iterate through objects to detect what's being marquee selected.

		// Get rid of the drag rectangle.
		delete m_DragRectangle;
		m_DragRectangle = NULL;
	}
	else if(AsClickable = dynamic_cast<IClickable*>(m_CurrentlyHoveredOver))
	{
		if(AsClickable->IsClicked())
			AsClickable->OnRelease();
	}
}

void CCursor::OverrideClick(IFunctor* const OverrideFunctor)
{
	m_ClickFunctor = const_cast<IFunctor*>(OverrideFunctor);
}

void CCursor::OverrideDrag(IFunctor* const OverrideFunctor)
{
	m_DragFunctor = const_cast<IFunctor*>(OverrideFunctor);
}

void CCursor::OverrideRelease(IFunctor* const OverrideFunctor)
{
	m_ReleaseFunctor = const_cast<IFunctor*>(OverrideFunctor);
}

void CCursor::OverrideDragObject(IDraggable* _ObjectToDrag)
{
	//Stop dragging the current object if there is one
	if(m_BeingDragged)
	{
		m_BeingDragged->OnEndDrag();
		IReferenceable* AsReferenceable = dynamic_cast<IReferenceable*>(m_BeingDragged);
		if(AsReferenceable)
			AsReferenceable->ReleaseReference();
		m_BeingDragged = NULL;
	}

	//Begin dragging another object
	IReferenceable* AsReferenceable = dynamic_cast<IReferenceable*>(_ObjectToDrag);
	if(AsReferenceable)
		AsReferenceable->AcquireReference();
	m_BeingDragged = _ObjectToDrag;
	m_BeingDragged->OnBeginDrag(); // Start dragging an object.
}

bool CCursor::HoverDetection(IObject* Object, CCursor* Caller)
{
	CCursor* Cursor = CCursor::GetInstance();

	RECT IntersectionRectangle;

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-005
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Added CCookingUnit dynamic cast check
	RECT ObjectRectangle = dynamic_cast<CUnitObject*>(Object) || dynamic_cast<CCookingUnit*>(Object) ? Object->GetDrawRectangle() : Object->GetCollisionRectangle();

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-005
	//////////////////////////////////////////////////////////////////////////

	if(dynamic_cast<CGUIObject*>(Object))
	{
		ObjectRectangle.left += (LONG)CAMERA->GetPositionX();
		ObjectRectangle.right += (LONG)CAMERA->GetPositionX();
		ObjectRectangle.top += (LONG)CAMERA->GetPositionY();
		ObjectRectangle.bottom += (LONG)CAMERA->GetPositionY();
	}

	if(IntersectRect(&IntersectionRectangle, &Cursor->GetCollisionRectangle(), &ObjectRectangle))
	{
		Cursor->m_CurrentlyHoveredOver = Object; // There is no currently hovered over object until "HoverDetection" occurs.
		Object->AcquireReference();
		return true;
	}
	return false;
}

bool CCursor::DragDetection(IObject* Object, CCursor* Caller)
{
	CCursor* Cursor = CCursor::GetInstance();

	RECT IntersectionRectangle;
	RECT ObjectRectangle = dynamic_cast<CUnitObject*>(Object) ? Object->GetDrawRectangle() : Object->GetCollisionRectangle();
	if(IntersectRect(&IntersectionRectangle, Cursor->m_DragRectangle, &ObjectRectangle))
	{
		ISelectable* AsSelectable = dynamic_cast<ISelectable*>(Object);
		if(AsSelectable)
		{
			if(AsSelectable->GetTeamValue() != TEAM_ENEMY)
				Cursor->Select(AsSelectable);
		}
	}
	return false;
}

std::list<ISelectable*>& CCursor::GetSelectedObjects() const
{
	return const_cast<std::list<ISelectable*>&>(m_SelectedObjects);
}

IObject* CCursor::GetHoveredOverObject() const
{
	return m_CurrentlyHoveredOver;
}

IObject* CCursor::CursorFactoryMethod(CObjectManager* ObjectManager)
{
	// Find sprite.
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/OvenMittCursor.spr");
	if(!Sprite)
		return NULL;

	// Create cursor.
	CCursor* Cursor = CCursor::GetInstance();
	Cursor->SetSprite(Sprite);

	return Cursor;
}

void CCursor::EnableDragSelection()
{
	m_CanDragSelect = true;
}

void CCursor::DisableDragSelection()
{
	m_CanDragSelect = false;
}