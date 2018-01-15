//////////////////////////////////////////////////////
// File: "CFlipAction.h"
// Creator: MG
// Creation Date: 10/03/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////

#include "../Objects/Base/IObject.h"
#include "CCookingUnit.h"

class CFlipAction
{
private:
	CCookingUnit* m_Unit;
	bool m_bReleased;
	D3DXVECTOR2 m_StartFlip;
	D3DXVECTOR2 m_EndFlip;
public:
	CFlipAction();
	void Update(float dt);
	void Draw();

	void SetCookingUnit(CCookingUnit* _unit) { m_Unit = _unit; }

	void SetStartFlip(float x, float y) { m_StartFlip.x = x; m_StartFlip.y = y; }

	void FinishFlip();

	CCookingUnit* GetCookingUnit() { return m_Unit; }
};