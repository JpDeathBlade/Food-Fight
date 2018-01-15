//////////////////////////////////////////////////////
// File: "CRecipe.h"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#ifndef CRECIPE_H_
#define CRECIPE_H_

#include "CCookingResource.h"

class CCookAction;
class CSprite;

class CRecipe
{
private:
	std::string unitCreationString;
	unsigned int m_NeededEggs;
	unsigned int m_NeededWheat;
	unsigned int m_NeededFruit;
	unsigned int m_NeededMeat;
	float m_NeededMilk;
	unsigned int m_NeededCheese;
	float m_NeededButter;

	float m_CookTime;

	std::vector<CCookAction*> m_CookActions;
public:
	CRecipe(std::string _unitString, float _time, unsigned int _eggs, unsigned int _wheat, unsigned int _fruit, unsigned int _meat, float _milk, unsigned int _cheese, float _butter);

	unsigned int GetNeededEggs() { return m_NeededEggs; }
	unsigned int GetNeededWheat() { return m_NeededWheat; }
	unsigned int GetNeededFruit() { return m_NeededFruit; }
	unsigned int GetNeededMeat() { return m_NeededMeat; }
	float GetNeededMilk() { return m_NeededMilk; }
	unsigned int GetNeededCheese() { return m_NeededCheese; }
	float GetNeededButter() { return m_NeededButter; }
	float GetCookTime() { return m_CookTime; }
	unsigned int GetMaxSpices() { return 2; }
	std::vector<CCookAction*>* GetCookActions() { return &m_CookActions; }

	std::string GetString() { return unitCreationString; }
};

class CCookAction
{
private:
	std::string m_ActionText;
	float m_ActionTime;
public:
	CCookAction(std::string _text, float _time);

	float GetActionTime() { return m_ActionTime; }
	std::string GetActionText() { return m_ActionText; }
};


CSprite* GetImageFromCreationString(std::string _create);

#endif