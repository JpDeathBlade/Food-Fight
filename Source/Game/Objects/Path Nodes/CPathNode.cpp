//////////////////////////////////////////////////////
// File: "CPathNode.cpp"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 9/20/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CPathNode.h"
#include <list>

// CPlannedNode //

CPathNode* CPlannedNode::m_DestinationNode = NULL;

CPlannedNode::CPlannedNode()
{
	m_Parent = NULL;
	m_PathNode = NULL;
}

CPlannedNode::CPlannedNode(CPlannedNode* Parent, CPathNode* PathNode)
{
	m_Parent = Parent;
	m_PathNode = PathNode;

}

CPlannedNode::CPlannedNode(const CPlannedNode& Source)
{
	m_Parent = Source.m_Parent;
	m_PathNode = Source.m_PathNode;
}

CPlannedNode& CPlannedNode::operator=(const CPlannedNode& Source)
{
	m_Parent = Source.m_Parent;
	m_PathNode = Source.m_PathNode;
	return *this;
}

CPlannedNode::~CPlannedNode()
{

}

void CPlannedNode::SetParent(CPlannedNode* const Parent)
{
	m_Parent = Parent;
}

CPlannedNode* CPlannedNode::GetParent() const
{
	return m_Parent;
}

CPathNode* CPlannedNode::GetPathNode() const
{
	return m_PathNode;
}

void CPlannedNode::SetDestinationNode(CPathNode* DestinationNode)
{
	m_DestinationNode = DestinationNode;
}

unsigned int CPlannedNode::GetTotalDistance() const
{
	if(m_Parent)
		return m_Parent->GetTotalDistance() + GetManhattanDistance(m_Parent->GetPathNode());
	else
		return 0;
}

unsigned int CPlannedNode::GetManhattanDistance(CPathNode* Destination) const
{
	return (unsigned int)D3DXVec2LengthSq(&(Destination->GetPosition() - m_PathNode->GetPosition()));
}

unsigned int CPlannedNode::GetManhattanDistance() const
{
	if(m_DestinationNode)
		return (unsigned int)D3DXVec2LengthSq(&(m_DestinationNode->GetPosition() - m_PathNode->GetPosition()));
	else
		return 0;
}

bool CPlannedNode::operator<(CPlannedNode& PlannedNode)
{
	return GetTotalDistance() + GetManhattanDistance() < PlannedNode.GetTotalDistance() + GetManhattanDistance();
}

bool CPlannedNode::operator>(CPlannedNode& PlannedNode)
{
	return GetTotalDistance() + GetManhattanDistance() > PlannedNode.GetTotalDistance() + GetManhattanDistance();
}

bool CPlannedNode::operator<=(CPlannedNode& PlannedNode)
{
	return GetTotalDistance() + GetManhattanDistance() <= PlannedNode.GetTotalDistance() + GetManhattanDistance();
}

bool CPlannedNode::operator>=(CPlannedNode& PlannedNode)
{
	return GetTotalDistance() + GetManhattanDistance() >= PlannedNode.GetTotalDistance() + GetManhattanDistance();
}

bool CPlannedNode::operator==(CPlannedNode& PlannedNode)
{
	return GetTotalDistance() + GetManhattanDistance() == PlannedNode.GetTotalDistance() + GetManhattanDistance();
}

bool CPlannedNode::operator!=(CPlannedNode& PlannedNode)
{
	return GetTotalDistance() + GetManhattanDistance() != PlannedNode.GetTotalDistance() + GetManhattanDistance();
}

// CPathNode //

vector<CPathNode*> CPathNode::m_arrNodes;
CPathNode*** CPathNode::m_PathChart;

CPathNode::CPathNode(void)
{
	m_eType		= PATH_NORMAL;
	m_nNodeID	= -1;
	m_Position = D3DXVECTOR2(0, 0);
}

CPathNode::~CPathNode(void)
{
	m_arrLinksToSetUp.clear();
	m_arrLinks.clear();
}

void CPathNode::AddNode(CPathNode* pNewNode)
{
	m_arrNodes.push_back(pNewNode);
}

void CPathNode::RemoveNode(CPathNode* pNodeToRemove)
{
	// TODO: Remove links and delete actual node?
	for(std::vector<CPathNode*>::iterator i = m_arrNodes.begin(); i != m_arrNodes.end();)
	{
		if((*i) == pNodeToRemove)
		{
			i = m_arrNodes.erase(i);
			break;
		}
		else
			++i;
	}
}

CPathNode* CPathNode::FindNodeFromID(int _find)
{
	for(unsigned int i = 0 ; i < m_arrNodes.size(); i++)
	{
		if(m_arrNodes[i]->GetNodeID() == _find)
			return m_arrNodes[i];
	}
	return NULL;
}

void CPathNode::CompilePaths(void)
{
	for(unsigned int i = 0; i < m_arrNodes.size(); ++i)
	{
		for(unsigned int j = 0; j < m_arrNodes[i]->m_arrLinksToSetUp.size(); ++j)
			m_arrNodes[i]->AddLink(m_arrNodes[i]->m_arrLinksToSetUp[j]);

		m_arrNodes[i]->m_arrLinksToSetUp.clear();
	}

	//m_PathChart = new CPathNode**[m_arrNodes.size()];
	//for(unsigned int i = 0; i < m_arrNodes.size(); ++i)
	//{
	//	m_PathChart[i] = new CPathNode*[m_arrNodes.size()];
	//}

	//for(unsigned int y = 0; y < m_arrNodes.size(); ++y)
	//{
	//	for(unsigned int x = 0; x < m_arrNodes.size(); ++x)
	//	{
	//		CPlannerNode::SetDestination(m_arrNodes[x]);

	//		if(x == y)
	//		{
	//			m_PathChart[y][x] = m_arrNodes[x];
	//			m_PathChart[x][y] = m_arrNodes[y];
	//			continue;
	//		}

	//		std::list<CPlannedNode*> Open;
	//		std::list<CPlannedNode*> Closed;
	//		
	//		Open.push_back(new CPlannedNode(NULL, m_arrNodes[y]);

	//		bool PathFound = false;
	//		while(!Open.empty())
	//		{
	//			CPlannedNode* TopNode = (*Open.begin());
	//			std::list<CPlannedNode*>::iterator i;
	//			for(i = Open.begin(); i != Open.end(); ++i)
	//			{
	//				if((*i) < TopNode)
	//					TopNode = (*i);
	//			}
	//			Open.erase(i);
	//			Closed.push_back(TopNode);
	//			if(TopNode->GetPathNode() == m_arrNodes[x])
	//			{
	//				m_PathChart[y][x] = m_arrNodes[x];
	//				m_PathChart[x][y] = m_arrNodes[y];
	//				PathFound = true;
	//				break;
	//			}

	//			std::vector<CPathNode*>& Links = TopNode.GetPathNode()->GetLinks();
	//			unsigned char NumLinks = Links.size();
	//			for(unsigned char i = 0; i < NumLinks; ++i) // For each link.
	//			{
	//				bool AlreadyClosed = false;
	//				for(std::list<CPlannedNode*>::iterator j = Closed.begin(); j != Closed.end(); ++j)
	//				{
	//					if(Links[i] == (*j)->GetPathNode()) // If this node has already been visited.
	//					{
	//						bool AlreadyClosed = true;
	//						break;
	//					}
	//				}
	//				if(AlreadyClosed)
	//				{
	//					continue;
	//				}
	//				else
	//				{
	//					bool AlreadyOpen = false;
	//					for(std::list<CPlannedNode*>::iterator j = Open.begin(); j != Open.end(); ++j)
	//					{
	//						if(Links[i] == (*j)->GetPathNode()) // If this link is already a candidate.
	//						{
	//							AlreadyOpen = true;
	//							if(TopNode->GetTotalDistance() < (*j)->GetTotalDistance()) // If the the the current node's heuristic is less than the candidate's parent's heuristic.
	//								(*j)->SetParent(TopNode);
	//							break;
	//						}
	//					}
	//					if(!AlreadyOpen)
	//						Open.push_back(new CPlannedNode(TopNode, Links[i]));
	//				}
	//			}
	//		}
	//		if(!PathFound)
	//		{
	//			m_PathChart[y][x] = NULL;
	//			m_PathChart[x][y] = NULL;
	//		}
	//	}
	//}
}

void CPathNode::ClearPathing(void)
{
	for(unsigned int i = 0; i < m_arrNodes.size(); ++i)
		delete m_arrNodes[i];

	m_arrNodes.clear();
}

void CPathNode::AddLink(int nNodeID)
{
	for(unsigned int i = 0; i < m_arrNodes.size(); ++i)
	{
		if(m_arrNodes[i]->GetNodeID() == nNodeID)
			m_arrLinks.push_back(m_arrNodes[i]);
	}
}

void CPathNode::RemoveLink(int nNodeID)
{
	vector<CPathNode*>::iterator Iter;

	Iter = m_arrLinks.begin();

	for(; Iter != m_arrLinks.end(); ++Iter)
	{
		if((*Iter)->GetNodeID() == nNodeID)
			Iter = m_arrLinks.erase(Iter);
	}
}

void CPathNode::AddLinkToSetup(int nNodeID)
{
	m_arrLinksToSetUp.push_back(nNodeID);
}

CPathNode* CPathNode::FindNextNode(CPathNode* Start, CPathNode* End)
{
	return m_PathChart[Start->GetNodeID()][End->GetNodeID()];
}