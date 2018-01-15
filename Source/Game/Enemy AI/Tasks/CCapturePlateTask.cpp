//////////////////////////////////////////////////////
// File: "CCapturePlateTask.cpp"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CCapturePlateTask.h"
#include "../CEnemyArtificialIntelligence.h"
#include "../../Objects/Units/CUnitObject.h"
#include "../../Unit Commands/CAttackCommand.h"
#include "../../Unit Commands/CAttackMoveCommand.h"
#include "../../Objects/Appliances/CPlate.h"
#include "../../Cooking Mini-Game/CRecipeManager.h"

CCapturePlateTask::CCapturePlateTask()
{
	m_TargetPlate = NULL;
	m_EnemyDefenseThreatLevel = 0;
	m_UnitsAlreadyRequested = 0;
	m_FriendlyAttackThreatLevel = 0;
}

CCapturePlateTask::CCapturePlateTask(CPlate* const TargetPlate)
{
	SetTargetPlate(TargetPlate);
	m_EnemyDefenseThreatLevel = 0;
	m_UnitsAlreadyRequested = 0;
	m_FriendlyAttackThreatLevel = 0;
}

CCapturePlateTask::CCapturePlateTask(const CCapturePlateTask& Source) : IAITask(Source)
{
	SetTargetPlate(Source.m_TargetPlate);
	m_EnemyDefenseThreatLevel = Source.m_EnemyDefenseThreatLevel;
	m_UnitsAlreadyRequested = Source.m_UnitsAlreadyRequested;
	m_FriendlyAttackThreatLevel = Source.m_FriendlyAttackThreatLevel;
}

CCapturePlateTask& CCapturePlateTask::operator=(const CCapturePlateTask& Source)
{
	IAITask::operator=(Source);
	SetTargetPlate(Source.m_TargetPlate);
	m_EnemyDefenseThreatLevel = Source.m_EnemyDefenseThreatLevel;
	m_UnitsAlreadyRequested = Source.m_UnitsAlreadyRequested;
	m_FriendlyAttackThreatLevel = Source.m_FriendlyAttackThreatLevel;
	return *this;
}

CCapturePlateTask::~CCapturePlateTask()
{

}

bool CCapturePlateTask::Process()
{
	CEnemyArtificialIntelligence* EnemyAI = GetEnemyAI();
	IAITask::Process();

	m_EnemyDefenseThreatLevel = 0;
	m_TargetPlate->GetObjectManagerOwner()->IterateLayer<CCapturePlateTask>(1, &ScanForEnemyDefenses, this);
	m_EnemyDefenseThreatLevel = max(1, m_EnemyDefenseThreatLevel);

	m_FriendlyAttackThreatLevel = GetAssignedUnits().size();
	if(m_EnemyDefenseThreatLevel > m_FriendlyAttackThreatLevel)
	{
		unsigned char UnitsToRequest = 0;
		unsigned char UnitsAlreadyRequested = m_UnitsAlreadyRequested;
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
			m_UnitsAlreadyRequested += UnitsToRequest;
		}
	}

	std::list<CUnitObject*>& AssignedUnits = GetAssignedUnits();
	for(std::list<CUnitObject*>::iterator i = AssignedUnits.begin(); i != AssignedUnits.end(); ++i)
	{
		CAttackMoveCommand* AsAttackMoveCommand = dynamic_cast<CAttackMoveCommand*>((*i)->GetUnitAICommand());
		if(AsAttackMoveCommand && AsAttackMoveCommand->GetDestination() == m_TargetPlate->GetPosition()) // If the unit is already executing this task...
			continue; // Skip to the next unit.
		else // The unit is not executing this task.
			(*i)->SetUnitAICommand(new CAttackMoveCommand(m_TargetPlate->GetPosition(), m_TargetPlate->GetObjectManagerOwner())); // Command the unit to execute this task.
	}
	if(m_TargetPlate->GetTeamValue() == TEAM_ENEMY) // Plate has been captured.
		return true;	
	else // Plate remains.
		return false;
}

void CCapturePlateTask::AssignUnit(CUnitObject* UnitObject)
{
	// TEMPORARY: Doesn't calculate a heuristic, just increases threat level.
	IAITask::AssignUnit(UnitObject);
	m_UnitsAlreadyRequested = max(0, m_UnitsAlreadyRequested - 1);
}


void CCapturePlateTask::SetTargetPlate(CPlate* const TargetPlate)
{
	m_TargetPlate = TargetPlate;
}

bool CCapturePlateTask::ScanForEnemyDefenses(IObject* Object, CCapturePlateTask* Sender)
{
	CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(Object);
	if(AsUnitObject && AsUnitObject->GetTeamValue() == TEAM_PLAYER && D3DXVec2LengthSq(&(AsUnitObject->GetPosition() - Sender->m_TargetPlate->GetPosition())) <= 100000)
	{
		if(Sender)
		{
			++(Sender->m_EnemyDefenseThreatLevel);
		}
	}
	return false;
}