//////////////////////////////////////////////////////
// File: "CGUIObject.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/14/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef _CGUIObject_H_
#define _CGUIObject_H_
#include "..\Base\IObject.h"
#include "..\Base\IAnimable.h"
#include <d3dx9.h>

class CGUIObject : public IObject, public IAnimable
{
	D3DXVECTOR2 m_Position;
public:
	CGUIObject(); // Default Constructor
	CGUIObject(const CGUIObject& Source); // Copy Constructor
	virtual CGUIObject& operator=(const CGUIObject& Source); // Assignment Operator
	virtual ~CGUIObject(); // Destructor

	virtual void Update(float Delta);
	virtual void Draw();
	virtual RECT GetDrawRectangle() const;
	virtual RECT GetCollisionRectangle() const;

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
};

void DrawGUIFrame(IAnimable* Animable, D3DXVECTOR2 Position, D3DXVECTOR2 Size, D3DCOLOR Color = D3DCOLOR_XRGB(255, 255, 255));
#endif