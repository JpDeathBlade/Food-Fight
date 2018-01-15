#include "CMessageSystem.h"
#include "IMessage.h"
CMessageSystem* CMessageSystem::m_Instance = NULL;

CMessageSystem::CMessageSystem()
{

}

CMessageSystem::~CMessageSystem()
{
	ClearMessages();
}

CMessageSystem* CMessageSystem::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CMessageSystem();
	return m_Instance;
}

void CMessageSystem::DeleteInstance()
{
	delete m_Instance;
	m_Instance = NULL;
}

void CMessageSystem::AddMessage(IMessage* Message)
{
	m_MessageQueue.push_back(Message);
}

void CMessageSystem::ProcessMessages()
{
	for(unsigned int i = 0; i < m_MessageQueue.size(); i++)
	{
		m_MessageQueue[i]->Process();
	}

	ClearMessages();
}

void CMessageSystem::ClearMessages()
{
	m_MessageQueue.clear();
}