#ifndef _IMessage_H_
#define _IMessage_H_
class IMessage
{
public:
	IMessage(); // Default Constructor
	IMessage(const IMessage& Source); // Copy Constructor
	virtual IMessage& operator=(const IMessage& Source); // Assignment Operator
	virtual ~IMessage(); // Destructor

	virtual void Process() = 0; // Processes any message-specific logic.
};
#endif