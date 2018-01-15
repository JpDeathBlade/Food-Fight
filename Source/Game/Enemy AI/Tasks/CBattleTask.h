//////////////////////////////////////////////////////
// File: "CBattleTask.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _CBattleTask_H_
#define _CBattleTask_H_

#include "IAITask.h"

class CEnemyAI;
class IObject;

class CBattleTask : public IAITask
{
public:
	CBattleTask();
	CBattleTask(const CBattleTask& Source);
	CBattleTask& operator=(const CBattleTask& Source);
	~CBattleTask();

	bool Process();

	static bool ScanForEnemyAttackers(IObject* Object, CBattleTask* Sender);
};
#endif