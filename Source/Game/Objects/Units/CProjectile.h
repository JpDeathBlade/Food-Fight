//////////////////////////////////////////////////////
// File: "CProjectile.h"
// Creator: MG
// Creation Date: 10/9/09
// Last Modified: 10/9/09
// Last Modified By: MG
//////////////////////////////////////////////////////

#include "..\Base\CGameObject.h"
#include "../../../Managers/Bad Chef/Networking/INetworkable.h"

class CUnitObject;
class CObjectManager;

class CProjectile : public CGameObject, public INetworkable
{
public:
	typedef void (*FunctionPointer)(CProjectile*);
private:
	CGameObject* m_UnitTracked;
	short m_Damage;
	float m_Speed;

public:
	static FunctionPointer FindHitFunction(short index);
	FunctionPointer OnHit;
	void SetupProjectile(D3DXVECTOR2 _startPos, CGameObject* _unitTracked, float _speed, short _damage, float fRotation = 0.0f);
	void Update(float Delta);
	void Draw(void);
	void OnCollision(IObject* Object, D3DXVECTOR2 Overlap);

	void SendCreationPacket(std::string _creationString, short _hitFunction);

	short GetDamage() { return m_Damage; }
	CGameObject* GetUnitTracked() { return m_UnitTracked; }
};

void BlueberryShrapnelHit(CProjectile* proj);
void WaffleAmberHit(CProjectile* proj);
void BaconArrowHit(CProjectile* proj);

IObject* CreateBlueberryShrapnel(CObjectManager* pManager);
IObject* CreateBaconArrow(CObjectManager* pManager);