//////////////////////////////////////////////////////
// File: "CStove.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef CSTOVE_H_
#define CSTOVE_H_

#include "../Base/CGameObject.h"
#include "../GUI Objects/CursorInterfaces.h"
#include "../Base/IAttackable.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include "../../Cooking Mini-Game/CCookingResource.h"
#include <list>

class CPan;

class CStove : public CGameObject, public ISelectable, public IAttackable, public IHoverable 
{
public:
	CStove();
	~CStove();
	void InitializeCookingMinigame(bool _facingRight, CObjectManager* objMan);
	void AddCookingResource(RESOURCE_TYPE _type, CObjectManager* objMan);
	void SubtractCookingResource(RESOURCE_TYPE _type);

	void SetSpawnRect(RECT _spawn) { m_SpawnRect = _spawn; }
	void SetSpawnX(int _spawnX) { m_SpawnRect.left = _spawnX; }
	void SetSpawnY(int _spawnY) { m_SpawnRect.top = _spawnY; }

	CPan* GetTopLeftPan() { return m_TopLeftPan; }
	CPan* GetTopRightPan() { return m_TopRightPan; }
	CPan* GetBottomLeftPan() { return m_BottomLeftPan; }
	CPan* GetBottomRightPan() { return m_BottomRightPan; }
	CGameObject* GetFruitPlate() { return m_FruitPlate; }
	CGameObject* GetMeatPlate() { return m_MeatPlate; }
	CGameObject* GetWheatPlate() { return m_WheatPlate; }
	CGameObject* GetEggPlate() { return m_EggPlate; }
	CGameObject* GetPlate(RESOURCE_TYPE _type);
	RECT GetSpawnRect();

	void OnCollision(IObject* Object, D3DXVECTOR2 Overlap);

	//Network Functions
	void SendUpdatePacket() {}
	void Update(float dt);
	void UpdateNetworked(float dt) {}

	int GetEggCount() const { return m_EggCount; }
	int GetFruitCount() const { return m_FruitCount; }
	int GetMeatCount() const { return m_MeatCount; }
	int GetWheatCount() const { return m_WheatCount; }

	CCookingResource* GetEgg();
	CCookingResource* GetFruit();
	CCookingResource* GetMeat();
	CCookingResource* GetWheat();

	void OnBeginHover();
	void OnEndHover();
private:
	//Resource counts 
	int m_EggCount;
	int m_FruitCount;
	int m_MeatCount;
	int m_WheatCount;

	bool m_bAlive;

	CGameObject* m_EggPlate;
	std::list<CCookingResource*> m_EggResources;

	CGameObject* m_FruitPlate;
	std::list<CCookingResource*> m_FruitResources;

	CGameObject* m_MeatPlate;
	std::list<CCookingResource*> m_MeatResources;

	CGameObject* m_WheatPlate;
	std::list<CCookingResource*> m_WheatResources;

	CPan* m_TopLeftPan;
	CPan* m_TopRightPan;
	CPan* m_BottomLeftPan;
	CPan* m_BottomRightPan;

	RECT m_SpawnRect;
};


IObject* CreateStoveFactoryMethod(CObjectManager* objManager);
IObject* CreateResourcePlateFactoryMethod(CObjectManager* objManager);

#endif