#ifndef CSECONDARYRESOURCE_H_
#define CSECONDARYRESOURCE_H_

#include "../Objects/Base/CGameObject.h"
#include "../Objects/GUI Objects/CursorInterfaces.h"
#include "CCookingResource.h"

class CSecondaryResource : public IClickable, public CGameObject
{
private:
	RESOURCE_TYPE m_SpawnedResource;
	CCookingResource* m_ResourceToDrag;
public:
	CSecondaryResource(RESOURCE_TYPE _type);
	void OnClick();
};


#endif