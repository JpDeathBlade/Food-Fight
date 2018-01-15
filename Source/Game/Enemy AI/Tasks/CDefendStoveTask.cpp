//////////////////////////////////////////////////////
// File: "CDefendStoveTask.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CDefendStoveTask.h"
#include "../../Objects/Units/CUnitObject.h"
#include "../../Unit Commands/CAttackCommand.h"
#include "../../Unit Commands/CAttackMoveCommand.h"
#include "../../Objects/Appliances/CStove.h"

CDefendStoveTask::CDefendStoveTask()
{
	m_TargetStove = NULL;
	m_EnemyDefenseThreatLevel = 0;
	m_FriendlyAttackThreatLevel = 0;
}

CDefendStoveTask::CDefendStoveTask(CStove* const TargetStove)
{
	SetTargetStove(TargetStove);
	m_EnemyDefenseThreatLevel = 0;
	m_FriendlyAttackThreatLevel = 0;
}

CDefendStoveTask::CDefendStoveTask(const CDefendStoveTask& Source) : IAITask(Source)
{
	SetTargetStove(Source.m_TargetStove);
	m_EnemyDefenseThreatLevel = Source.m_EnemyDefenseThreatLevel;
	m_FriendlyAttackThreatLevel = Source.m_FriendlyAttackThreatLevel;
}

CDefendStoveTask& CDefendStoveTask::operator=(const CDefendStoveTask& Source)
{
	IAITask::operator =(Source);
	SetTargetStove(Source.m_TargetStove);
	m_EnemyDefenseThreatLevel = Source.m_EnemyDefenseThreatLevel;
	m_FriendlyAttackThreatLevel = Source.m_FriendlyAttackThreatLevel;
	return *this;
}

CDefendStoveTask::~CDefendStoveTask()
{

}

bool CDefendStoveTask::Process()
{
	CEnemyArtificialIntelligence* EnemyAI = GetEnemyAI();
	IAITask::Process();

	std::list<CUnitObject*>& AssignedUnits = GetAssignedUnits();
	for(std::list<CUnitObject*>::iterator i = AssignedUnits.begin(); i != AssignedUnits.end(); ++i)
	{
            CAttackMoveCommand* AsAttackMoveCommand = dynamic_cast<CAttackMoveCommand*>((*i)->GetUnitAICommand());
			if(AsAttackMoveCommand && AsAttackMoveCommand->GetDestination() == m_TargetStove->GetPosition()) // If the unit is already executing this task...
				continue; // Skip to the next unit.
			else // The unit is not executing this task.
				(*i)->SetUnitAICommand(new CAttackMoveCommand(m_TargetStove->GetPosition(), m_TargetStove->GetObjectManagerOwner())); // Command the unit to execute this task.
	}
	if(m_TargetStove->GetObjectManagerOwner()->IterateLayer<CDefendStoveTask>(1, &ScanForEnemyAttackers, this)) // Enemies still within range.
		return false;
	else if(m_TargetStove->CheckRemove()) // Stove is dead.
		return true;
	return false;
}

void CDefendStoveTask::SetTargetStove(CStove* const TargetStove)
{
	m_TargetStove = TargetStove;
}

bool CDefendStoveTask::ScanForEnemyAttackers(IObject* Object, CDefendStoveTask* Sender)
{
	CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(Object);
	if(AsUnitObject && AsUnitObject->GetTeamValue() == TEAM_PLAYER && D3DXVec2LengthSq(&(AsUnitObject->GetPosition() - Sender->m_TargetStove->GetPosition())) <= 1000000)
	{
		return true;
	}
	return false;
}