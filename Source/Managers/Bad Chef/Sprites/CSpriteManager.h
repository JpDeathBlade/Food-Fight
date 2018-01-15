#ifndef _CSpriteManager_H_
#define _CSpriteManager_H_
#include <map>
#include <string>

class CSprite;
#include "../../SGD/CSGD_Direct3D.h"

class CSpriteManager
{
	static CSpriteManager* m_Instance;
	std::map<std::string, CSprite*> m_Sprites;

	CSpriteManager(); // Default Constructor
	CSpriteManager(const CSpriteManager& Source); // Copy Constructor - Disabled
	CSpriteManager& operator=(const CSpriteManager& Source); // Assignment Operator - Disabled
	~CSpriteManager(); // Destructor
public:
	static CSpriteManager* GetInstance();
	static void DeleteInstance();

	bool LoadSpriteFromImage(const char* FilePath, D3DCOLOR _color = 0);
	bool LoadSpriteFromSprite(const char* FilePath);
	bool UnloadSprite(const char* FilePath);
	void UnloadAllSprites();

	CSprite* GetSprite(const char* FilePath);
};

#endif //end of CProfileMananger.h