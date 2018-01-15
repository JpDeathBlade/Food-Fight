//////////////////////////////////////////////////////
// File: "CAttackStoveTask.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _CAttackStoveTask_H_
#define _CAttackStoveTask_H_
#include "IAITask.h"

class CStove;
class IObject;

class CAttackStoveTask : public IAITask
{
	CStove* m_TargetStove;
	unsigned char m_FriendlyAttackThreatLevel;
	unsigned char m_UnitsRequested;
	unsigned char m_EnemyDefenseThreatLevel;
public:
	CAttackStoveTask();
	CAttackStoveTask(CStove* const TargetStove);
	CAttackStoveTask(const CAttackStoveTask& Source);
	CAttackStoveTask& operator=(const CAttackStoveTask& Source);
	~CAttackStoveTask();

	bool Process();

	void AssignUnit(CUnitObject* UnitObject);

	void SetTargetStove(CStove* const TargetStove);
	static bool ScanForEnemyDefenses(IObject* Object, CAttackStoveTask* Sender);
};
#endif