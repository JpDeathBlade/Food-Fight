#ifndef _CMessageSystem_H_
#define _CMessageSystem_H_
#include <vector>

class IMessage;

class CMessageSystem
{
	static CMessageSystem* m_Instance;
	std::vector<IMessage*> m_MessageQueue;

	CMessageSystem(); // Default Constructor
	CMessageSystem(const CMessageSystem& Source); // Copy Constructor - Disabled
	CMessageSystem& operator=(const CMessageSystem& Source); // Assignment Operator - Disabled
	~CMessageSystem(); // Destructor
public:
	static CMessageSystem* GetInstance(); // Returns the instance of the singleton.
	static void DeleteInstance(); // Deletes the instance of the singleton.

	void AddMessage(IMessage* Message); // Posts a message into the message queue.
	void ProcessMessages(); // Processes the messages in the queue.
	void ClearMessages(); // Clears all messages from the queue.
};

#endif