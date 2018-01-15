//////////////////////////////////////////////////////
// File: "CObjectManager.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _CObjectManager_H_
#define _CObjectManager_H_
#include "CObjectFactory.h"
#include <vector>
#include <list>
#include <d3dx9.h>

class IObject;

class CObjectLayer
{
public:
	typedef bool (*FunctionPointer)(IObject*, IObject*);

	enum ECoordinateSpace { CS_Screen, CS_World };
private:
	std::list<IObject*> m_Objects;
	std::list<IObject*> m_ObjectsToAdd;
	ECoordinateSpace	m_CoordinateSpace;
	bool m_Locked;

	void AddQueuedObjects();
public:
	CObjectLayer(); // Default Constructor
	CObjectLayer(const CObjectLayer& Source); // Copy Constructor
	CObjectLayer& operator=(const CObjectLayer& Source); // Assignment Operator
	~CObjectLayer(); // Destructor

	void AddObject(IObject* Object);
	void RemoveObject(IObject* Object);
	void RemoveAllObjects();

	void UpdateAllObjects(float Delta);
	void DrawAllObjects(D3DXMATRIX Transform, RECT rView);

	template<typename CallerType>
	bool Iterate(bool (*Function)(IObject*, CallerType*), CallerType* pCaller = NULL);
	template<typename CallerType>
	bool IterateReversed(bool (*Function)(IObject*, CallerType*), CallerType* pCaller = NULL);

	ECoordinateSpace GetCoordinateSpace();
	void SetCoordinateSpace(ECoordinateSpace CoordinateSpace);

	IObject* operator[](int Index);
	int Size() const;
};

template<typename CallerType>
bool CObjectLayer::Iterate(bool (*Function)(IObject*, CallerType*), CallerType* pCaller)
{
	m_Locked = true;
	for(std::list<IObject*>::iterator Iterator = m_Objects.begin(); Iterator != m_Objects.end(); ++Iterator)
		if(Function(*Iterator, pCaller))
		{
			m_Locked = false;
			return true;
		}
		m_Locked = false;
		return false;
}

template<typename CallerType>
bool CObjectLayer::IterateReversed(bool (*Function)(IObject*, CallerType*), CallerType* pCaller)
{
	m_Locked = true;
	for(std::list<IObject*>::reverse_iterator Iterator = m_Objects.rbegin(); Iterator != m_Objects.rend(); ++Iterator)
		if(Function(*Iterator, pCaller))
		{
			m_Locked = false;
			return true;
		}
		m_Locked = false;
		return false;
}

class CCamera;

class CObjectManager
{
	std::vector<CObjectLayer> m_ObjectLayers;
	std::list<IObject*> m_CollideableObjects;

	void ProcessCollisions();
public:
	CObjectManager(); // Default Constructor
	CObjectManager(unsigned char Layers); // Custom Constructor
	CObjectManager(const CObjectManager& Source); // Copy Constructor - Disabled
	CObjectManager& operator=(const CObjectManager& Source); // Assignment Operator - Disabled
	~CObjectManager(); // Destructor

	void Initialize(unsigned char NumLayers);

	void SetLayerCoordinateSpace(unsigned char LayerIndex, CObjectLayer::ECoordinateSpace CoordinateSpace);

	void AddObjectToLayer(unsigned char LayerIndex, IObject* Object);
	void RemoveObjectFromLayer(unsigned char LayerIndex, IObject* Object);
	void RemoveAllObjectsFromLayer(unsigned char LayerIndex);
	void RemoveAllObjects();

	void UpdateAllObjectsInLayer(unsigned char LayerIndex, float Delta);
	void DrawAllObjectsInLayer(unsigned char LayerIndex, D3DXMATRIX Transform, RECT rView);

	void UpdateAllObjects(float Delta);
	void DrawAllObjects(D3DXMATRIX Transform, RECT rView);

	template<typename CallerType>
	bool IterateLayer(unsigned char LayerIndex, bool (*Function)(IObject*, CallerType*), CallerType* pCaller = NULL);
	template<typename CallerType>
	bool Iterate(bool (*Function)(IObject*, CallerType*), CallerType* pCaller = NULL);

	template<typename CallerType>
	bool IterateLayerReversed(unsigned char LayerIndex, bool (*Function)(IObject*, CallerType*), CallerType* pCaller = NULL);
	template<typename CallerType>
	bool IterateReversed(bool (*Function)(IObject*, CallerType*), CallerType* pCaller = NULL);
};

template<typename CallerType>
bool CObjectManager::IterateLayer(unsigned char LayerIndex, bool (*Function)(IObject*, CallerType*), CallerType* pCaller)
{
	return m_ObjectLayers[LayerIndex].Iterate(Function, pCaller);
}

template<typename CallerType>
bool CObjectManager::Iterate(bool (*Function)(IObject*, CallerType*), CallerType* pCaller)
{
	for(unsigned char Index = 0; Index < m_ObjectLayers.size(); ++Index)
		if(IterateLayer(Index, Function, pCaller))
			return true;
	return false;
}

template<typename CallerType>
bool CObjectManager::IterateLayerReversed(unsigned char LayerIndex, bool (*Function)(IObject*, CallerType*), CallerType* pCaller)
{
	return m_ObjectLayers[LayerIndex].IterateReversed(Function, pCaller);
}

template<typename CallerType>
bool CObjectManager::IterateReversed(bool (*Function)(IObject*, CallerType*), CallerType* pCaller)
{
	for(unsigned char Index = 0; Index < m_ObjectLayers.size(); ++Index)
		if(IterateLayerReversed(m_ObjectLayers.size() - 1 - Index, Function, pCaller))
			return true;
	return false;
}

D3DXVECTOR2 MinimumTranslationDistance(RECT A, RECT B);
#endif