//////////////////////////////////////////////////////
// File: "CMiniMap.h"
// Creator: AB
// Creation Date: 9/18/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef CMiniMap_H_
#define CMiniMap_H_
#include "CGUIObject.h"
#include "CursorInterfaces.h"

class CSprite;
class CObjectManager;

class CMiniMap : public CGUIObject, public IClickable
{
	static CMiniMap* m_Instance;
	CObjectManager* m_ObjectManager;
	float m_Scale;
	D3DXVECTOR2	m_Offset;
	CSprite* m_MiniMapSprite;

	CMiniMap(); // Default Constructor
	CMiniMap(const CMiniMap& Source); // Copy Constructor - Disabled
	CMiniMap& operator=(const CMiniMap& Source); // Assignment Operator - Disabled
	~CMiniMap(); // Destructor

	void DrawIcons();
	static bool ObjectIterator(IObject* Object, CMiniMap* Caller);
public:
	static CMiniMap* GetInstance();
	static void DeleteInstance();

	void LoadMinimap(const char* szFilename);

	void OnClick();

	void Draw();
	RECT GetDrawRectangle() const;
	RECT GetCollisionRectangle() const;

	void SetObjectManager(CObjectManager* const ObjectManager);

	void SetScale(float Scale);

	void SetOffset(const D3DXVECTOR2 Offset);
	void SetOffset(float X, float Y);
	void SetOffsetX(float X);
	void SetOffsetY(float Y);

	static IObject* CreateMiniMap(CObjectManager* ObjectManager);
};

#endif