//////////////////////////////////////////////////////
// File: "CObjectFactory.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CObjectFactory.h"
#include "CObjectManager.h"
CObjectFactory* CObjectFactory::m_Instance = NULL;

CObjectFactory::CObjectFactory()
{

}

CObjectFactory::~CObjectFactory()
{

}

CObjectFactory* CObjectFactory::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CObjectFactory;
	return m_Instance;
}

void CObjectFactory::DeleteInstance()
{
	delete m_Instance;
	m_Instance = NULL;
}

bool CObjectFactory::AddMethod(std::string Identifier, FunctionPointer Method)
{
	for(std::map<std::string, FunctionPointer>::iterator Iterator = m_FactoryMethods.begin(); Iterator != m_FactoryMethods.end(); Iterator++)
	{
		if(Iterator->first == Identifier)
		{
			return false;
		}
	}
	m_FactoryMethods[Identifier] = Method;
	return true;
}

bool CObjectFactory::RemoveMethod(std::string Identifier)
{
	for(std::map<std::string, FunctionPointer>::iterator Iterator = m_FactoryMethods.begin(); Iterator != m_FactoryMethods.end(); Iterator++)
	{
		if(Iterator->first == Identifier)
		{
			Iterator = m_FactoryMethods.erase(Iterator);
			return true;
		}
	}
	return false;
}

void CObjectFactory::ClearMethods()
{
	m_FactoryMethods.clear();
}

IObject* CObjectFactory::Construct(std::string Identifier, CObjectManager* ObjectManager)
{
	for(std::map<std::string, FunctionPointer>::iterator Iterator = m_FactoryMethods.begin(); Iterator != m_FactoryMethods.end(); Iterator++)
	{
		if(Iterator->first == Identifier)
		{
			return (Iterator->second)(ObjectManager);
		}
	}
	return NULL;
}