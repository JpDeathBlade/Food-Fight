//////////////////////////////////////////////////////
// File: "IAttackable.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: 9/17/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef IATTACKABLE_H_
#define IATTACKABLE_H_

#include "../../../Managers/Bad Chef/Networking/INetworkable.h"

class IAttackable : public INetworkable
{
private:
	short m_Health;
	unsigned short m_MaxHealth;
public:
	//constructor and destructor
	IAttackable()				{ m_Health = 0; m_MaxHealth = 0; }
	virtual ~IAttackable() = 0 {}

	//Accessors and mutators
	short GetHealth() const
	{
		return m_Health;
	}

	virtual void SetHealth(unsigned short Health)	
	{
		m_Health = Health < m_MaxHealth ? Health : m_MaxHealth;
	}

	unsigned short GetMaxHealth() const
	{
		return m_MaxHealth;
	}

	virtual void SetMaxHealth(unsigned short MaxHealth)	
	{
		m_MaxHealth = MaxHealth;
		SetHealth(GetHealth());
	}

	virtual void ApplyDamage(short Damage, bool fromPacket = false);
};
#endif