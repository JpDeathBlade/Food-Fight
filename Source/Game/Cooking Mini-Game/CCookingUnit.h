//////////////////////////////////////////////////////
// File: "CCookingUnit.h"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#ifndef CCOOKINGUNIT_H_
#define CCOOKINGUNIT_H_

#include "../Objects/Base/CGameObject.h"
#include "../Objects/GUI Objects/CursorInterfaces.h"
#include "../../Managers/Bad Chef/CObjectManager.h"
#include "../../Managers/Bad Chef/Events/CEvent.h"

class CPan;
class CUnitObject;
class CFlipAction;

class CCookingUnit : public CGameObject, public IDraggable, public IHoverable
{
private:
	std::string m_CreationString;
	bool bDone;
	bool bActionDone;
	CPan* m_PanOwner;
	CFlipAction* m_FlipAction;
	float m_ScrambleTimer;
	float m_Rotation;
public:
	CCookingUnit();

	CUnitObject* SpawnUnit();
	void OnClick();
	void OnCollision(IObject* Object, D3DXVECTOR2 Overlap);
	void OnRelease() {}
	void OnDrag() {}
	void OnBeginDrag();
	void OnEndDrag();
	void OnBeginHover();
	void OnEndHover();

	void Update(float dt);
	void Draw();
	void StrictDraw();
	void EndFlip();

	void StartFlip();

	void SetCreationString(std::string _create);
	void SetFinished(bool _done) {bDone = _done;}
	void SetPanOnwer(CPan* _pan) { m_PanOwner = _pan; }
	void SetActionDone() { bActionDone = true; }
	
	std::string GetCreationString() { return m_CreationString; }
	bool GetActionDone() { return bActionDone; }
	CPan* GetPanOwner() { return m_PanOwner; }
};

IObject* CreateCookingUnitFactoryMethod(CObjectManager* objMan);
#endif