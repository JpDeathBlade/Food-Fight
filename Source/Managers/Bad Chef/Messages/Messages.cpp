#include "Messages.h"
#include "../CObjectFactory.h"
#include "../../../Game/Cooking Mini-Game/CCookingUnit.h"
#include "../../../Game/Objects/Units/CUnitObject.h"

CCreateCookingUnitMessage::CCreateCookingUnitMessage(CObjectManager* _om, std::string _create, int _x, int _y, std::string _unitCreate)
{
	m_ObjectManager = _om;
	m_CreationString = _create;
	m_X = _x;
	m_Y = _y;
	m_UnitCreationString = _unitCreate;
}

void CCreateCookingUnitMessage::Process()
{
	CCookingUnit* newObj = static_cast<CCookingUnit*>(CObjectFactory::GetInstance()->Construct(m_CreationString, m_ObjectManager));
	newObj->SetPosition((float)(m_X), (float)(m_Y));
	newObj->SetCreationString(m_UnitCreationString);
}

CCreateObjectMessage::CCreateObjectMessage(CObjectManager* _om, std::string _create, int _x, int _y)
{
	m_ObjectManager = _om;
	m_CreationString = _create;
	m_X = _x;
	m_Y = _y;
}

void CCreateObjectMessage::Process()
{
	CGameObject* newObj = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct(m_CreationString, m_ObjectManager));
	CUnitObject* testUnit = NULL;
	if(testUnit = dynamic_cast<CUnitObject*>(newObj))
	{
		testUnit->SetTeamValue(TEAM_PLAYER);
		testUnit->SetPosition((float)(m_X), (float)(m_Y));
		return;
	}
	newObj->SetPosition((float)(m_X), (float)(m_Y));
}