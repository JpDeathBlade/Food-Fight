#ifndef _CRecipeBook_H_
#define _CRecipeBook_H_
#include "CGUIObject.h"

class CRecipe;
class IObject;
class CObjectManager;

class CRecipeBook : public CGUIObject
{
	static CRecipeBook* m_Instance;

	CRecipeBook(); // Default Constructor
	CRecipeBook(const CRecipeBook& Source); // Copy Constructor - Disabled
	CRecipeBook& operator=(const CRecipeBook& Source); // Assignment Operator - Disabled
	~CRecipeBook(); // Destructor
public:
	static CRecipeBook* GetInstance();
	static void DeleteInstance();

	static IObject* CreateRecipeBook(CObjectManager* ObjectManager);
};
#endif