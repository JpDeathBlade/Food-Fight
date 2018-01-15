//////////////////////////////////////////////////////
// File: "CCoffeeMaker.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: 9/21/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CCOFFEEMAKER_H_
#define CCOFFEEMAKER_H_

#include "../Base/CGameObject.h"
#include "../GUI OBjects/CursorInterfaces.h"

class CObjectManager;

class CCoffeeMaker : public CGameObject, public ISelectable
{
private:
public:
	////////////////Constructor and Destructor////////////////
	CCoffeeMaker(void);
	~CCoffeeMaker(void);

	void OnCollision(IObject* Object, D3DXVECTOR2 Overlap);
};

IObject* CreateCoffeeMakerFactoryMethod(CObjectManager* pManager);

#endif