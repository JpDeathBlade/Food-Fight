//////////////////////////////////////////////////////
// File: "CToaster.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CToaster.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"


void CToaster::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{
	
}

IObject* CreateToasterFactoryMethod(CObjectManager* pManager)
{
	CSprite* Sprite = CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Appliances/Toaster.spr");
	if(!Sprite)
		return NULL;

	CToaster* pToaster = new CToaster();
	pToaster->SetSprite(Sprite);
	pManager->AddObjectToLayer(1, pToaster);
	return pToaster;
}

/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BUG-006
// BUG FIX START
/////////////////////////////////////////////////////////////////
float CToaster::GetZDepth() const
{
	return CGameObject::GetZDepth();
}
///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BUG-006
//////////////////////////////////////////////////////////////////////////