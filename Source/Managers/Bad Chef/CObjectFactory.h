//////////////////////////////////////////////////////
// File: "CObjectFactory.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef _CObjectFactory_H_
#define _CObjectFactory_H_
#include <string>
#include <map>

class IObject;
class CObjectManager;

class CObjectFactory
{
public:
	typedef IObject* (*FunctionPointer)(CObjectManager*);

private:
	static CObjectFactory* m_Instance;
	std::map<std::string, FunctionPointer> m_FactoryMethods;
	
	CObjectFactory(); // Default Constructor
	CObjectFactory(const CObjectFactory& Source); // Copy Constructor - Disabled
	CObjectFactory& operator=(const CObjectFactory& Source); // Assignment Operator - Disabled
	~CObjectFactory(); // Destructor
public:
	static CObjectFactory* GetInstance(); // Returns the singleton's instance.
	static void DeleteInstance(); // Deletes the singleton's instance.

	bool AddMethod(std::string Identifier, FunctionPointer Method); // Attempts to associate the provided function with the provided string identifier. If the provided string identifier is already associated, false is returned. Else, the provided string identifier is associated with the provided function and true is returned.
	bool RemoveMethod(std::string Identifier); // Attempts to disassociate the provided string identifier from its associated function. If the provided string identifier is already associated, the provided string identifier is disassociated from its associated function and true is returned. Else, false is returned.
	void ClearMethods(); // Disassociates all string identifiers.
	IObject* Construct(std::string Identifier, CObjectManager* ObjectManager); // Attempts to call the function associated to the provided string identifier. If found, the function is called and its return value is returned. Else, NULL is returned. If you would like to add the object at the beginning of the next frame, pass in true as the 3rd param.
};
#endif