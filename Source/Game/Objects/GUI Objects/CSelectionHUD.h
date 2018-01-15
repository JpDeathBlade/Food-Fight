#ifndef _CSelectionHUD_H
#define _CSelectionHUD_H
#include "CGUIObject.h"
#include "CursorInterfaces.h"
#include "CCursor.h"
#include "../../../Managers/Bad Chef/Events/CEvent.h"

class CUnitObject;

class CUnitIcon : public CGUIObject
{
	CUnitObject* m_UnitObject;
public:
	static const unsigned char Width = 48;
	static const unsigned char Height = 52;

	CUnitIcon(); // Default Constructor
	CUnitIcon(const CUnitIcon& Source); // Copy Constructor
	CUnitIcon& operator=(const CUnitIcon& Source); // Assignment Operator
	~CUnitIcon(); // Destructor

	void Draw();

	CUnitObject* GetSelectedUnit() const;
	void SetSelectedUnit(CUnitObject* const UnitObject);
};

class CObjectManager;
class CCursor;

class CSelectionHUD : public CGUIObject, public IClickable
{
	static CSelectionHUD* m_Instance;
	CUnitIcon m_UnitIcons[CCursor::MaxSelectables];
	CUnitObject* m_SubSelected;

	class OnSelectionChangeFunctor : public CEventFunctor<CCursor>
	{
		void operator()();
	}; 
	OnSelectionChangeFunctor OnSelectionChange;

	CSelectionHUD(); // Default Constructor
	CSelectionHUD(const CSelectionHUD& Source); // Copy Constructor - Disabled
	CSelectionHUD& operator=(const CSelectionHUD& Source); // Assignment Operator - Disabled
	~CSelectionHUD(); // Destructor
public:
	static CSelectionHUD* GetInstance();
	static void DeleteInstance();
	CEvent<CSelectionHUD> SubSelectionChanged;

	CUnitObject* GetSubSelected() const;
	void SetSubSelected(CUnitObject* const UnitObject);

	void Update(float Delta);
	void Draw();
	RECT GetDrawRectangle() const;
	RECT GetCollisionRectangle() const;

	void OnClick();

	static IObject* CreateSelectionHUD(CObjectManager* ObjectManager);
};
#endif