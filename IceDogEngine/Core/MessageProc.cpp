#include "MessageProc.h"

namespace IceDogCore
{
	MessageProc::MessageProc()
	{
		c_msgPriority = MessagePriority::INPUT_1;
	}


	MessageProc::~MessageProc()
	{
	}

	void MessageProc::SetPriority(const MessagePriority& priority)
	{
		c_msgPriority = priority;
	}
	MessagePriority MessageProc::GetPriority()
	{
		return c_msgPriority;
	}
}