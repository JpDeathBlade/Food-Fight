//////////////////////////////////////////////////////
// File: "CToaster.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef CTOASTER_H_
#define CTOASTER_H_

#include "../Base/CGameObject.h"
#include "../GUI OBjects/CursorInterfaces.h"

class CToaster : public CGameObject, public ISelectable
{
private:

public:
	void OnCollision(IObject* Object, D3DXVECTOR2 Overlap);
	float GetZDepth() const;
};

IObject* CreateToasterFactoryMethod(CObjectManager* pManager);

#endif