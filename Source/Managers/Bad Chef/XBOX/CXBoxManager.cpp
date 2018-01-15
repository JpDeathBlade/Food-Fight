#include "CXBoxManager.h"

CXBoxManager CXBoxManager::m_Instance;

CXBoxManager::CXBoxManager(void)
{
	m_XBOX_Players.reserve(4);
}

CXBoxManager::~CXBoxManager()
{
	ShutdownCXBoxManager();
}

bool CXBoxManager::InitCXBoxManager(int _numPlayers)
{
	for(int i = 0; i < _numPlayers; i++)
	{
		CXBoxController* newPlayer = new CXBoxController(i);
		m_XBOX_Players.push_back(newPlayer);
	}

	return 0;
}

void CXBoxManager::ShutdownCXBoxManager()
{
	for(unsigned int i = 0; i < m_XBOX_Players.size(); i++)
	{
		delete m_XBOX_Players[i];
		m_XBOX_Players[i] = NULL;
	}

	m_XBOX_Players.clear();
}

void CXBoxManager::Update()
{
	for(unsigned int i = 0; i < m_XBOX_Players.size(); i++)
	{
		m_XBOX_Players[i]->Update();
	}
}

CXBoxController* CXBoxManager::GetController(int _player)
{
	return m_XBOX_Players[_player];
}