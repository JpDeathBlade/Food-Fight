//////////////////////////////////////////////////////
// File: "CPan.h"
// Creator: MG
// Creation Date: 9/20/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef CPAN_H_
#define CPAN_H_

#include "../../Managers/Bad Chef/CObjectManager.h"
#include "../Objects/Base/CGameObject.h"
#include "CCookingResource.h"
#include "CRecipeManager.h"
#include "../Objects/GUI Objects/CursorInterfaces.h"
#include "../../Managers/Bad Chef/Events/CEvent.h"

class CCookingUnit;
class CStove;
class CParticleEmitter;

enum EPanStage { PAN_NONE, PAN_COMBINE, PAN_COOKING, PAN_DONE };

class CPan : public CGameObject, public IClickable, public IHoverable
{
private:
	std::vector<CCookingResource*> resourcesAdded;
	std::vector<RESOURCE_TYPE> m_ActiveSpices;
	EPanStage m_PanStage;
	float m_StageTimer;
	CRecipe* m_ActiveRecipe;
	CCookAction* m_ActiveAction;
	CCookingUnit* m_CookingUnit;
	CStove* m_Stove;
	D3DCOLOR m_DrawColor;
	CParticleEmitter* m_pSmoke;
public:
	CPan();
	~CPan();
	
	void Update(float Delta);
	void Draw();
	void OnClick();
	void Reset();

	void AddResource(CCookingResource* _type);

	CEvent<CPan> ReachedDoneStage;
	EPanStage GetPanStage()	{ return m_PanStage; }
	void SetPanStage(EPanStage _stage);

	float GetStageTimer() { return m_StageTimer; }
	CCookAction* GetActiveAction() { return m_ActiveAction; }

	std::vector<RESOURCE_TYPE>* GetActiveSpices() { return &m_ActiveSpices; }
	CRecipe* GetActiveRecipe() { return m_ActiveRecipe; }

	CCookingUnit* GetCookingUnit();

	CStove* GetStove() const;
	void SetStove(CStove* const Stove);

	void OnBeginHover();
	void OnEndHover();
};

IObject* CreatePanFactoryMethod(CObjectManager* objMan);
#endif