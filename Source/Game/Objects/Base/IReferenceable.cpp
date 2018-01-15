#include "IReferenceable.h"

IReferenceable::IReferenceable()
{
	m_References = 0;
}

IReferenceable::IReferenceable(const IReferenceable& Source)
{
	m_References = 0;
}

IReferenceable& IReferenceable::operator=(const IReferenceable& Source)
{
	m_References = 0;
	return *this;
}

IReferenceable::~IReferenceable()
{

}

void IReferenceable::AcquireReference()
{
	++m_References;
}

void IReferenceable::ReleaseReference()
{
	if(m_References <= 1)
		delete this;
	else
		--m_References;
}