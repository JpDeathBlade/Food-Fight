//////////////////////////////////////////////////////
// File: "IObject.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/17/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef _IObject_H_
#define _IObject_H_
#include "IReferenceable.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include <d3dx9.h>

class IObject : public IReferenceable
{
	bool m_bIsCollideable;
	bool m_bDoRemove;
	CObjectManager* m_ManagerOwner;
public:
	IObject(); // Default Constructor
	IObject(const IObject& Source); // Copy Constructor
	virtual IObject& operator=(const IObject& Source); // Assignment Operator
	virtual ~IObject() = 0; // Destructor

	virtual void Update(float Delta); // Updates object-specific logic.
	virtual void Draw(); // Draws the object.
	virtual void DrawDebug(); // Draws debug-specific information.

	virtual RECT GetDrawRectangle() const; // Returns a rectangle defining the visible area of the object.
	virtual RECT GetCollisionRectangle() const; // Returns a rectangle defining the collideable area of the object.
	virtual void OnCollision(IObject* Object, D3DXVECTOR2 Overlap); // Processes object-specific collision logic based on the colliding object and the overlapping area.

	virtual float GetZDepth() const = 0;
	virtual bool IsCollideable() const { return m_bIsCollideable; }
	virtual void SetCollideable(const bool Collideable) { m_bIsCollideable = Collideable; }

	void DoRemove() { m_bDoRemove = true; }
	bool CheckRemove() { return m_bDoRemove; }

	CObjectManager* GetObjectManagerOwner() { return m_ManagerOwner; }
	void SetObjectManagerOwner(CObjectManager* _om) { m_ManagerOwner = _om; }
};

int ZSortComparison(const IObject* A, const IObject* B);
#endif