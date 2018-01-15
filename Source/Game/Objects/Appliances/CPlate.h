//////////////////////////////////////////////////////
// File: "CPlate.h"
// Creator: AB
// Creation Date: 9/14/09
// Last Modified: 10/1/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CPLATE_H_
#define CPLATE_H_

#include "../Base/CGameObject.h"
#include "../GUI OBjects/CursorInterfaces.h"

class CObjectManager;
class CSprite;
class CSound;

class CPlate : public CGameObject, public ISelectable
{
private:
	int				m_nWheat;
	int				m_nEgg;
	int				m_nMeat;
	int				m_nFruit;
	unsigned int	m_unPlayerThreshold;
	unsigned int	m_unEnemyThreshold;
	unsigned int	m_unCapturePoints;
	float			m_fCaptureRange;
	float			m_fAccumulator;
	CSprite*		m_pCaptureBar;
	CSprite*		m_pOutline;
	ETeamValue		m_ePreviousTeam;
	CSound*			m_PlayerCapture;
	CSound*			m_EnemyCapture;
	CSound*			m_PlayerLost;
	CSound*			m_EnemyLost;

public:
	///////////////Constructor and Destructor///////////////
	CPlate(void);
	~CPlate(void);

	///////////////////////////////////////////////////////////////
	// Function: Update
	//
	// In: A float that is the time that has elapsed since the last update.
	//
	// Out: void
	//
	// Purpose: To Update the plate based on time.
	///////////////////////////////////////////////////////////////
	void Update(float Delta);

	///////////////////////////////////////////////////////////////
	// Function: Draw
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Draw the plate.
	///////////////////////////////////////////////////////////////
	void Draw(void);

	void OnCollision(IObject* Object, D3DXVECTOR2 Overlap);

	float GetZDepth() const;

	///////////////////////////////////////////////////////////////
	// Function: CheckCapturingUnits
	//
	// In: IObject pointer to the object that needs to be checked
	//		and an IObject pointer to the plate.
	//
	// Out: A bool for if it found something
	//
	// Purpose: To modify the Capture Point based on the units nearby.
	////////////////////////////////////////////////////////////////
	static bool CheckCapturingUnits(IObject* pObject, CPlate* pCaller);

	///////////////////////////////////////////////////////////////
	// Function: AddWheat and RemoveWheat
	//
	// In: Add/Remove some wheat resources to/from the plate.
	//
	// Out: void
	//
	// Purpose: Add/Remove wheat resources to/from the plate.
	////////////////////////////////////////////////////////////////
	void AddWheat(int nNumAdd = 1)			{ m_nWheat += nNumAdd; }
	void RemoveWheat(int nNumRemove = 1)	{ m_nWheat -= nNumRemove; }

	///////////////////////////////////////////////////////////////
	// Function: AddEgg and RemoveEgg
	//
	// In: Add/Remove some egg resources to/from the plate.
	//
	// Out: void
	//
	// Purpose: Add/Remove egg resources to/from the plate.
	///////////////////////////////////////////////////////////////
	void AddEgg(int nNumAdd = 1)		{ m_nEgg += nNumAdd; }
	void RemoveEgg(int nNumRemove = 1)	{ m_nEgg -= nNumRemove; }

	///////////////////////////////////////////////////////////////
	// Function: AddMeat and RemoveMeat
	//
	// In: Add/Remove some meat resources to/from the plate.
	//
	// Out: void
	//
	// Purpose: Add/Remove meat resources to/from the plate.
	///////////////////////////////////////////////////////////////
	void AddMeat(int nNumAdd = 1)		{ m_nMeat += nNumAdd; }
	void RemoveMeat(int nNumRemove = 1)	{ m_nMeat -= nNumRemove; }

	///////////////////////////////////////////////////////////////
	// Function: AddFruit and RemoveFruit
	//
	// In: Add/Remove some fruit resources to/from the plate.
	//
	// Out: void
	//
	// Purpose: Add/Remove fruit resources to/from the plate.
	///////////////////////////////////////////////////////////////
	void AddFruit(int nNumAdd = 1)			{ m_nFruit += nNumAdd; }
	void RemoveFruit(int nNumRemove = 1)	{ m_nFruit -= nNumRemove; }

	///////////////////////////////////////////////////////////////
	// Function: AddPoints and RemovePoints
	//
	// In: Add/Remove some points to/from the plate.
	//
	// Out: void
	//
	// Purpose: Add/Remove some points to/from the plate.
	///////////////////////////////////////////////////////////////
	void AddPoints(int nNumAdd = 1);
	void RemovePoints(int nNumRemove = 1);

	/////////////////Accessors and Mutators/////////////////
	int GetWheat(void) const							{ return m_nWheat; }
	void SetWheat(int nWheat)							{ m_nWheat = nWheat; }

	int GetEgg(void) const								{ return m_nEgg; }
	void SetEgg(int nEgg)								{ m_nEgg = nEgg; }

	int GetMeat(void) const								{ return m_nMeat; }
	void SetMeat(int nMeat)								{ m_nMeat = nMeat; }

	int GetFruit(void) const							{ return m_nFruit; }
	void SetFruit(int nFruit)							{ m_nFruit = nFruit; }

	unsigned int GetPlayerThreshold(void) const			{ return m_unPlayerThreshold; }
	void SetPlayerThreshold(unsigned int unThreshold)	{ m_unPlayerThreshold = unThreshold; }

	unsigned int GetEnemyThreshold(void) const			{ return m_unEnemyThreshold; }
	void SetEnemyThreshold(unsigned int unThreshold)		{ m_unEnemyThreshold = unThreshold; }

	unsigned int GetCapturePoints(void) const			{ return m_unCapturePoints; }
	void SetCapturePoints(unsigned int unCapturePoints)	{ m_unCapturePoints = unCapturePoints; }

	float GetCaptureRange(void) const					{ return m_fCaptureRange; }
	void SetCaptureRange(float fCaptureRange)			{ m_fCaptureRange = fCaptureRange; }
};

IObject* CreatePlateFactoryMethod(CObjectManager* objManager);

#endif