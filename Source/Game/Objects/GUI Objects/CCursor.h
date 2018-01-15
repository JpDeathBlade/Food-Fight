#ifndef _CCursor_H_
#define _CCursor_H_
#include "CGUIObject.h"
#include "../Base/IFunctor.h"
#include "../../../Managers/Bad Chef/Events/CEvent.h"
#include <list>

class CSGD_DirectInput;
class CObjectManager;
class ISelectable;
class IDraggable;

class CCursor : public CGUIObject
{
	static CCursor* m_Instance;
	CSGD_DirectInput* m_DirectInput;
	std::list<ISelectable*> m_SelectedObjects;
	IObject* m_CurrentlyHoveredOver;
	IObject* m_PreviouslyHoveredOver;
	IDraggable* m_BeingDragged;
	IFunctor* m_ClickFunctor;
	IFunctor* m_DragFunctor;
	IFunctor* m_ReleaseFunctor;
	bool m_CanDragSelect;
	RECT* m_DragRectangle;

	CCursor(); // Default Constructor
	CCursor(const CCursor& Source); // Copy Constructor - Disabled
	CCursor& operator=(const CCursor& Source); // Assignment Operator - Disabled
	~CCursor(); // Destructor

	void OnClick(); // Reacts to mouse buttons being clicked.
	void OnDrag(); // Reacts to mouse buttons behing held.
	void OnRelease(); // Reacts to mouse buttons being released.

	static bool HoverDetection(IObject* Object, CCursor* Caller); // Detects which object the cursor is over and stores it.
	static bool DragDetection(IObject* Object, CCursor* Caller); // Detects which objects are contained by the marquee selection rectangle.
public:
	static const unsigned char MaxSelectables = 12;
	CEvent<CCursor> SelectionChanged;

	static CCursor* GetInstance(); // Returns the singleton's instance.
	static void DeleteInstance(); // Deletes the singleton's instance.

	void Input(); // Detects input from the mouse buttons and acts upon it.
	void Update(float Delta); // Updates position based on mouse movement.
	void Draw(); // Draws the cursor.

	void SetPosition(D3DXVECTOR2 Position); // Clamps to the camera.
	void SetPosition(float X, float Y); // Clamps to the camera.
	void SetPositionX(float X); // Clamps to the camera.
	void SetPositionY(float Y); // Clamps to the camera.

	void ResetSprite(); // Reset the cursor's sprite to its original sprite.

	void Select(ISelectable* const Selectable); // Attempts to select the given object.
	void Deselect(ISelectable* const Selectable); // Deselects the given object.
	void ToggleSelection(ISelectable* const Selectable); // Attempts to select the given object if it is not selected. Deselects the given object if it is.
	void DeselectAll(); // Deselects every object selected.

	void OverrideClick(IFunctor* const OverrideFunctor); // Overrides the cursor's behavior when clicking.
	void OverrideDrag(IFunctor* const OverrideFunctor); // Overrides the cursor's behavior when dragging.
	void OverrideRelease(IFunctor* const OverrideFunctor); // Overrides the cursor's behavior when released.

	void OverrideDragObject(IDraggable* _ObjectToDrag); //Sets the currently dragging object to whatever is passed in

	void EnableDragSelection();
	void DisableDragSelection();

	std::list<ISelectable*>& GetSelectedObjects() const; // Returns a list of the currently selected objects.
	IObject* GetHoveredOverObject() const; // Returns the object currently being hovered over.

	static IObject* CursorFactoryMethod(CObjectManager* ObjectManager); // Factory method for the creation of the cursor.
};
#endif