//////////////////////////////////////////////////////
// File: "CRecipeManager.h"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#ifndef CRECIPEMANAGER_H_
#define CRECIPEMANAGER_H_

#include "CRecipe.h"
#include "../Objects/GUI Objects/CGUIObject.h"

class CRecipeManager : public CGUIObject
{
	static CRecipeManager* m_Instance;
	std::vector<CRecipe*> m_Recipes;
	unsigned char m_Page;

	CRecipeManager(); // Default Constructor
	CRecipeManager(const CRecipeManager& Source); // Copy Constructor - Disabled
	CRecipeManager& operator=(const CRecipeManager& Source); // Assignment Operator - Disabled
	~CRecipeManager(); // Destructor
public:
	static CRecipeManager* GetInstance(); // Returns the singleton's instance.
	static void DeleteInstance(); // Deletes the singleton's instance.

	void InitializeRecipes();
	void AddRecipe(CRecipe* newRecipe);
	std::vector<CRecipe*>& GetRecipes();
	CRecipe* TryRecipe(int _egg, int _meat, int _wheat, int _fruit, float _milk, int _cheese, float _butter);

	void Draw();

	void AcquireReference();
	void ReleaseReference();

	void FlipLeft();
	void FlipRight();
};
#endif