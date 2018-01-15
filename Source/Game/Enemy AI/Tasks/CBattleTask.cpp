//////////////////////////////////////////////////////
// File: "CBattleTask.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CBattleTask.h"
#include "../../Objects/Units/CUnitObject.h"
#include "../../Unit Commands/CAttackCommand.h"
#include "../../Unit Commands/CAttackMoveCommand.h"
#include "../../Objects/Appliances/CStove.h"

CBattleTask::CBattleTask()
{

}

CBattleTask::CBattleTask(const CBattleTask& Source) : IAITask(Source)
{

}

CBattleTask& CBattleTask::operator=(const CBattleTask& Source)
{
	IAITask::operator =(Source);
	return *this;
}

CBattleTask::~CBattleTask()
{

}

bool CBattleTask::Process()
{
	CEnemyArtificialIntelligence* EnemyAI = GetEnemyAI();
	IAITask::Process();

	std::list<CUnitObject*>& AssignedUnits = GetAssignedUnits();
	for(std::list<CUnitObject*>::iterator i = AssignedUnits.begin(); i != AssignedUnits.end(); ++i)
	{
            CAttackMoveCommand* AsAttackMoveCommand = dynamic_cast<CAttackMoveCommand*>((*i)->GetUnitAICommand());
			if(AsAttackMoveCommand && AsAttackMoveCommand->GetDestination() == AssignedUnits.front()->GetPosition()) // If the unit is already executing this task...
				continue; // Skip to the next unit.
			else // The unit is not executing this task.
				(*i)->SetUnitAICommand(new CAttackMoveCommand(AssignedUnits.front()->GetPosition(), AssignedUnits.front()->GetObjectManagerOwner())); // Command the unit to execute this task.
	}
	if(AssignedUnits.front()->GetObjectManagerOwner()->IterateLayer<CBattleTask>(1, &ScanForEnemyAttackers, this)) // Enemies still within range.
		return false;
	else
		return true;
}

bool CBattleTask::ScanForEnemyAttackers(IObject* Object, CBattleTask* Sender)
{
	CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(Object);
	if(AsUnitObject && AsUnitObject->GetTeamValue() == TEAM_PLAYER && D3DXVec2LengthSq(&(AsUnitObject->GetPosition() - Sender->GetAssignedUnits().front()->GetPosition())) <= 1000000)
	{
		return true;
	}
	return false;
}