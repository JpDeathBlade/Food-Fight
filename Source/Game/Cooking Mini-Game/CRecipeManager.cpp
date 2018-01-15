//////////////////////////////////////////////////////
// File: "CRecipeManager.cpp"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#include "CRecipeManager.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"

CRecipeManager* CRecipeManager::m_Instance = NULL;

#define RECIPE_GRACE 0.2

CRecipeManager::CRecipeManager()
{
	m_Page = 0;
}

CRecipeManager::~CRecipeManager()
{
	for(unsigned int i = 0; i < m_Instance->m_Recipes.size(); i++)
	{
		for(unsigned int j = 0; j < m_Instance->m_Recipes[i]->GetCookActions()->size(); j++)
		{
			delete m_Instance->m_Recipes[i]->GetCookActions()->at(j);
		}
		m_Instance->m_Recipes[i]->GetCookActions()->clear();
		delete m_Instance->m_Recipes[i];
	}
	m_Instance->m_Recipes.clear();
	m_Instance = NULL;
}

CRecipeManager* CRecipeManager::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CRecipeManager;
	return m_Instance;
}

void CRecipeManager::DeleteInstance()
{
	delete m_Instance;
}

void CRecipeManager::InitializeRecipes()
{
	AddRecipe(new CRecipe("Toast", 15.0f, 0, 1, 0, 0, 0.0f, 0, 0.1f));

	CRecipe* ssueRecipe = new CRecipe("Sunny Side Up Egg", 30.0f, 1, 0, 0, 0, 0.0f, 0, 0.1f);
	ssueRecipe->GetCookActions()->push_back(new CCookAction("Flip", 10));
	//AddRecipe(new CRecipe("SunnySideUpEgg", 30, 1, 0, 0, 0, 0, 0, 0.1));
	AddRecipe(ssueRecipe);

	CRecipe* suRecipe = new CRecipe("Scrambled Eggs", 30.0f, 2, 0, 0, 0, 0.25f, 0, 0.1f);
	suRecipe->GetCookActions()->push_back(new CCookAction("Scramble", 10));
	AddRecipe(suRecipe);

	AddRecipe(new CRecipe("Sausage", 30.0f, 0, 0, 0, 1, 0.0f, 0, 0.0f));
	AddRecipe(new CRecipe("Ham", 30.0f, 0, 0, 0, 2, 0.0f, 0, 0.0f));

	CRecipe* pancakeRecipe = new CRecipe("Pancake", 30.0f, 1, 2, 0, 0, 1, 0, 0.2f);
	pancakeRecipe->GetCookActions()->push_back(new CCookAction("Flip", 10));
	AddRecipe(pancakeRecipe);

	AddRecipe(new CRecipe("Waffle", 20.0f, 2, 2, 0, 0, 1, 0, 0.2f));
	AddRecipe(new CRecipe("Blueberry Muffin", 30.0f, 1, 2, 2, 0, 0.5f, 0, 0.2f));

	CRecipe* ftRecipe = new CRecipe("French Toast", 30, 1, 1, 0, 0, 0.2f, 0, 0.2f);
	ftRecipe->GetCookActions()->push_back(new CCookAction("Flip", 10));
	AddRecipe(ftRecipe);

	AddRecipe(new CRecipe("Grapefruit", 3, 0, 0, 2, 0, 0, 0, 0));
	AddRecipe(new CRecipe("Banana", 3, 0, 0, 3, 0, 0, 0, 0));

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Removed Strawberry Recipe

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END 
	//////////////////////////////////////////////////////////////////////////

	m_Recipes.resize(m_Recipes.size());
}

void CRecipeManager::AddRecipe(CRecipe* newRecipe)
{
	m_Recipes.push_back(newRecipe);
}

std::vector<CRecipe*>& CRecipeManager::GetRecipes()
{
	return m_Recipes;
}

CRecipe* CRecipeManager::TryRecipe(int _egg, int _meat, int _wheat, int _fruit, float _milk, int _cheese, float _butter)
{
	for(unsigned int i = 0; i < m_Recipes.size(); i++)
	{
		if(m_Recipes[i]->GetNeededEggs() == _egg &&
			m_Recipes[i]->GetNeededMeat() == _meat &&
			m_Recipes[i]->GetNeededWheat() == _wheat &&
			m_Recipes[i]->GetNeededFruit() == _fruit/* &&
													m_Recipes[i]->GetNeededMilk()+RECIPE_GRACE >= _milk && m_Recipes[i]->GetNeededMilk()-RECIPE_GRACE <= _milk &&
													m_Recipes[i]->GetNeededCheese() == _cheese &&
													m_Recipes[i]->GetNeededButter()+RECIPE_GRACE >= _butter && m_Recipes[i]->GetNeededButter()-RECIPE_GRACE <= _butter*/)
		{
			return m_Recipes[i];
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BUG-015
// BUG FIX START
/////////////////////////////////////////////////////////////////
void CRecipeManager::Draw()
{
	CGUIObject::Draw();
	std::string TextToWrite;
	unsigned char Index = 2 * m_Page;
	float PageTop = GetPositionY() - 216;
	char CharacterBuffer[64];
	D3DXVECTOR2 FitSize;
	D3DXVECTOR2 ReturnSize;
	FitSize.y = 223;

	// Left Page
	float LeftPageStart = GetPositionX() - 334;
	FitSize.x = 329;
	float RecipeTop = PageTop;
	float NextLineTop = RecipeTop;
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", CStringTable::GetInstance()->FindString(m_Recipes[Index]->GetString()).c_str(), LeftPageStart, NextLineTop, 0.75, 0.75, 0, D3DCOLOR_XRGB(0, 0, 0));
	NextLineTop += 164;

	GetImageFromCreationString(m_Recipes[Index]->GetString())->Draw(0, 0, D3DXVECTOR2(LeftPageStart + 130.0f, NextLineTop-32));

	char CookTimeAsChar[8];
	sprintf_s(CookTimeAsChar, 8, "%.1f", m_Recipes[Index]->GetCookTime());
	TextToWrite = CStringTable::GetInstance()->FindString("Cook for ") + CookTimeAsChar + CStringTable::GetInstance()->FindString(" seconds.");
	ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), LeftPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
	NextLineTop += ReturnSize.y+8;
	FitSize.y -= ReturnSize.y;
	int NeededResource = m_Recipes[Index]->GetMaxSpices();
	if(NeededResource)
	{
		TextToWrite = CStringTable::GetInstance()->FindString("May take ");
		_itoa_s(NeededResource, CharacterBuffer, 64, 10);
		TextToWrite += CharacterBuffer;
		TextToWrite += CStringTable::GetInstance()->FindString(" spices.");
		ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), LeftPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
		NextLineTop += ReturnSize.y+8;
		FitSize.y -= ReturnSize.y;
	}

	NeededResource = m_Recipes[Index]->GetNeededFruit();
	if(NeededResource)
	{
		TextToWrite = CStringTable::GetInstance()->FindString("Fruit") + ": ";
		_itoa_s(NeededResource, CharacterBuffer, 64, 10);
		TextToWrite += CharacterBuffer;
		ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), LeftPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
		NextLineTop += ReturnSize.y+8;
		FitSize.y -= ReturnSize.y;
	}

	NeededResource = m_Recipes[Index]->GetNeededMeat();
	if(NeededResource)
	{
		TextToWrite = CStringTable::GetInstance()->FindString("Meat") + ": ";
		_itoa_s(NeededResource, CharacterBuffer, 64, 10);
		TextToWrite += CharacterBuffer;
		ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), LeftPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
		NextLineTop += ReturnSize.y+8;
		FitSize.y -= ReturnSize.y;
	}

	NeededResource = m_Recipes[Index]->GetNeededEggs();
	if(NeededResource)
	{
		TextToWrite = CStringTable::GetInstance()->FindString("Eggs") + ": ";
		_itoa_s(NeededResource, CharacterBuffer, 64, 10);
		TextToWrite += CharacterBuffer;
		ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), LeftPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
		NextLineTop += ReturnSize.y+8;
		FitSize.y -= ReturnSize.y;
	}

	NeededResource = m_Recipes[Index]->GetNeededWheat();
	if(NeededResource)
	{
		TextToWrite = CStringTable::GetInstance()->FindString("Wheat") + ": ";
		_itoa_s(NeededResource, CharacterBuffer, 64, 10);
		TextToWrite += CharacterBuffer;
		ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), LeftPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
		NextLineTop += ReturnSize.y+8;
		FitSize.y -= ReturnSize.y;
	}

	/////////////////////////////////////////////////////////////////	
	// BUG FIX	
	// Reference Bug # BB-013	
	// BUG FIX START	
	/////////////////////////////////////////////////////////////////

	//NeededResource = m_Recipes[Index]->GetNeededButter();
	//if(NeededResource)
	//{
	//	TextToWrite = CStringTable::GetInstance()->FindString("Butter") + ": ";
	//	TextToWrite += itoa(NeededResource, CharacterBuffer, 10);
	//	ReturnSize = BITMAP->DrawString("Resources/Graphics/Fonts/Font.png", TextToWrite.c_str(), LeftPageStart, NextLineTop, 0.6, 0.6, 0, D3DCOLOR_XRGB(255, 255, 255), &FitSize);
	//	NextLineTop += ReturnSize.y;
	//	FitSize.y -= ReturnSize.y;
	//}


	//NeededResource = m_Recipes[Index]->GetNeededMilk();
	//if(NeededResource)
	//{
	//	TextToWrite = CStringTable::GetInstance()->FindString("Milk") + ": ";
	//	TextToWrite += itoa(NeededResource, CharacterBuffer, 10);
	//	ReturnSize = BITMAP->DrawString("Resources/Graphics/Fonts/Font.png", TextToWrite.c_str(), LeftPageStart, NextLineTop, 0.6, 0.6, 0, D3DCOLOR_XRGB(255, 255, 255), &FitSize);
	//	NextLineTop += ReturnSize.y;
	//	FitSize.y -= ReturnSize.y;
	//}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-013
	//////////////////////////////////////////////////////////////////////////

	_itoa_s(Index + 1, CharacterBuffer, 64, 10);
	TextToWrite = CharacterBuffer;
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), LeftPageStart, GetPositionY() + 182, 1, 1, 0, D3DCOLOR_XRGB(0, 0, 0));

	// Right Page
	++Index;
	float RightPageStart = GetPositionX() + 32;
	RecipeTop = PageTop;
	NextLineTop = RecipeTop;
	FitSize.y = 223;
	FitSize.x = 329;
	if(Index < m_Recipes.size())
	{
		BITMAP->DrawStringSprite("Resources/Data/Font.spr", CStringTable::GetInstance()->FindString(m_Recipes[Index]->GetString()).c_str(), RightPageStart, NextLineTop, 0.75, 0.75, 0, D3DCOLOR_XRGB(0, 0, 0));
		NextLineTop += 164;

		GetImageFromCreationString(m_Recipes[Index]->GetString())->Draw(0, 0, D3DXVECTOR2(RightPageStart + 130.0f, NextLineTop-32));

		sprintf_s(CookTimeAsChar, 8, "%.1f", m_Recipes[Index]->GetCookTime());
		TextToWrite = CStringTable::GetInstance()->FindString("Cook for ") + CookTimeAsChar + CStringTable::GetInstance()->FindString(" seconds.");
		ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), RightPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
		NextLineTop += ReturnSize.y+8;
		FitSize.y -= ReturnSize.y;
		NeededResource = m_Recipes[Index]->GetMaxSpices();
		if(NeededResource)
		{
			TextToWrite = CStringTable::GetInstance()->FindString("May take ");
			_itoa_s(NeededResource, CharacterBuffer, 64, 10);
			TextToWrite += CharacterBuffer;
			TextToWrite += CStringTable::GetInstance()->FindString(" spices.");
			ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), RightPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
			NextLineTop += ReturnSize.y+8;
			FitSize.y -= ReturnSize.y;
		}

		NeededResource = m_Recipes[Index]->GetNeededFruit();
		if(NeededResource)
		{
			TextToWrite = CStringTable::GetInstance()->FindString("Fruit") + ": ";
			_itoa_s(NeededResource, CharacterBuffer, 64, 10);
			TextToWrite += CharacterBuffer;
			ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), RightPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
			NextLineTop += ReturnSize.y+8;
			FitSize.y -= ReturnSize.y;
		}

		NeededResource = m_Recipes[Index]->GetNeededMeat();
		if(NeededResource)
		{
			TextToWrite = CStringTable::GetInstance()->FindString("Meat") + ": ";
			_itoa_s(NeededResource, CharacterBuffer, 64, 10);
			TextToWrite += CharacterBuffer;
			ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), RightPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
			NextLineTop += ReturnSize.y+8;
			FitSize.y -= ReturnSize.y;
		}

		NeededResource = m_Recipes[Index]->GetNeededEggs();
		if(NeededResource)
		{
			TextToWrite = CStringTable::GetInstance()->FindString("Eggs") + ": ";
			_itoa_s(NeededResource, CharacterBuffer, 64, 10);
			TextToWrite += CharacterBuffer;
			ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), RightPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
			NextLineTop += ReturnSize.y+8;
			FitSize.y -= ReturnSize.y;
		}

		NeededResource = m_Recipes[Index]->GetNeededWheat();
		if(NeededResource)
		{
			TextToWrite = CStringTable::GetInstance()->FindString("Wheat") + ": ";
			_itoa_s(NeededResource, CharacterBuffer, 64, 10);
			TextToWrite += CharacterBuffer;
			ReturnSize = BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), RightPageStart, NextLineTop, 0.6f, 0.6f, 0, D3DCOLOR_XRGB(0, 0, 0), &FitSize);
			NextLineTop += ReturnSize.y+8;
			FitSize.y -= ReturnSize.y;
		}

		/////////////////////////////////////////////////////////////////		
		// BUG FIX		
		// Reference Bug # BB-013		
		// BUG FIX START		
		/////////////////////////////////////////////////////////////////

		//NeededResource = m_Recipes[Index]->GetNeededButter();
		//if(NeededResource)
		//{
		//	TextToWrite = CStringTable::GetInstance()->FindString("Butter") + ": ";
		//	TextToWrite += itoa(NeededResource, CharacterBuffer, 10);
		//	ReturnSize = BITMAP->DrawString("Resources/Graphics/Fonts/Font.png", TextToWrite.c_str(), RightPageStart, NextLineTop, 0.6, 0.6, 0, D3DCOLOR_XRGB(255, 255, 255), &FitSize);
		//	NextLineTop += ReturnSize.y;
		//	FitSize.y -= ReturnSize.y;
		//}

		//NeededResource = m_Recipes[Index]->GetNeededMilk();
		//if(NeededResource)
		//{
		//	TextToWrite = CStringTable::GetInstance()->FindString("Milk") + ": ";
		//	TextToWrite += itoa(NeededResource, CharacterBuffer, 10);
		//	ReturnSize = BITMAP->DrawString("Resources/Graphics/Fonts/Font.png", TextToWrite.c_str(), RightPageStart, NextLineTop, 0.6, 0.6, 0, D3DCOLOR_XRGB(255, 255, 255), &FitSize);
		//	NextLineTop += ReturnSize.y;
		//	FitSize.y -= ReturnSize.y;
		//}

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-013
		//////////////////////////////////////////////////////////////////////////
	}

	_itoa_s(Index + 1, CharacterBuffer, 64, 10);
	TextToWrite = CharacterBuffer;
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", TextToWrite.c_str(), RightPageStart, GetPositionY() + 182, 1, 1, 0, D3DCOLOR_XRGB(0, 0, 0));

}
///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BUG-015
//////////////////////////////////////////////////////////////////////////

void CRecipeManager::AcquireReference()
{

}

void CRecipeManager::ReleaseReference()
{

}

void CRecipeManager::FlipLeft()
{
	/////////////////////////////////////////////////////////////////		
	// BUG FIX
	// Reference Bug # BB-017	
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	if(m_Page > 0)
	{
		--m_Page;
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/PageFlip.wav", "User Interface")->Play();
	}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-017
	//////////////////////////////////////////////////////////////////////////
}

void CRecipeManager::FlipRight()
{
	/////////////////////////////////////////////////////////////////		
	// BUG FIX
	// Reference Bug # BB-017
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	if(m_Page < (m_Recipes.size() - 1) * 0.5f)
	{
		++m_Page;
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/PageFlip.wav", "User Interface")->Play();
	}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-017
	//////////////////////////////////////////////////////////////////////////
}