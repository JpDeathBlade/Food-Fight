//////////////////////////////////////////////////////
// File: "CMoveCommand.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/21/09
// Last Modified By: SA
//////////////////////////////////////////////////////
#include "CMoveCommand.h"
#include "../Objects/Units/CUnitObject.h"
#include "../Objects/Units/CSkilledUnitObject.h"
#include "../Objects/Tiles/CTileStage.h"
#include "../Objects/Tiles/CTile.h"
#include "../../Utilities/CCodeProfiler.h"
#include "../../Managers/Bad Chef/Networking/NetworkUpdateClasses.h"
#include "../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include <list>
#include <vector>

bool LineToRectangleIntersection(D3DXVECTOR2 Start, D3DXVECTOR2 End, RECT Rectangle)
{
	float DeltaX = (End.x - Start.x);
	float m;
	if(DeltaX)
		m = ((End.y - Start.y) / DeltaX);
	else
		m = 0;
	float b = Start.y - ((End.y - Start.y) / DeltaX * Start.x);

	float YIntersect = m * Rectangle.left + b; 
	if(YIntersect > Rectangle.top && YIntersect < Rectangle.bottom) // Left Side
	{
		return true;
	}

	YIntersect = m * Rectangle.right + b; 
	if(YIntersect > Rectangle.top && YIntersect < Rectangle.bottom) // Right Side
	{
		return true;
	}

	if(m)
	{
		float XIntersect = (Rectangle.top - b) / m;
		if(XIntersect > Rectangle.left && XIntersect < Rectangle.right) // Top Side
		{
			return true;
		}

		XIntersect = (Rectangle.bottom - b) / m;
		if(XIntersect > Rectangle.left && XIntersect < Rectangle.right) // Bottom Side
		{
			return true;
		}
	}
	return false;
}

struct SCollideableObjectInPathData
{
	D3DXVECTOR2 Start;
	D3DXVECTOR2 End;
	CGameObject* GameObject;
};

bool CollideableObjectInPath(IObject* Object, SCollideableObjectInPathData* Data)
{
	if(Data->GameObject != Object)
	{
		CGameObject* AsGameObject = dynamic_cast<CGameObject*>(Object);
		if(AsGameObject && !dynamic_cast<CUnitObject*>(AsGameObject) && AsGameObject->IsCollideable())
			return LineToRectangleIntersection(Data->Start, Data->End, AsGameObject->GetCollisionRectangle());
		else
			return false;
	}
	return false;
}

bool CheckPathIsTraversable(D3DXVECTOR2 Start, D3DXVECTOR2 End, CGameObject* GameObject)
{
	CTileStage* TileStage = CTileStage::GetInstance();
	unsigned int ax = static_cast<unsigned int>(Start.x / TileStage->GetTileWidth());
	unsigned int ay = static_cast<unsigned int>(Start.y / TileStage->GetTileHeight());

	unsigned int bx = static_cast<unsigned int>(End.x / TileStage->GetTileWidth());
	unsigned int by = static_cast<unsigned int>(End.y / TileStage->GetTileHeight());

	RECT Bounds;
	if(ax > bx)
	{
		Bounds.left = bx;
		Bounds.right = ax;
	}
	else
	{
		Bounds.left = ax;
		Bounds.right = bx;
	}
	if(ay > by)
	{
		Bounds.top = by;
		Bounds.bottom = ay;
	}
	else
	{
		Bounds.top = ay;
		Bounds.bottom = by;
	}
	Bounds.left = Bounds.left < 0 ? 0 : Bounds.left;
	Bounds.right = Bounds.right > TileStage->GetMapWidth() - 1 ? TileStage->GetMapWidth() - 1 : Bounds.right;
	Bounds.top = Bounds.top < 0 ? 0 : Bounds.top;
	Bounds.bottom = Bounds.bottom > TileStage->GetMapHeight() - 1 ? TileStage->GetMapHeight() - 1 : Bounds.bottom;
	// Off stage check
	if(Bounds.left < 0 || Bounds.right >= TileStage->GetMapWidth() || Bounds.top < 0 || Bounds.bottom >= TileStage->GetMapHeight())
		return false;

	// Tile stage check
	for(int y = Bounds.top; y <= Bounds.bottom; ++y) // For each row in that rectangle area:
	{
		for(int x = Bounds.left; x <= Bounds.right; ++x) // For each column in that row:
		{
			CTile* Tile = &(TileStage->GetTiles()[x][y]); // Cell Algorithm to get the right index.
			if(Tile->GetCollisionID()) // If the tile is collideable
			{
				RECT TileRect;
				TileRect.left = x * TileStage->GetTileWidth();
				TileRect.right = TileRect.left + TileStage->GetTileWidth();
				TileRect.top = y * TileStage->GetTileHeight();
				TileRect.bottom = TileRect.top + TileStage->GetTileHeight();

				if(LineToRectangleIntersection(Start, End, TileRect))
					return false;
			}
		}
	}

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-010
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Game Object Check
	//SCollideableObjectInPathData CollideableObjectInPathData;
	//CollideableObjectInPathData.Start = Start;
	//CollideableObjectInPathData.End = End;
	//CollideableObjectInPathData.GameObject = GameObject;

	//if(GameObject->GetObjectManagerOwner()->Iterate<SCollideableObjectInPathData>(&CollideableObjectInPath, &CollideableObjectInPathData))
	//	return false;

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-010
	//////////////////////////////////////////////////////////////////////////

	return true;
}

CMoveCommand::CMoveCommand(bool procPaths)
{
	SetDestination(0, 0);
	m_ProcessPath = procPaths;
	m_bPartAttacking = false;
}

CMoveCommand::CMoveCommand(const CMoveCommand& Source)
{
	SetDestination(Source.GetDestination());
	m_ProcessPath = true;
}

CMoveCommand& CMoveCommand::operator=(const CMoveCommand& Source)
{
	SetDestination(Source.GetDestination());
	m_ProcessPath = true;
	return *this;
}

CMoveCommand::~CMoveCommand()
{
	//MAKE SURE PATH IS CLEARED
}

void CMoveCommand::OptimizePath(CUnitObject* Object)
{
	// Optimize beginning.
	if(m_Path.size() >= 2)
	{
		for(std::list<CPathNode*>::iterator CurrentNode = ++m_Path.begin(); CurrentNode != m_Path.end(); ++CurrentNode)
		{
			if(CheckPathIsTraversable(Object->GetPosition(), (*CurrentNode)->GetPosition(), Object))
				m_Path.erase(--std::list<CPathNode*>::iterator(CurrentNode));
			else
				break;
		}
		// TODO: Optimize ending.
		//for(std::list<CPathNode*>::reverse_iterator CurrentNode = ++m_Path.rbegin(); CurrentNode != m_Path.rend(); ++CurrentNode)
		//{
		//	if(CheckPathIsTraversable((*CurrentNode)->GetPosition(), GetDestination()))
		//		m_Path.erase(std::list<CPathNode*>::iterator(CurrentNode.base()));
		//	else
		//		break;
		//}
	}
}

bool CMoveCommand::ProcessPath(CUnitObject* Object)
{
	if(CheckPathIsTraversable(Object->GetPosition(), GetDestination(), Object)) // If it is impossible to go directly to the destination...
	{
		m_ProcessPath = false;
		return true; // No path is necessary, but it is possible to reach the destination.
	}
	else
	{
		// Calculate path of nodes to destination.
		CPathNode* BegginingNode = NULL;
		D3DXVECTOR2 ObjectToNode;

		CPathNode* EndingNode = NULL;
		D3DXVECTOR2 DestinationToNode;

		bool FirstRun = true;
		for(std::vector<CPathNode*>::iterator i = CPathNode::GetNodeList()->begin(); i != CPathNode::GetNodeList()->end(); ++i)
		{
			D3DXVECTOR2 ObjectToNewNode = (*i)->GetPosition() - Object->GetPosition();
			if(D3DXVec2LengthSq(&ObjectToNewNode) < D3DXVec2LengthSq(&ObjectToNode) || FirstRun) // Find nearest node to position. If there is a direct traversal to that node, use that node. Otherwise, use a different nearest node. Try until all nodes are exhausted.
			{
				if(CheckPathIsTraversable(Object->GetPosition(), (*i)->GetPosition(), Object))
				{
					BegginingNode = (*i);
					ObjectToNode = ObjectToNewNode;
				}
			}

			D3DXVECTOR2 DestinationToNewNode = (*i)->GetPosition() - GetDestination();
			if(D3DXVec2LengthSq(&DestinationToNewNode) < D3DXVec2LengthSq(&DestinationToNode) || FirstRun) // Find nearest node to destination. If there is a direct traversal to that destination, use that node. Otherwise, use a different nearest node.
			{
				if(CheckPathIsTraversable(GetDestination(), (*i)->GetPosition(), Object))
				{
					EndingNode = (*i);
					DestinationToNode = DestinationToNewNode;
				}
			}

			FirstRun = false;
		}
		if(BegginingNode && EndingNode) // If there is a traversable node for each point, beggining and end...
		{
			CPlannedNode::SetDestinationNode(EndingNode);

			std::list<CPlannedNode*> Open;
			std::list<CPlannedNode*> Closed;

			Open.push_back(new CPlannedNode(NULL, BegginingNode));

			bool PathFound = false;
			while(!Open.empty())
			{
				CPlannedNode* TopNode = (*Open.begin());
				std::list<CPlannedNode*>::iterator j = Open.begin();
				for(std::list<CPlannedNode*>::iterator i = Open.begin(); i != Open.end(); ++i)
				{
					if((*i) < TopNode)
					{
						TopNode = (*i);
						j = i;
					}
				}
				j = Open.erase(j);
				Closed.push_back(TopNode);
				if(TopNode->GetPathNode() == EndingNode)
				{
					CPlannedNode* Parent = TopNode;
					while(Parent)
					{
						m_Path.push_front(Parent->GetPathNode());
						Parent = Parent->GetParent();
					}
					PathFound = true;
					OptimizePath(Object);
					m_ProcessPath = false;
					break;
				}

				std::vector<CPathNode*>& Links = TopNode->GetPathNode()->GetLinks();
				unsigned char NumLinks = Links.size();
				for(unsigned char i = 0; i < NumLinks; ++i) // For each link.
				{
					bool AlreadyClosed = false;
					for(std::list<CPlannedNode*>::iterator j = Closed.begin(); j != Closed.end(); ++j)
					{
						if(Links[i] == (*j)->GetPathNode()) // If this node has already been visited.
						{
							bool AlreadyClosed = true;
							break;
						}
					}
					if(AlreadyClosed)
					{
						continue;
					}
					else
					{
						bool AlreadyOpen = false;
						for(std::list<CPlannedNode*>::iterator j = Open.begin(); j != Open.end(); ++j)
						{
							if(Links[i] == (*j)->GetPathNode()) // If this link is already a candidate.
							{
								AlreadyOpen = true;
								if(TopNode->GetTotalDistance() < (*j)->GetTotalDistance()) // If the current node's heuristic is less than the candidate's parent's heuristic.
									(*j)->SetParent(TopNode);
								break;
							}
						}
						if(!AlreadyOpen)
							Open.push_back(new CPlannedNode(TopNode, Links[i]));
					}
				}
			}
			for(std::list<CPlannedNode*>::iterator i = Open.begin(); i != Open.end(); ++i)
			{
				delete (*i);
			}
			Open.clear();
			for(std::list<CPlannedNode*>::iterator i = Closed.begin(); i != Closed.end(); ++i)
			{
				delete (*i);
			}
			Closed.clear();
			return PathFound;
		}
		else // One or two of the points can not be traversed to by node, beggining and/or end.
		{
			return false;
		}
		return false;
	}
	return false;
}

bool CMoveCommand::Execute(CUnitObject* Object)
{
	D3DXVECTOR2 Destination = GetDestination(); // Set destination. Go directly to destination.
	if(m_ProcessPath)
	{
		if(!ProcessPath(Object)) // No path found.
			return true; // Command complete.
		if(Object->GetNWOwner())
			{
			NW_MANAGER_START
				if(CNetworkManager::GetInstance()->IsConnected())
				{
					CNetworkManager::GetInstance()->SendMoveCommandPacket(this, Object->GetSharedID());
#if _DEBUG
					//printf("Move Command packet sent about unit #%d Type: %s", Object->GetSharedID(), Object->GetCreationString().c_str());
#endif
				}
			NW_MANAGER_END
			}
	}

	//Check if i magically got there.... (networking purposes)
	D3DXVECTOR2 ToDestination = (GetDestination() - Object->GetPosition());
	if(D3DXVec2LengthSq(&ToDestination) <= 100)
	{
		Object->SetVelocity(0, 0);
		Object->SetState(Object->STATE_IDLE);
		m_Path.clear();
		return true;
	}

	if(m_Path.size()) // If there is a predetermined path.
		Destination = m_Path.front()->GetPosition(); // Set destination to the next node.

	ToDestination = (Destination - Object->GetPosition());
	D3DXVECTOR2 Direction;
	D3DXVec2Normalize(&Direction, &ToDestination);
	Object->SetVelocity(Direction * Object->GetMovementSpeed());

	CSkilledUnitObject* pSkilled = dynamic_cast<CSkilledUnitObject*>(Object);
	if(pSkilled)
	{
		if(Direction.x > 0)
		{
			Object->SetDirection(Object->DIRECTION_RIGHT);
			Object->SetState(Object->STATE_MOVING);
		}
		else if(Direction.x < 0)
		{
			Object->SetDirection(Object->DIRECTION_LEFT);
			Object->SetState(Object->STATE_MOVING);
		}
	}
	else
	{
		if(Direction.x > 0)
		{
			Object->SetDirection(Object->DIRECTION_RIGHT);
			Object->SetState(Object->STATE_MOVING);
		}
		else if(Direction.x < 0)
		{
			Object->SetDirection(Object->DIRECTION_LEFT);
			Object->SetState(Object->STATE_MOVING);
		}
	}

	if(D3DXVec2LengthSq(&ToDestination) <= 100)
	{
		Object->SetVelocity(0, 0);
		Object->SetState(Object->STATE_IDLE);
		if(m_Path.size())
		{
			m_Path.pop_front();
			return false;
		}
		ToDestination = GetDestination() - Object->GetPosition();
		if(m_Path.empty() && D3DXVec2LengthSq(&ToDestination) <= 100)
			return true;
		return true;
	}
	return false;
}

D3DXVECTOR2 CMoveCommand::GetDestination() const
{
	return m_Destination;
}

float CMoveCommand::GetDestinationX() const
{
	return m_Destination.x;
}

float CMoveCommand::GetDestinationY() const
{
	return m_Destination.y;
}

void CMoveCommand::SetDestination(D3DXVECTOR2 Destination)
{
	m_Destination = Destination;
	m_ProcessPath = true;
}

void CMoveCommand::SetDestination(float X, float Y)
{
	SetDestinationX(X);
	SetDestinationY(Y);
	
}

void CMoveCommand::SetDestinationX(float X)
{
	m_Destination.x = X;
	m_ProcessPath = true;
}

void CMoveCommand::SetDestinationY(float Y)
{
	m_Destination.y = Y;
	m_ProcessPath = true;
}