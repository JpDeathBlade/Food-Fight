#include "CRecipeBook.h"
CRecipeBook* CRecipeBook::m_Instance = NULL;

CRecipeBook::CRecipeBook()
{

}

CRecipeBook::~CRecipeBook()
{
	m_Instance = NULL;
}

CRecipeBook* CRecipeBook::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CRecipeBook;
	return m_Instance;
}

void CRecipeBook::DeleteInstance()
{
	delete m_Instance;
}

IObject* CRecipeBook::CreateRecipeBook(CObjectManager* ObjectManager)
{
	CRecipeBook* RecipeBook = CRecipeBook::GetInstance();

	ObjectManager->AddObjectToLayer(4, RecipeBook);
	return RecipeBook;
}
