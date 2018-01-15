#include "IAITask.h"
#include "../../Objects/Units/CUnitObject.h"
#include "../CEnemyArtificialIntelligence.h"

IAITask::IAITask()
{
	SetPriority(0);
}

IAITask::IAITask(const IAITask& Source)
{
	SetPriority(Source.GetPriority());
}

IAITask& IAITask::operator=(const IAITask& Source)
{
	SetPriority(Source.GetPriority());
	return *this;
}

IAITask::~IAITask()
{
	for(std::list<CUnitObject*>::iterator i = m_AssignedUnits.begin(); i != m_AssignedUnits.end(); ++i)
	{
		if(*i)
		{
			(*i)->SetUnitAICommand(NULL);
			(*i)->ReleaseReference();
		}
	}
}

bool IAITask::Process()
{
	// Release references to units that are scheduled for deletion.
	for(std::list<CUnitObject*>::iterator i = m_AssignedUnits.begin(); i != m_AssignedUnits.end();)
	{
		if((*i)->CheckRemove())
		{
			(*i)->SetUnitAICommand(NULL);
			(*i)->ReleaseReference();
			i = m_AssignedUnits.erase(i);
		}
		else
		{
			++i;
		}
	}
	return false;
}


unsigned char IAITask::GetPriority() const
{
	return m_Priority;
}

void IAITask::SetPriority(unsigned char Priority)
{
	m_Priority = Priority;
}

void IAITask::AssignUnit(CUnitObject* UnitObject)
{
	m_AssignedUnits.push_back(UnitObject);
	if(UnitObject)
		UnitObject->AcquireReference();
}

bool IAITask::UnassignUnit(CUnitObject* UnitObject)
{
	for(std::list<CUnitObject*>::iterator i = m_AssignedUnits.begin(); i != m_AssignedUnits.end();)
	{
		if((*i) == UnitObject)
		{
			(*i)->ReleaseReference();
			i = m_AssignedUnits.erase(i);
			return true;
		}
		else
			++i;
	}
	return false;
}

std::list<CUnitObject*>& IAITask::GetAssignedUnits()
{
	return m_AssignedUnits;
}

CEnemyArtificialIntelligence* IAITask::GetEnemyAI() const
{
	return m_EnemyAI;
}

void IAITask::SetEnemyAI(CEnemyArtificialIntelligence* const EnemyAI)
{
	m_EnemyAI = EnemyAI;
}