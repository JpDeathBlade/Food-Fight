#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "../../../Game/Objects/Base/IObject.h"
#include "../CObjectManager.h"
#include "IMessage.h"

class CCreateCookingUnitMessage : public IMessage
{
private:
	CObjectManager* m_ObjectManager;
	std::string m_CreationString;
	int m_X;
	int m_Y;
	std::string m_UnitCreationString;
public:
	CCreateCookingUnitMessage(CObjectManager* _om, std::string _create, int _x, int _y, std::string _unitCreate);

	void Process();
};

class CCreateObjectMessage : public IMessage
{
private:
	CObjectManager* m_ObjectManager;
	std::string m_CreationString;
	int m_X;
	int m_Y;
public:
	CCreateObjectMessage(CObjectManager* _om, std::string _create, int _x, int _y);
	void Process();
};


#endif