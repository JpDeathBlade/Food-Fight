//////////////////////////////////////////////////////
// File: "CSpiceHolder.h"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#ifndef CSPICEHOLDER_H_
#define CSPICEHOLDER_H_

#include "../Objects/Base/CGameObject.h"
#include "../Objects/GUI Objects/CursorInterfaces.h"
#include "CCookingResource.h"

class CStove;

class CSpiceHolder : public IClickable, public CGameObject, public IHoverable
{
	RESOURCE_TYPE m_SpawnedResource;
	CStove* m_StoveOwner;
public:
	CSpiceHolder(RESOURCE_TYPE _type);
	void OnClick();

	void SetStoveOwner(CStove* _stove) { m_StoveOwner = _stove; }

	void OnBeginHover();
	void OnEndHover();
};

IObject* CreateSaltHolderFactoryMethod(CObjectManager* objManager);
IObject* CreatePepperHolderFactoryMethod(CObjectManager* objManager);
IObject* CreateSugarHolderFactoryMethod(CObjectManager* objManager);
IObject* CreateCinnaHolderFactoryMethod(CObjectManager* objManager);
#endif