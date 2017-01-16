#pragma once

#include "MessagePriority.h"
#include "../Platform/Message.h"

namespace IceDogCore
{
	// this gay is used to receive and process the message
	class MessageProc
	{
	public:
		MessageProc();
		~MessageProc();

		void SetPriority(const MessagePriority& priority);

		MessagePriority GetPriority();

		/* process the message */
		virtual int Process(const IceDogPlatform::MessageType& msgType,const float& pm0,const float& pm1) { return 0; }

	private:
		MessagePriority c_msgPriority;
	};

}

