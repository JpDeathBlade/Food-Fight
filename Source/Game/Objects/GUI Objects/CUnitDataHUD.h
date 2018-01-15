#ifndef _CUnitDataHUD_H
#define _CUnitDataHUD_H
#include "CGUIObject.h"
#include "CursorInterfaces.h"
#include "CButton.h"

class CObjectManager;

class CUnitDataHUD : public CGUIObject, public IHoverable
{
	static CUnitDataHUD* m_Instance;

	CButton m_UnitTooltip;

	CUnitDataHUD(); // Default Constructor
	CUnitDataHUD(const CUnitDataHUD& Source); // Copy Constructor - Disabled
	CUnitDataHUD& operator=(const CUnitDataHUD& Source); // Assignment Operator - Disabled
	~CUnitDataHUD(); // Destructor
public:
	static const unsigned char Width = 116;
	static const unsigned char Height = 128;

	static CUnitDataHUD* GetInstance();
	static void DeleteInstance();

	void Draw();
	RECT GetDrawRectangle() const;
	RECT GetCollisionRectangle() const;

	void OnHover();

	static IObject* CreateUnitDataHUD(CObjectManager* ObjectManager);
};
#endif