#ifndef JS_CXBoxManager_H_
#define JS_CXBoxManager_H_

#include <vector>
using std::vector;

#include "CXBoxController.h"

// Camera MACRO to return the Camera Instance
#define XBOX	CXBoxManager::GetInstance()

class CXBoxManager
{
private:
	vector<CXBoxController*> m_XBOX_Players;
	static CXBoxManager m_Instance;		// Instance of the CTextManager

private:
	CXBoxManager(void);
	CXBoxManager(CXBoxManager &ref);
	CXBoxManager &operator= (CXBoxManager &ref);
public:
	~CXBoxManager();

	static CXBoxManager* GetInstance(void) { return &m_Instance; }

	bool InitCXBoxManager(int _numPlayers = 1);
	void ShutdownCXBoxManager();

	void Update();

	CXBoxController* GetController(int _player);
};

#endif