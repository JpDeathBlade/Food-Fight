//////////////////////////////////////////////////////
// File: "CEnemyAI.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _CEnemyAI_H_
#define _CEnemyAI_H_
#include <list>
#include "../../Managers/Bad Chef/Events/CEvent.h"

class IAITask;
class CBuildUnitTask;
class CUnitObject;
class CStove;
class CPlate;
class CPan;
class CRecipe;

class CEnemyArtificialIntelligence
{
	class CPanReachedDoneStageFunctor : public CEventFunctor<CPan>
	{
		CEnemyArtificialIntelligence* m_EnemyAI;
	public:
		void operator()();

		CEnemyArtificialIntelligence* GetEnemyAI() const;
		void SetEnemyAI(CEnemyArtificialIntelligence* const EnemyAI);
	};

	CPanReachedDoneStageFunctor OnPanStageDone;

	static bool TaskComparator(IAITask* A, IAITask* B);

	std::list<IAITask*> m_Tasks;
	std::list<CRecipe*> m_Recipes;
	std::list<CUnitObject*> m_OwnedUnits;
	std::list<CUnitObject*> m_UnassignedUnits;
	CStove* m_FriendlyStove;
	CStove* m_EnemyStove;
	std::list<CPlate*> m_Plates;
	bool m_DependingOnResources;

	void ProcessCooking();
	void UsePan(CPan* Pan);
	void ProcessTasks();
public:
	CEnemyArtificialIntelligence(); // Default Constructor
	CEnemyArtificialIntelligence(const CEnemyArtificialIntelligence& Source); // Copy Constructor - Disabled
	CEnemyArtificialIntelligence& operator=(const CEnemyArtificialIntelligence& Source); // Assignment Operator - Disabled
	~CEnemyArtificialIntelligence(); // Destructor

	void Initialize();

	void Update(float Delta);

	void SetFriendlyStove(CStove* FriendlyStove);
	void SetEnemyStove(CStove* EnemyStove);

	void AddPlate(CPlate* Plate);
	CPlate* FindPlate() const;

	void OwnUnit(CUnitObject* UnitObject);
	void AssignTask(IAITask* Task);

	CUnitObject* GetUnit();
	
	CStove* GetFriendlyStove() const;

	void QueueRecipe(CRecipe* Recipe);
};
#endif