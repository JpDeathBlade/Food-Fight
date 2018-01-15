//////////////////////////////////////////////////////
// File: "IAITask.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _IAITask_H_
#define _IAITask_H_
#include <list>

class CUnitObject;
class CEnemyArtificialIntelligence;

class IAITask
{
	unsigned char m_Priority;
	CEnemyArtificialIntelligence* m_EnemyAI;
	std::list<CUnitObject*> m_AssignedUnits;
public:
	IAITask();
	IAITask(const IAITask& Source);
	virtual IAITask& operator=(const IAITask& Source);
	virtual ~IAITask();

	virtual bool Process() = 0;

	unsigned char GetPriority() const;
	void SetPriority(unsigned char Priority);

	CEnemyArtificialIntelligence* GetEnemyAI() const;
	virtual void SetEnemyAI(CEnemyArtificialIntelligence* const EnemyAI);

	std::list<CUnitObject*>& GetAssignedUnits();

	virtual void AssignUnit(CUnitObject* UnitObject);
	virtual bool UnassignUnit(CUnitObject* UnitObject);
};
#endif