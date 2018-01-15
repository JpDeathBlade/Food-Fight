//////////////////////////////////////////////////////
// File: "CDefendStoveTask.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _CDefendStoveTask_H_
#define _CDefendStoveTask_H_
#include "IAITask.h"

class CStove;
class IObject;

class CDefendStoveTask : public IAITask
{
	CStove* m_TargetStove;
	unsigned char m_FriendlyAttackThreatLevel;
	unsigned char m_EnemyDefenseThreatLevel;
public:
	CDefendStoveTask();
	CDefendStoveTask(CStove* const TargetStove);
	CDefendStoveTask(const CDefendStoveTask& Source);
	CDefendStoveTask& operator=(const CDefendStoveTask& Source);
	~CDefendStoveTask();

	bool Process();

	void SetTargetStove(CStove* const TargetStove);
	static bool ScanForEnemyAttackers(IObject* Object, CDefendStoveTask* Sender);
};
#endif