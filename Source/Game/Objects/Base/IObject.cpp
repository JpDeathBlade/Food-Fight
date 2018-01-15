//////////////////////////////////////////////////////
// File: "IObject.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/17/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "IObject.h"

IObject::IObject()
{
	SetCollideable(true);
	m_bDoRemove = false;
	m_ManagerOwner = NULL;
}

IObject::IObject(const IObject& Source) : IReferenceable(Source)
{
	SetCollideable(Source.IsCollideable());
}

IObject& IObject::operator=(const IObject& Source)
{
	IReferenceable::operator=(Source);
	SetCollideable(Source.IsCollideable());
	return *this;
}

IObject::~IObject()
{

}

void IObject::Update(float Delta)
{

}

void IObject::Draw()
{

}

void IObject::DrawDebug()
{

}

RECT IObject::GetDrawRectangle() const
{
	RECT DrawRectangle;
	DrawRectangle.top = 0;
	DrawRectangle.left = 0;
	DrawRectangle.bottom = 0;
	DrawRectangle.right = 0;
	return DrawRectangle;
}

RECT IObject::GetCollisionRectangle() const
{
	RECT CollisionRectangle;
	CollisionRectangle.top = 0;
	CollisionRectangle.left = 0;
	CollisionRectangle.bottom = 0;
	CollisionRectangle.right = 0;
	return CollisionRectangle;
}

void IObject::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{

}

int ZSortComparison(const IObject* A, const IObject* B)
{
	unsigned char AZ = (unsigned char)A->GetZDepth();
	unsigned char BZ = (unsigned char)B->GetZDepth();
	if(AZ < BZ)
		return true;
	else
		return false;
}