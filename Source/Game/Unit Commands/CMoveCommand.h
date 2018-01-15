#ifndef _CMoveCommand_H_
#define _CMoveCommand_H_
#include "IUnitAICommand.h"
#include "../Objects/Path Nodes/CPathNode.h"
#include <d3dx9.h>
#include <list>

class CUnitObject;

class CMoveCommand : public IUnitAICommand
{
	std::list<CPathNode*> m_Path;
	D3DXVECTOR2 m_Destination;
	bool m_ProcessPath;
	bool m_bPartAttacking;

	void OptimizePath(CUnitObject* Object);
	bool ProcessPath(CUnitObject* Object);
public:
	CMoveCommand(bool procPaths = true);
	CMoveCommand(const CMoveCommand& Source);
	virtual CMoveCommand& operator=(const CMoveCommand& Source);
	virtual ~CMoveCommand();

	bool Execute(CUnitObject* Object);

	D3DXVECTOR2 GetDestination() const;
	float GetDestinationX() const;
	float GetDestinationY() const;
	std::list<CPathNode*>* GetPath() { return &m_Path; }

	void SetPartAttacking(bool _part) { m_bPartAttacking = _part; }
	bool GetPartAttacking() { return m_bPartAttacking; }

	void SetDestination(D3DXVECTOR2 Destination);
	void SetDestination(float X, float Y);
	void SetDestinationX(float X);
	void SetDestinationY(float Y);
	void SetProcessPath(bool _proc) { m_ProcessPath = _proc; }
};

class CGameObject;

bool CheckPathIsTraversable(D3DXVECTOR2 Start, D3DXVECTOR2 End, CGameObject* GameObject);
#endif