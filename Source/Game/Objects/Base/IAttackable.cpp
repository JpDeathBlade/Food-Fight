#include "IAttackable.h"
#include "../../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include "../Units/CUnitObject.h"

void IAttackable::ApplyDamage(short Damage, bool fromPacket)
{
	NW_MANAGER_START
	if(CNetworkManager::GetInstance()->IsConnected())
	{
		if(!this->GetNWOwner() || fromPacket)
		{
			m_Health -= Damage;
		}
		if(!fromPacket)
		{
#if _DEBUG
			CUnitObject* testUnit = NULL;
			if(testUnit = dynamic_cast<CUnitObject*>(this))
			{
				printf("%d damage sent to \"%s\"\n", Damage, testUnit->GetCreationString().c_str());
			}
#endif
			CNetworkManager::GetInstance()->SendDealDamagePacket(0, this->GetSharedID(), Damage);
		}
	}
	else
	{
		m_Health -= Damage;
	}
	NW_MANAGER_END
}