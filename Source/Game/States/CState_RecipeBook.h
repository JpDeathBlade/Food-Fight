#ifndef _CState_RecipeBook_H_
#define _CState_RecipeBook_H_

#include "IGameState.h"
class CRecipeManager;

class CState_RecipeBook : public IGameState
{
	CRecipeManager* m_RecipeManager;
public:
	CState_RecipeBook(); // Default Constructor
	CState_RecipeBook(const CState_RecipeBook& Source); // Copy Constructor - Disabled
	CState_RecipeBook& operator=(const CState_RecipeBook& Source); // Assignment Operator - Disabled
	~CState_RecipeBook(); // Destructor

	void OnEnter();

	bool Input();
	void Update(float Delta);
	void Draw();
};
#endif