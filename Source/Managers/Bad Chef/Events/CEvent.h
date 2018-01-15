#ifndef _CEvent_H_
#define _CEvent_H_
#include "../../../Game/Objects/Base/IFunctor.h"
#include <vector>

template<typename SenderType>
class CEventFunctor : public IFunctor
{
	SenderType* m_Sender;
public:
	SenderType* GetSender() const;
	void SetSender(SenderType* const Sender);

	virtual void operator()() = 0;
};

template<typename SenderType>
SenderType* CEventFunctor<SenderType>::GetSender() const
{
	return m_Sender;
}

template<typename SenderType>
void CEventFunctor<SenderType>::SetSender(SenderType* const Sender)
{
	m_Sender = Sender;
}

template<typename SenderType>
class CEvent
{
	std::vector<CEventFunctor<SenderType>*> m_EventHandlers;
public:
	CEvent(); // Default Constructor
	CEvent(const CEvent& Source); // Copy Constructor
	CEvent& operator=(const CEvent& Source); // Assignment Operator
	~CEvent(); // Destructor

	void Trigger(SenderType* Sender); // Notifies all event handlers of the occuring event.

	void operator+=(CEventFunctor<SenderType>* EventHandler);
	void operator=(CEventFunctor<SenderType>* EventHandler);
	void operator-=(CEventFunctor<SenderType>* EventHandler);
};

template<typename SenderType>
CEvent<SenderType>::CEvent()
{

}

template<typename SenderType>
CEvent<SenderType>::CEvent(const CEvent& Source)
{

}

template<typename SenderType>
CEvent<SenderType>& CEvent<SenderType>::operator=(const CEvent& Source)
{
	return *this;
}

template<typename SenderType>
CEvent<SenderType>::~CEvent()
{

}

template<typename SenderType>
void CEvent<SenderType>::Trigger(SenderType* Sender)
{
	for(unsigned char i = 0; i < m_EventHandlers.size(); ++i)
	{
		m_EventHandlers[i]->SetSender(Sender);
		(*m_EventHandlers[i])();
	}
}

template<typename SenderType>
void CEvent<SenderType>::operator+=(CEventFunctor<SenderType>* EventHandler)
{
	m_EventHandlers.push_back(EventHandler);
}

template<typename SenderType>
void CEvent<SenderType>::operator=(CEventFunctor<SenderType>* EventHandler)
{
	m_EventHandlers.clear();
	m_EventHandlers.push_back(EventHandler);
}

template<typename SenderType>
void CEvent<SenderType>::operator-=(CEventFunctor<SenderType>* EventHandler)
{
	for(std::vector<CEventFunctor<SenderType>*>::iterator i = m_EventHandlers.begin(); i != m_EventHandlers.end(); ++i)
	{
		if((*i) == EventHandler)
			i = m_EventHandlers.erase(i);
		return;
	}
}
#endif