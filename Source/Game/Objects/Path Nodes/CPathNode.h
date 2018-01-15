//////////////////////////////////////////////////////
// File: "CPathNode.h"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 9/20/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CPATHNODE_H_
#define CPATHNODE_H_

#include <vector>
#include <d3dx9.h>
using std::vector;

enum ENodeType {PATH_NORMAL, PATH_STOVE, PATH_COFFEE, PATH_TOASTER, PATH_TOASTEROVEN, PATH_PLATE};

class CPathNode;

class CPlannedNode
{
	static CPathNode* m_DestinationNode;
	CPlannedNode* m_Parent;
	CPathNode* m_PathNode;
public:
	CPlannedNode();
	CPlannedNode(CPlannedNode* Parent, CPathNode* PathNode);
	CPlannedNode(const CPlannedNode& Source);
	CPlannedNode& operator=(const CPlannedNode& Source);
	~CPlannedNode();

	void SetParent(CPlannedNode* const Parent);
	CPlannedNode* GetParent() const;

	CPathNode* GetPathNode() const;
	
	static void SetDestinationNode(CPathNode* DestinationNode);

	unsigned int GetTotalDistance() const;
	unsigned int GetManhattanDistance(CPathNode* Destination) const;
	unsigned int GetManhattanDistance() const;

	bool operator<(CPlannedNode& PlannedNode);
	bool operator>(CPlannedNode& PlannedNode);
	bool operator<=(CPlannedNode& PlannedNode);
	bool operator>=(CPlannedNode& PlannedNode);
	bool operator==(CPlannedNode& PlannedNode);
	bool operator!=(CPlannedNode& PlannedNode);
};

class CPathNode
{
private:
	static vector<CPathNode*>	m_arrNodes;
	static CPathNode***			m_PathChart;
	vector<CPathNode*>			m_arrLinks;
	vector<int>					m_arrLinksToSetUp;	
	int							m_nNodeID;
	D3DXVECTOR2					m_Position;
	ENodeType					m_eType;
public:
	///////////////////Constructor and destructor//////////////////
	CPathNode(void);
	~CPathNode(void);

	////////////////////////Static Methods/////////////////////////

	///////////////////////////////////////////////////////////////
	// Function: AddNode
	//
	// In: A pointer to the new node you want to add to the static 
	//		vector of nodes.
	//
	// Out: void
	//
	// Purpose: Adds a new node to the static list of nodes that
	//			everybody accesses.
	///////////////////////////////////////////////////////////////
	static void AddNode(CPathNode* pNewNode);

	///////////////////////////////////////////////////////////////
	// Function: RemoveNode
	//
	// In: A pointer to the node you want to remove from the static 
	//		vector of nodes.
	//
	// Out: void
	//
	// Purpose: Removes a node from the static list of nodes that
	//			everybody accesses.
	///////////////////////////////////////////////////////////////
	static void RemoveNode(CPathNode* pNodeToRemove);

	///////////////////////////////////////////////////////////////
	// Function: FindNodeFromID
	//
	// In: A pointer to the node you want to remove from the static 
	//		vector of nodes.
	//
	// Out: void
	//
	// Purpose: Removes a node from the static list of nodes that
	//			everybody accesses.
	///////////////////////////////////////////////////////////////
	static CPathNode* FindNodeFromID(int _find);

	///////////////////////////////////////////////////////////////
	// Function: ComplilePaths
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Takes the id links and turns them into 
	//			link pointers.
	///////////////////////////////////////////////////////////////
	static void CompilePaths(void);

	///////////////////////////////////////////////////////////////
	// Function: ClearPathing
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Clears the complete node list.
	///////////////////////////////////////////////////////////////
	static void ClearPathing(void);

	/////////////Accessor for the static list of nodes/////////////
	static vector<CPathNode*>* GetNodeList(void)	{ return &m_arrNodes; }

	/////////////////////////Class Methods/////////////////////////
	
	///////////////////////////////////////////////////////////////
	// Function: AddLink
	//
	// In: An int that is the id of the node to point at.
	//
	// Out: void
	//
	// Purpose: Adds a pointer link to the list of link pointers.
	///////////////////////////////////////////////////////////////
	void AddLink(int nNodeID);

	///////////////////////////////////////////////////////////////
	// Function: RemoveLink
	//
	// In: An int that is the id of the node to point at.
	//
	// Out: void
	//
	// Purpose: Removes a pointer link from the list of link pointers.
	///////////////////////////////////////////////////////////////
	void RemoveLink(int nNodeID);

	///////////////////////////////////////////////////////////////
	// Function: AddLinkToSetup
	//
	// In: An int to add to the list of link ids.
	//
	// Out: void
	//
	// Purpose: Adds an int to the list of link ids.
	///////////////////////////////////////////////////////////////
	void AddLinkToSetup(int nNodeID);

	////////////////////Accessors and Mutators////////////////////
	int GetNodeID(void) const	{ return m_nNodeID; }
	void SetNodeID(int nID)		{ m_nNodeID = nID; }

	D3DXVECTOR2 GetPosition() const {return m_Position;} // Returns the object's position.
	float GetPositionX() const {return m_Position.x;} // Returns the object's position's x-coordinate.
	float GetPositionY() const {return m_Position.y;} // Returns the object's position's y-coordinate.

	void SetPosition(D3DXVECTOR2 Position) // Sets the object's position.
	{
		m_Position.x = Position.x;
		m_Position.y = Position.y;
	}
	void SetPosition(float X, float Y) // Sets the object's position.
	{
		m_Position.x = X;
		m_Position.y = Y;
	}
	void SetPositionX(float X) {m_Position.x = X;} // Sets the object's position's x-coordinate.
	void SetPositionY(float Y) {m_Position.y = Y;} // Sets the object's position's y-coordinate.

	int GetType(void) const		{ return (int)m_eType; }
	void SetType(int nType)		{ m_eType = (ENodeType)nType; }

	vector<CPathNode*>& GetLinks() {return m_arrLinks;}

	static CPathNode* FindNextNode(CPathNode* Start, CPathNode* End);
};
#endif