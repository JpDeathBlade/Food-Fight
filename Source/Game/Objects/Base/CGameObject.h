//////////////////////////////////////////////////////
// File: "CGameObject.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/17/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef _CGameObject_H_
#define _CGameObject_H_
#include "IObject.h"
#include "IAnimable.h"
#include <d3dx9.h>

class CGameObject : public IObject, public IAnimable
{
	D3DXVECTOR2 m_Position;
	float m_Rotation;
	D3DXVECTOR2 m_Scale;
	D3DXVECTOR2 m_Velocity;
public:
	CGameObject(); // Default Constructor
	CGameObject(const CGameObject& Source); // Copy Constructor
	virtual CGameObject& operator=(const CGameObject& Source); // Assignment Operator
	virtual ~CGameObject(); // Destructor

	virtual void Update(float Delta);
	virtual void Draw();
	virtual RECT GetDrawRectangle() const;
	virtual RECT GetCollisionRectangle() const;
	virtual void OnCollision(IObject* Object, D3DXVECTOR2 Overlap);

	virtual float GetZDepth() const;

	virtual D3DXVECTOR2 GetPosition() const; // Returns the object's position.
	virtual float GetPositionX() const; // Returns the object's position's x-coordinate.
	virtual float GetPositionY() const; // Returns the object's position's y-coordinate.

	virtual void SetPosition(D3DXVECTOR2 Position); // Sets the object's position.
	virtual void SetPosition(float X, float Y); // Sets the object's position.
	virtual void SetPositionX(float X); // Sets the object's position's x-coordinate.
	virtual void SetPositionY(float Y); // Sets the object's position's y-coordinate.

	virtual void Translate(D3DXVECTOR2 Distance); // Translates the object.
	virtual void Translate(float X, float Y); // Translates the object.
	virtual void TranslateX(float X); // Translates the object in the x-axis.
	virtual void TranslateY(float Y); // Translates the object in the y-axis.

	virtual D3DXVECTOR2 GetVelocity() const; // Returns the object's velocity.
	virtual float GetVelocityX() const; // Returns the object's velocity's x-coordinate.
	virtual float GetVelocityY() const; // Returns the object's velocity's y-coordinate.

	virtual void SetVelocity(D3DXVECTOR2 Velocity); // Sets the object's velocity.
	virtual void SetVelocity(float X, float Y); // Sets the object's velocity.
	virtual void SetVelocityX(float X); // Sets the object's velocity's x-coordinate.
	virtual void SetVelocityY(float Y); // Sets the object's velocity's y-coordinate.

	virtual void Accelerate(D3DXVECTOR2 Velocity); // Accelerates the object.
	virtual void Accelerate(float X, float Y); // Accelerates the object.
	virtual void AccelerateX(float X); // Accelerates the object in the x-axis.
	virtual void AccelerateY(float Y); // Accelerates the object in the y-axis.

	float GetRotation() const; // Returns the rotation of the object on the Z axis.

	void SetRotation(float Angle); // Sets the rotation of the object on the Z axis.

	D3DXVECTOR2 GetScale() const; // Returns the scale of the object.
	float GetScaleX() const; // Returns the scale of the object in the X axis.
	float GetScaleY() const; // Returns the scale of the object in the Y axis.

	void SetScale(D3DXVECTOR2 Scale); // Sets the scale of the object.
	void SetScale(float X, float Y); // Sets the scale of the object.
	void SetScaleX(float X); // Sets the scale of the object in the X axis.
	void SetScaleY(float Y); // Sets the scale of the object in the Y axis.
};
#endif