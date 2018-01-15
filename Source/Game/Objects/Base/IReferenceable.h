#ifndef _IReferenceable_H_
#define _IReferenceable_H_

class IReferenceable
{
	unsigned short m_References;
public:
	IReferenceable(); // Default Constructor
	IReferenceable(const IReferenceable& Source); // Copy Constructor
	virtual IReferenceable& operator=(const IReferenceable& Source); // Assignment Operator
	virtual ~IReferenceable(); // Destructor

	virtual void AcquireReference(); // Adds a reference to the object.
	virtual void ReleaseReference(); // Removes a reference from the object.
};
#endif