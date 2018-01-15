#ifndef _CCookingResource_H_
#define _CCookingResource_H_
#include "../Objects/Base/CGameObject.h"
#include "../Objects/GUI Objects/CursorInterfaces.h"
#include "../../Managers/Bad Chef/CObjectManager.h"

class CStove;

enum RESOURCE_TYPE { RESOURCE_EGG, RESOURCE_WHEAT, RESOURCE_MEAT, RESOURCE_FRUIT, RESOURCE_MILK, RESOURCE_BUTTER, RESOURCE_SALT, RESOURCE_PEPPER, RESOURCE_SUGAR, RESOURCE_CINNA };

class CCookingResource : public CGameObject, public IDraggable, public IHoverable
{
private:
	CStove* m_StoveOwner;
	RESOURCE_TYPE m_RscType;
	bool m_bUsed;
	bool m_bDropped;
public:
	CCookingResource();
	CCookingResource(const CCookingResource& Source);
	CCookingResource& operator=(const CCookingResource& Source);
	~CCookingResource();

	void OnBeginDrag();
	void OnDrag();
	void OnEndDrag();

	void OnEndAnimate();

	void SetUsed(bool _used) { m_bUsed = _used; }
	void SetStoveOwner(CStove* _stove) { m_StoveOwner = _stove; }
	void SetResourceType(RESOURCE_TYPE _type) { m_RscType = _type; }

	RESOURCE_TYPE GetResourceType() const { return m_RscType; }
	CStove* GetStoveOwner() { return m_StoveOwner; }

	void OnBeginHover();
	void OnEndHover();

	float GetZDepth() const;
};

bool DropResourceOnUnit(IObject* obj, CCookingResource* _resource, bool fromPacket);

IObject* CreateFruitFactoryMethod(CObjectManager* objManager);
IObject* CreateMeatFactoryMethod(CObjectManager* objManager);
IObject* CreateWheatFactoryMethod(CObjectManager* objManager);
IObject* CreateEggFactoryMethod(CObjectManager* objManager);
IObject* CreateMilkFactoryMethod(CObjectManager* objManager);
IObject* CreateButterFactoryMethod(CObjectManager* objManager);
IObject* CreateCinnaFactoryMethod(CObjectManager* objManager);
IObject* CreateSugarFactoryMethod(CObjectManager* objManager);
IObject* CreateSaltFactoryMethod(CObjectManager* objManager);
IObject* CreatePepperFactoryMethod(CObjectManager* objManager);
#endif