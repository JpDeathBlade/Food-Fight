//////////////////////////////////////////////////////
// File: "CRecipe.cpp"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#include "CRecipe.h"
#include "../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"

CRecipe::CRecipe(std::string _unitString, float _time, unsigned int _eggs, unsigned int _wheat, unsigned int _fruit, unsigned int _meat, float _milk, unsigned int _cheese, float _butter)
{
	unitCreationString = _unitString;
	m_NeededEggs = _eggs;
	m_NeededWheat = _wheat;
	m_NeededFruit = _fruit;
	m_NeededMeat = _meat;
	m_NeededMilk = _milk;
	m_NeededCheese = _cheese;
	m_NeededButter = _butter;
	m_CookTime = _time;
}

CCookAction::CCookAction(std::string _text, float _time)
{
	m_ActionText = _text;
	m_ActionTime = _time;
}

CSprite* GetImageFromCreationString(std::string _create)
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	if(_create == "Toast")
	{
		return SpriteManager->GetSprite("Resources/Data/Toast.spr");
	}
	else if(_create == "Sunny Side Up Egg")
	{
		return SpriteManager->GetSprite("Resources/Data/SunnySideUp.spr");
	}
	else if(_create == "Scrambled Eggs")
	{
		return SpriteManager->GetSprite("Resources/Data/Scambled.spr");
	}
	else if(_create == "Sausage")
	{
		return SpriteManager->GetSprite("Resources/Data/Sausage.spr");
	}
	else if(_create == "Ham")
	{
		return SpriteManager->GetSprite("Resources/Data/Ham.spr");
	}
	else if(_create == "Pancake")
	{
		return SpriteManager->GetSprite("Resources/Data/Pancake.spr");
	}
	else if(_create == "Waffle")
	{
		return SpriteManager->GetSprite("Resources/Data/Waffle.spr");
	}
	else if(_create == "Blueberry Muffin")
	{
		return SpriteManager->GetSprite("Resources/Data/BlueBerryMuffin.spr");
	}
	else if(_create == "French Toast")
	{
		return SpriteManager->GetSprite("Resources/Data/FrenchToast.spr");
	}
	else if(_create == "Grapefruit")
	{
		return SpriteManager->GetSprite("Resources/Data/GrapeFruit.spr");
	}
	else if(_create == "Banana")
	{
		return SpriteManager->GetSprite("Resources/Data/Banana.spr");
	}
	else if(_create == "Strawberries")
	{
		return SpriteManager->GetSprite("Resources/Data/FF_Unit_Strawberry.PNG");
	}
	return NULL;
}