//////////////////////////////////////////////////////
// File: "CCapturePlateTask.h"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _CCapturePlateTask_H_
#define _CCapturePlateTask_H_
#include "IAITask.h"

class CPlate;
class IObject;

class CCapturePlateTask : public IAITask
{
	CPlate* m_TargetPlate;
	unsigned char m_FriendlyAttackThreatLevel;
	unsigned char m_UnitsAlreadyRequested;
	unsigned char m_EnemyDefenseThreatLevel;
public:
	CCapturePlateTask();
	CCapturePlateTask(CPlate* const TargetPlate);
	CCapturePlateTask(const CCapturePlateTask& Source);
	CCapturePlateTask& operator=(const CCapturePlateTask& Source);
	~CCapturePlateTask();

	bool Process();

	void AssignUnit(CUnitObject* UnitObject);

	void SetTargetPlate(CPlate* const TargetPlate);
	static bool ScanForEnemyDefenses(IObject* Object, CCapturePlateTask* Sender);
};
#endif