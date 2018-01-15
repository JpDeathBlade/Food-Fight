//////////////////////////////////////////////////////
// File: "CSkilledUnitObject.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: 9/17/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CSKILLEDUNITOBJECT_H_
#define CSKILLEDUNITOBJECT_H_

#include "CUnitObject.h"
#include "../Base/CGameObject.h"

#define SKILLEDUNIT_MAXLEVEL 10

class CSkilledUnitObject : public CUnitObject
{
private:
	unsigned short	m_usExperience;
	unsigned char	m_ucLevel;

public:
	//constructors and destructor
	CSkilledUnitObject(void);
	~CSkilledUnitObject(void) { }

	int Attack(IAttackable* _pTarget);

	///////////////////////////////////////////////////////////////
	// Function: Update
	//
	// In: A float that is the time that has elapsed since the las update
	//
	// Out: void
	//
	// Purpose: To Update the unit based on time.
	///////////////////////////////////////////////////////////////
	void Update(float Delta);

	///////////////////////////////////////////////////////////////
	// Function: Draw
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Renders the unit based on its position and animation.
	///////////////////////////////////////////////////////////////
	void Draw(void);

	///////////////////////////////////////////////////////////////
	// Function: OnHover
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Handles any special actions that the unit needs
	//			to do while it is being hovered over.
	///////////////////////////////////////////////////////////////
	virtual void OnHover(void);

	///////////////////////////////////////////////////////////////
	// Function: ContextSensitiveClick
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Handles any special actions that the unit needs
	//			to do while it is being hovered over.
	///////////////////////////////////////////////////////////////
	virtual void ContextSensitiveClick(void);

	unsigned char  GetLevel()		{ return m_ucLevel;		 }
	unsigned short GetExperience()	{ return m_usExperience; }

	int GetExperienceValue()		{ return ((m_ucLevel * 5) - 2); }
	int GetExpForNextLevel();
	
	void SetExperience(unsigned short _exp) { m_usExperience = _exp; }
	void AddExperience(int amount);
	void LevelUp();
};

	///////////////////////Factory Fuctions///////////////////////
	IObject* CreateToast(CObjectManager* pManager);
	IObject* CreateSunnySideUpEgg(CObjectManager* pManager);
	IObject* CreateScrambledEgg(CObjectManager* pManager);
	IObject* CreateSausage(CObjectManager* pManager);
	IObject* CreateHam(CObjectManager* pManager);
	IObject* CreatePancake(CObjectManager* pManager);
	IObject* CreateWaffle(CObjectManager* pManager);
	IObject* CreateBluberryMuffin(CObjectManager* pManager);
	IObject* CreateFrenchToast(CObjectManager* pManager);
	IObject* CreateGrapefruit(CObjectManager* pManager);
	IObject* CreateBanana(CObjectManager* pManager);
	IObject* CreateStrawberries(CObjectManager* pManager);
#endif