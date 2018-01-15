//////////////////////////////////////////////////////
// File: "CObjectManager.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CObjectManager.h"
#include "..\..\Game\Objects\Base\IObject.h"
#include "..\SGD\CSGD_Direct3D.h"
#include <cstdlib>
#include "../../Game/Objects/Tiles/CTileStage.h"
#include "../../Game/Objects/Tiles/CTile.h"
#include "../../Game/Objects/Base/CGameObject.h"
#include "../../Game/Objects/GUI Objects/CGUIObject.h"

// CObjectLayer //

CObjectLayer::CObjectLayer()
{
	m_CoordinateSpace = CS_World;
	m_Locked = false;
}

CObjectLayer::CObjectLayer(const CObjectLayer& Source)
{
	m_CoordinateSpace = Source.m_CoordinateSpace;
	m_Locked = false;
}

CObjectLayer& CObjectLayer::operator=(const CObjectLayer& Source)
{
	m_CoordinateSpace = Source.m_CoordinateSpace;
	m_Locked = false;
	return *this;
}

CObjectLayer::~CObjectLayer()
{
	RemoveAllObjects();
	m_ObjectsToAdd.clear();
}

void CObjectLayer::AddObject(IObject* Object)
{
	if(m_Locked)
		m_ObjectsToAdd.push_back(Object);
	else
		m_Objects.push_back(Object);
	Object->AcquireReference();
}

void CObjectLayer::AddQueuedObjects()
{
	for(std::list<IObject*>::iterator i = m_ObjectsToAdd.begin(); i != m_ObjectsToAdd.end();)
	{
		m_Objects.push_back(*i);
		i = m_ObjectsToAdd.erase(i);
	}
}

void CObjectLayer::RemoveObject(IObject* Object)
{
	m_Objects.remove(Object);
	if(Object)
		Object->ReleaseReference();
}

void CObjectLayer::RemoveAllObjects()
{
	m_Locked = true;
	for(std::list<IObject*>::iterator Iterator = m_Objects.begin(); Iterator != m_Objects.end();)
	{
		if(*Iterator)
			(*Iterator)->ReleaseReference();
		Iterator = m_Objects.erase(Iterator);
	}
	m_Locked = false;
}

void CObjectLayer::UpdateAllObjects(float Delta)
{
	m_Locked = true;
	for(std::list<IObject*>::iterator Iterator = m_Objects.begin(); Iterator != m_Objects.end();)
	{
		if(*Iterator)
		{
			(*Iterator)->Update(Delta);
			if((*Iterator)->CheckRemove())
			{
				(*Iterator)->ReleaseReference();
				Iterator = m_Objects.erase(Iterator);
				continue;
			}
		}
		++Iterator;
	}
	m_Locked = false;
	AddQueuedObjects();
	m_Objects.sort(&ZSortComparison);
}

void CObjectLayer::DrawAllObjects(D3DXMATRIX Transform, RECT rView)
{
	if(GetCoordinateSpace() == CS_Screen)
		D3DXMatrixIdentity(&Transform);
	CSGD_Direct3D::GetInstance()->GetSprite()->SetTransform(&Transform);
	m_Locked = true;
	for(std::list<IObject*>::iterator Iterator = m_Objects.begin(); Iterator != m_Objects.end(); ++Iterator)
	{
		RECT rTrash;

		if(IntersectRect(&rTrash, &rView, &(*Iterator)->GetDrawRectangle()) || GetCoordinateSpace() == CS_Screen)
			(*Iterator)->Draw();
	}
	m_Locked = false;
}

CObjectLayer::ECoordinateSpace CObjectLayer::GetCoordinateSpace()
{
	return m_CoordinateSpace;
}

void CObjectLayer::SetCoordinateSpace(ECoordinateSpace CoordinateSpace)
{
	m_CoordinateSpace = CoordinateSpace;
}

IObject* CObjectLayer::operator[](int Index)
{
	int i = 0;
	m_Locked = true;
	for(std::list<IObject*>::iterator Iterator = m_Objects.begin(); Iterator != m_Objects.end(); ++Iterator)
	{
		if(i == Index)
		{
			m_Locked = false;
			return (*Iterator);
		}
		++i;
	}
	m_Locked = false;
	return NULL;
}

int CObjectLayer::Size() const
{
	return m_Objects.size();
}

// CObjectManager //

CObjectManager::CObjectManager()
{

}

CObjectManager::~CObjectManager()
{
	RemoveAllObjects();
	m_ObjectLayers.clear();
}

void CObjectManager::Initialize(unsigned char NumLayers)
{
	m_ObjectLayers.resize(NumLayers);
}

void CObjectManager::SetLayerCoordinateSpace(unsigned char LayerIndex, CObjectLayer::ECoordinateSpace CoordinateSpace)
{
	m_ObjectLayers[LayerIndex].SetCoordinateSpace(CoordinateSpace);
}

void CObjectManager::AddObjectToLayer(unsigned char LayerIndex, IObject* Object)
{
	Object->SetObjectManagerOwner(this);
	m_ObjectLayers[LayerIndex].AddObject(Object);
}

void CObjectManager::RemoveObjectFromLayer(unsigned char LayerIndex, IObject* Object)
{
	m_ObjectLayers[LayerIndex].RemoveObject(Object);
}

void CObjectManager::RemoveAllObjectsFromLayer(unsigned char LayerIndex)
{
	m_ObjectLayers[LayerIndex].RemoveAllObjects();
}

void CObjectManager::RemoveAllObjects()
{
	for(unsigned char Index = 0; Index < m_ObjectLayers.size(); ++Index)
		RemoveAllObjectsFromLayer(Index);
}

void CObjectManager::UpdateAllObjectsInLayer(unsigned char LayerIndex, float Delta)
{
	m_ObjectLayers[LayerIndex].UpdateAllObjects(Delta);
}

void CObjectManager::DrawAllObjectsInLayer(unsigned char LayerIndex, D3DXMATRIX Transform, RECT rView)
{
	m_ObjectLayers[LayerIndex].DrawAllObjects(Transform, rView);
}

void CObjectManager::UpdateAllObjects(float Delta)
{
	for(unsigned char Index = 0; Index < m_ObjectLayers.size(); ++Index)
		UpdateAllObjectsInLayer(Index, Delta);
	ProcessCollisions();
}

void CObjectManager::DrawAllObjects(D3DXMATRIX Transform, RECT rView)
{
	for(unsigned char Index = 0; Index < m_ObjectLayers.size(); ++Index)
		DrawAllObjectsInLayer(Index, Transform, rView);
}

void CObjectManager::ProcessCollisions()
{
	for(unsigned char i = 0; i < m_ObjectLayers.size(); ++i) // Layer for source object to check against others.
	{
		for(unsigned char j = 0; j < m_ObjectLayers[i].Size(); ++j) // Object to check against others.
		{
			if(m_ObjectLayers[i][j]->IsCollideable())
			{
				RECT SourceRectangle = m_ObjectLayers[i][j]->GetCollisionRectangle();
				CGameObject* AsGameObject = dynamic_cast<CGameObject*>(m_ObjectLayers[i][j]);
				if(AsGameObject)
				{
					CTileStage* TileStage = CTileStage::GetInstance();
					int ax = SourceRectangle.left / TileStage->GetTileWidth();
					int ay = SourceRectangle.top / TileStage->GetTileHeight();

					int bx = SourceRectangle.right / TileStage->GetTileWidth();
					int by = SourceRectangle.bottom / TileStage->GetTileHeight();

					ax = ax < 0 ? 0 : ax;
					bx = bx > TileStage->GetMapWidth() - 1 ? static_cast<unsigned int>(TileStage->GetMapWidth() - 1) : bx;
					ay = ay < 0 ? 0 : ay;
					by = by > TileStage->GetMapHeight() - 1 ? static_cast<unsigned int>(TileStage->GetMapHeight() - 1) : by;
					if(ax > 0 && bx < TileStage->GetMapWidth() && ay > 0 && by < TileStage->GetMapHeight())
					{
						for(int y = ay; y <= by; ++y) // For each row in that rectangle area:
						{
							for(int x = ax; x <= bx; ++x) // For each column in that row:
							{
								CTile* Tile = &(TileStage->GetTiles()[x][y]); // Cell Algorithm to get the right index.
								if(Tile->GetCollisionID()) // If the tile is collideable.
								{
									RECT TileRectangle;
									TileRectangle.left = x * TileStage->GetTileWidth();
									TileRectangle.right = TileRectangle.left + TileStage->GetTileWidth();
									TileRectangle.top = y * TileStage->GetTileHeight();
									TileRectangle.bottom = TileRectangle.top + TileStage->GetTileHeight();

									D3DXVECTOR2 Overlap = MinimumTranslationDistance(SourceRectangle, TileRectangle);
									if(Overlap.x || Overlap.y)
										m_ObjectLayers[i][j]->OnCollision(NULL, Overlap);
								}
							}
						}
					}
				}
				for(unsigned char k = i; k < m_ObjectLayers.size(); ++k) // Layer for destination object to be checked against.
				{
					for(unsigned char l = i == k ? j : 0; l < m_ObjectLayers[k].Size(); ++l) // Object to be checked against.
					{
						if(m_ObjectLayers[k][l]->IsCollideable())
						{
							RECT DestinationRectangle = m_ObjectLayers[k][l]->GetCollisionRectangle();
							D3DXVECTOR2 Overlap = MinimumTranslationDistance(SourceRectangle, DestinationRectangle);
							if(Overlap.x || Overlap.y)
							{
								m_ObjectLayers[i][j]->OnCollision(m_ObjectLayers[k][l], Overlap);
								m_ObjectLayers[k][l]->OnCollision(m_ObjectLayers[i][j], -Overlap);
							}
						}
					}
				}
			}
		}
	}
}


D3DXVECTOR2 MinimumTranslationDistance(RECT A, RECT B)
{
	// Our displacement result vector containing the translation (movement) information
	// that resolves our intersection.
	D3DXVECTOR2 Result(0, 0);

	// This is re-used to calculate the difference in distance between sides.
	float Difference = 0.0f;

	// This stores the absolute minimum distance we'll need to separate our colliding object.
	float MinimumTranslationDistance = 0.0f;

	enum eAxis { AXIS_X = 0, AXIS_Y = 1 };
	enum eSide { SIDE_LEFT = -1, SIDE_RIGHT = 1 };

	// Axis stores the value of X or Y.  X = 0, Y = 1.
	// Side stores the value of left (-1) or right (+1).
	// They're used later in calculating the result vector.
	int nAxis = AXIS_X, nSide = 0;

	// Left
	Difference = (float)(A.right - B.left);
	if (Difference < 0.0f)
		return D3DXVECTOR2(0, 0);

	{ 
		// These braces are superfluous but should make it more 
		//clear that they're similiar to the if statements below.
		MinimumTranslationDistance = Difference;
		nAxis = AXIS_X;
		nSide = SIDE_LEFT;
	}

	// Right
	Difference = (float)(B.right - A.left); //right.Max.X - left.Min.X;
	if(Difference < 0.0f)
		return D3DXVECTOR2(0, 0);

	if(Difference < MinimumTranslationDistance)
	{
		MinimumTranslationDistance = Difference;
		nAxis = AXIS_X;
		nSide = SIDE_RIGHT;
	}

	// Down
	Difference = (float)(A.bottom - B.top); //left.Max.Y - right.Min.Y;
	if(Difference < 0.0f)
		return D3DXVECTOR2(0, 0);

	if(Difference < MinimumTranslationDistance)
	{
		MinimumTranslationDistance = Difference;
		nAxis = AXIS_Y;
		nSide = SIDE_LEFT;
	}

	// Up
	Difference = (float)(B.bottom - A.top); //right.Max.Y - left.Min.Y;
	if(Difference < 0.0f)
		return D3DXVECTOR2(0, 0);

	if (Difference < MinimumTranslationDistance)
	{
		MinimumTranslationDistance = Difference;
		nAxis = AXIS_Y;
		nSide = SIDE_RIGHT;
	}

	// Intersection occurred:
	if (nAxis == AXIS_Y) // Y Axis
		Result.y = (float)nSide * MinimumTranslationDistance;
	else // X Axis
		Result.x = (float)nSide * MinimumTranslationDistance;

	return Result;
}
