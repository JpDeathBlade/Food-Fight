//////////////////////////////////////////////////////
// File: "CAttackStoveTask.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CAttackStoveTask.h"
#include "../CEnemyArtificialIntelligence.h"
#include "../../Objects/Units/CUnitObject.h"
#include "../../Unit Commands/CAttackCommand.h"
#include "../../Unit Commands/CAttackMoveCommand.h"
#include "../../Objects/Appliances/CStove.h"
#include "../../Cooking Mini-Game/CRecipeManager.h"

CAttackStoveTask::CAttackStoveTask()
{
	m_TargetStove = NULL;
	m_EnemyDefenseThreatLevel = 0;
	m_UnitsRequested = 0;
	m_FriendlyAttackThreatLevel = 0;
}

CAttackStoveTask::CAttackStoveTask(CStove* const TargetStove)
{
	SetTargetStove(TargetStove);
	m_EnemyDefenseThreatLevel = 0;
	m_UnitsRequested = 0;
	m_FriendlyAttackThreatLevel = 0;
}

CAttackStoveTask::CAttackStoveTask(const CAttackStoveTask& Source) : IAITask(Source)
{
	SetTargetStove(Source.m_TargetStove);
	m_EnemyDefenseThreatLevel = Source.m_EnemyDefenseThreatLevel;
	m_UnitsRequested = Source.m_UnitsRequested;
	m_FriendlyAttackThreatLevel = Source.m_FriendlyAttackThreatLevel;
}

CAttackStoveTask& CAttackStoveTask::operator=(const CAttackStoveTask& Source)
{
	IAITask::operator =(Source);
	SetTargetStove(Source.m_TargetStove);
	m_EnemyDefenseThreatLevel = Source.m_EnemyDefenseThreatLevel;
	m_UnitsRequested = Source.m_UnitsRequested;
	m_FriendlyAttackThreatLevel = Source.m_FriendlyAttackThreatLevel;
	return *this;
}

CAttackStoveTask::~CAttackStoveTask()
{

}

bool CAttackStoveTask::Process()
{
	CEnemyArtificialIntelligence* EnemyAI = GetEnemyAI();
	IAITask::Process();

	m_EnemyDefenseThreatLevel = 0;
	m_TargetStove->GetObjectManagerOwner()->IterateLayer<CAttackStoveTask>(1, &ScanForEnemyDefenses, this);
	m_EnemyDefenseThreatLevel = max(1, m_EnemyDefenseThreatLevel);

	m_FriendlyAttackThreatLevel = GetAssignedUnits().size();
	if(m_EnemyDefenseThreatLevel > m_FriendlyAttackThreatLevel)
	{
		unsigned char UnitsToRequest = 0;
		unsigned char UnitsAlreadyRequested = m_UnitsRequested;
		for(unsigned char i = 0; i < m_EnemyDefenseThreatLevel - m_FriendlyAttackThreatLevel; ++i)
		{
			CUnitObject* UnitObject = EnemyAI->GetUnit();
			if(UnitObject)
				AssignUnit(UnitObject);
			else
				++UnitsToRequest;
		}
		UnitsToRequest = max(0, UnitsToRequest - UnitsAlreadyRequested);
		if(UnitsToRequest)
		{
			std::vector<CRecipe*>& Recipes = CRecipeManager::GetInstance()->GetRecipes();
			for(unsigned char i = 0; i < UnitsToRequest; ++i)
			{
				EnemyAI->QueueRecipe(Recipes[rand() % Recipes.size()]);
			}
			m_UnitsRequested += UnitsToRequest;
		}
	}

	std::list<CUnitObject*>& AssignedUnits = GetAssignedUnits();
	for(std::list<CUnitObject*>::iterator i = AssignedUnits.begin(); i != AssignedUnits.end(); ++i)
	{
		CAttackMoveCommand* AsAttackMoveCommand = dynamic_cast<CAttackMoveCommand*>((*i)->GetUnitAICommand());
		if(AsAttackMoveCommand && AsAttackMoveCommand->GetDestination() == m_TargetStove->GetPosition()) // If the unit is already executing this task...
			continue; // Skip to the next unit.
		else // The unit is not executing this task.
			(*i)->SetUnitAICommand(new CAttackMoveCommand(m_TargetStove->GetPosition(), m_TargetStove->GetObjectManagerOwner())); // Command the unit to execute this task.
	}
	if(m_TargetStove->CheckRemove()) // Stove is dead.
		return true;
	else // Stove remains.
		return false;
}

void CAttackStoveTask::AssignUnit(CUnitObject* UnitObject)
{
	// TEMPORARY: Doesn't calculate a heuristic, just increases threat level.
	IAITask::AssignUnit(UnitObject);
	m_UnitsRequested = max(0, m_UnitsRequested - 1);
}

void CAttackStoveTask::SetTargetStove(CStove* const TargetStove)
{
	m_TargetStove = TargetStove;
}

bool CAttackStoveTask::ScanForEnemyDefenses(IObject* Object, CAttackStoveTask* Sender)
{
	CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(Object);
	if(AsUnitObject && AsUnitObject->GetTeamValue() == TEAM_PLAYER && D3DXVec2LengthSq(&(AsUnitObject->GetPosition() - Sender->m_TargetStove->GetPosition())) <= 1000000)
	{
		if(Sender)
		{
			++(Sender->m_EnemyDefenseThreatLevel);
		}
	}
	return false;
}