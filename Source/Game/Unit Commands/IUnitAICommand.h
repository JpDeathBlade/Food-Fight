//////////////////////////////////////////////////////
// File: "IUnitAIComand.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/14/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef _IUnitAICommand_H_
#define _IUnitAICommand_H_
#include "../Objects/Base/IReferenceable.h"

class CUnitObject;

class IUnitAICommand : public IReferenceable
{
public:
	IUnitAICommand();
	IUnitAICommand(const IUnitAICommand& Source);
	virtual IUnitAICommand& operator=(const IUnitAICommand& Source);
	virtual ~IUnitAICommand();

	virtual bool Execute(CUnitObject* Object) = 0;
};
#endif