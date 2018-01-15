//////////////////////////////////////////////////////
// File: "CEnemyAI.cpp"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CEnemyArtificialIntelligence.h"
#include "Tasks/IAITask.h"
#include "Tasks/CAttackStoveTask.h"
#include "Tasks/CCapturePlateTask.h"
#include "../Objects/Units/CUnitObject.h"
#include "../Objects/Appliances/CStove.h"
#include "../Objects/Appliances/CPlate.h"
#include "../Cooking Mini-Game/CPan.h"
#include "../Cooking Mini-Game/CCookingUnit.h"

// CEnemyAI::CPanReachedDoneStageFunctor //

void CEnemyArtificialIntelligence::CPanReachedDoneStageFunctor::operator()()
{
	if(GetSender()->GetCookingUnit())
		GetEnemyAI()->OwnUnit(GetSender()->GetCookingUnit()->SpawnUnit()); // Spawn the unit.
}

CEnemyArtificialIntelligence* CEnemyArtificialIntelligence::CPanReachedDoneStageFunctor::GetEnemyAI() const
{
	return m_EnemyAI;
}

void CEnemyArtificialIntelligence::CPanReachedDoneStageFunctor::SetEnemyAI(CEnemyArtificialIntelligence* const EnemyAI)
{
	m_EnemyAI = EnemyAI;
}

// CEnemyAI //

bool CEnemyArtificialIntelligence::TaskComparator(IAITask* A, IAITask* B)
{
	if(A->GetPriority() > B->GetPriority())
		return true;
	else
		return false;
}

CEnemyArtificialIntelligence::CEnemyArtificialIntelligence()
{
	m_FriendlyStove = NULL;
	m_EnemyStove = NULL;
	m_DependingOnResources = false;
}

CEnemyArtificialIntelligence::~CEnemyArtificialIntelligence()
{
	while(!m_Tasks.empty())
	{
		delete m_Tasks.front();
		m_Tasks.pop_front();
	}
	for(std::list<CUnitObject*>::iterator i = m_UnassignedUnits.begin(); i != m_UnassignedUnits.end();)
	{
		if(*i)
		{
			(*i)->ReleaseReference();
			i = m_UnassignedUnits.erase(i);
		}
		else
			++i;
	}
	for(std::list<CUnitObject*>::iterator i = m_OwnedUnits.begin(); i != m_OwnedUnits.end();)
	{
		(*i)->ReleaseReference();
		i = m_OwnedUnits.erase(i);
	}
	if(m_FriendlyStove)
	{
		m_FriendlyStove->GetTopLeftPan()->ReachedDoneStage -= &OnPanStageDone;
		m_FriendlyStove->GetTopRightPan()->ReachedDoneStage -= &OnPanStageDone;
		m_FriendlyStove->GetBottomLeftPan()->ReachedDoneStage -= &OnPanStageDone;
		m_FriendlyStove->GetBottomRightPan()->ReachedDoneStage -= &OnPanStageDone;
		m_FriendlyStove->ReleaseReference();
	}
	if(m_EnemyStove)
		m_EnemyStove->ReleaseReference();
	for(std::list<CPlate*>::iterator i = m_Plates.begin(); i != m_Plates.end();)
	{
		(*i)->ReleaseReference();
		i = m_Plates.erase(i);
	}
}

void CEnemyArtificialIntelligence::ProcessCooking()
{
	UsePan(m_FriendlyStove->GetTopLeftPan());
	UsePan(m_FriendlyStove->GetTopRightPan());
	UsePan(m_FriendlyStove->GetBottomLeftPan());
	UsePan(m_FriendlyStove->GetBottomRightPan());
}

void CEnemyArtificialIntelligence::UsePan(CPan* Pan)
{
	if(!m_Recipes.empty())
	{
		if(Pan->GetPanStage() == PAN_NONE) // If the pan is free, pop the front Recipe and put in onto the pan.
		{
			CRecipe* Recipe = m_Recipes.front();
			bool HasEnoughResources = true;

			if(m_FriendlyStove->GetEggCount() < (int)Recipe->GetNeededEggs())
				HasEnoughResources = false;
			if(m_FriendlyStove->GetFruitCount() < (int)Recipe->GetNeededFruit())
				HasEnoughResources = false;
			if(m_FriendlyStove->GetMeatCount() < (int)Recipe->GetNeededMeat())
				HasEnoughResources = false;
			if(m_FriendlyStove->GetWheatCount() < (int)Recipe->GetNeededWheat())
				HasEnoughResources = false;

			if(HasEnoughResources)
			{
				m_DependingOnResources = false;
				for(unsigned char i = 0; i < Recipe->GetNeededEggs(); ++i)
				{
					CCookingResource* CookingResource = m_FriendlyStove->GetEgg();
					CookingResource->SetUsed(true);
					Pan->AddResource(CookingResource);
					m_FriendlyStove->SubtractCookingResource(CookingResource->GetResourceType());
				}
				for(unsigned char i = 0; i < Recipe->GetNeededFruit(); ++i)
				{
					CCookingResource* CookingResource = m_FriendlyStove->GetFruit();
					CookingResource->SetUsed(true);
					Pan->AddResource(CookingResource);
					m_FriendlyStove->SubtractCookingResource(CookingResource->GetResourceType());
				}
				for(unsigned char i = 0; i < Recipe->GetNeededMeat(); ++i)
				{
					CCookingResource* CookingResource = m_FriendlyStove->GetMeat();
					CookingResource->SetUsed(true);
					Pan->AddResource(CookingResource);
					m_FriendlyStove->SubtractCookingResource(CookingResource->GetResourceType());
				}
				for(unsigned char i = 0; i < Recipe->GetNeededWheat(); ++i)
				{
					CCookingResource* CookingResource = m_FriendlyStove->GetWheat();
					CookingResource->SetUsed(true);
					Pan->AddResource(CookingResource);
					m_FriendlyStove->SubtractCookingResource(CookingResource->GetResourceType());
				}

				//// TODO: Use the butter, cheese, and milk.
				//for(unsigned char i = 0; i < Recipe->GetNeededButter(); ++i)
				//	Pan->AddResource(/* TODO */);
				//for(unsigned char i = 0; i < Recipe->GetNeededCheese(); ++i)
				//	Pan->AddResource(/* TODO */);
				//for(unsigned char i = 0; i < Recipe->GetNeededMilk(); ++i)
				//	Pan->AddResource(/* TODO */);

				m_Recipes.pop_front();
			}
			else
			{
				if(!m_DependingOnResources)
				{
					// Capture a plate.
					CCapturePlateTask* CapturePlateTask = new CCapturePlateTask;
					CapturePlateTask->SetEnemyAI(this);
					CapturePlateTask->SetTargetPlate(FindPlate());
					CapturePlateTask->SetPriority(UCHAR_MAX);
					AssignTask(CapturePlateTask);
					m_DependingOnResources = true;
					return;
				}
			}
		}
	}
}

void CEnemyArtificialIntelligence::ProcessTasks()
{
	m_Tasks.sort(&TaskComparator);
	if(m_Tasks.front()->Process())
	{
		std::list<CUnitObject*>& AssignedUnits = m_Tasks.front()->GetAssignedUnits();
		while(!AssignedUnits.empty())
		{
			m_UnassignedUnits.push_back(AssignedUnits.front());
			AssignedUnits.pop_front();
		}
		delete m_Tasks.front();
		m_Tasks.pop_front();
	}
}

void CEnemyArtificialIntelligence::Initialize()
{
	OnPanStageDone.SetEnemyAI(this);

	for(int i = 0; i < 4; ++i)
	{
		m_FriendlyStove->AddCookingResource(RESOURCE_MEAT, m_FriendlyStove->GetObjectManagerOwner());
		m_FriendlyStove->AddCookingResource(RESOURCE_FRUIT, m_FriendlyStove->GetObjectManagerOwner());
	}

	// Attack the player's stove.
	CAttackStoveTask* AttackStoveTask = new CAttackStoveTask;
	AttackStoveTask->SetEnemyAI(this);
	AttackStoveTask->SetTargetStove(m_EnemyStove);
	AttackStoveTask->SetPriority(static_cast<unsigned char>(UCHAR_MAX * 0.5f));
	m_Tasks.push_back(AttackStoveTask);
}

void CEnemyArtificialIntelligence::Update(float Delta)
{
	// Release references to units that are scheduled for deletion.
	for(std::list<CUnitObject*>::iterator i = m_UnassignedUnits.begin(); i != m_UnassignedUnits.end();)
	{
		if((*i)->CheckIsDead())
		{
			(*i)->ReleaseReference();
			i = m_UnassignedUnits.erase(i);
		}
		else
		{
			++i;
		}
	}
	for(std::list<CUnitObject*>::iterator i = m_OwnedUnits.begin(); i != m_OwnedUnits.end();)
	{
		if((*i)->CheckIsDead())
		{
			(*i)->ReleaseReference();
			i = m_OwnedUnits.erase(i);
		}
		else
		{
			++i;
		}
	}
	if(m_Recipes.empty() && !m_DependingOnResources)
	{
		std::vector<CRecipe*>& Recipes = CRecipeManager::GetInstance()->GetRecipes();
		QueueRecipe(Recipes[rand() % Recipes.size()]);
	}
	ProcessCooking();
	ProcessTasks();
}

void CEnemyArtificialIntelligence::SetFriendlyStove(CStove* FriendlyStove)
{
	m_FriendlyStove = FriendlyStove;
	m_FriendlyStove->AcquireReference();
	m_FriendlyStove->GetTopLeftPan()->ReachedDoneStage += &OnPanStageDone;
	m_FriendlyStove->GetTopRightPan()->ReachedDoneStage += &OnPanStageDone;
	m_FriendlyStove->GetBottomLeftPan()->ReachedDoneStage += &OnPanStageDone;
	m_FriendlyStove->GetBottomRightPan()->ReachedDoneStage += &OnPanStageDone;
}

void CEnemyArtificialIntelligence::SetEnemyStove(CStove* EnemyStove)
{
	m_EnemyStove = EnemyStove;
	m_EnemyStove->AcquireReference();
}

void CEnemyArtificialIntelligence::AddPlate(CPlate* Plate)
{
	m_Plates.push_back(Plate);
	Plate->AcquireReference();
}

CPlate* CEnemyArtificialIntelligence::FindPlate() const
{
	for(std::list<CPlate*>::const_iterator Iterator = m_Plates.begin(); Iterator != m_Plates.end(); ++Iterator)
	{
		if((*Iterator)->GetTeamValue() != TEAM_ENEMY)
		{
			return *Iterator;
		}
	}
	return NULL;
}

void CEnemyArtificialIntelligence::OwnUnit(CUnitObject* UnitObject)
{
	m_OwnedUnits.push_back(UnitObject);
	UnitObject->AcquireReference();
	m_UnassignedUnits.push_back(UnitObject);
	UnitObject->AcquireReference();
}

void CEnemyArtificialIntelligence::AssignTask(IAITask* Task)
{
	m_Tasks.push_back(Task);
}

CUnitObject* CEnemyArtificialIntelligence::GetUnit()
{
	if(!m_UnassignedUnits.empty())
	{
		CUnitObject* UnitObject = m_UnassignedUnits.front();
		m_UnassignedUnits.pop_front();
		UnitObject->ReleaseReference();
		return UnitObject;
	}
	else if(!m_OwnedUnits.empty() && (++m_Tasks.begin()) != m_Tasks.end() && (*(++(m_Tasks.begin())))->GetAssignedUnits().size())
	{
		CUnitObject* UnitObject = (*(++(m_Tasks.begin())))->GetAssignedUnits().front();
		(*(++(m_Tasks.begin())))->UnassignUnit(UnitObject);
		return UnitObject;
	}
	else
		return NULL;
}

CStove* CEnemyArtificialIntelligence::GetFriendlyStove() const
{
	return m_FriendlyStove;
}

void CEnemyArtificialIntelligence::QueueRecipe(CRecipe* Recipe)
{
	m_Recipes.push_back(Recipe);
}