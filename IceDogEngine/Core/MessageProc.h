#pragma once

#include "../Utils/Common/UtilBasic.h"
#include "MessagePriority.h"
#include "../Platform/Message.h"

namespace IceDogCore
{
	// this gay is used to receive and process the message
	class MessageProc
	{
	public:
		MessageProc();
		MessageProc(const MessagePriority& priority);
		~MessageProc();

		/* regist the proc */
		void Init();

		/* set the priority of this processor */
		void SetPriority(const MessagePriority& priority);

		/* return the priority of this processor */
		MessagePriority GetPriority();

		/* bind the function to the processor */
		void BindProcessor(std::function<int(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1)> proc)
		{
			c_procFunc = proc;
		}

		/* process the message */
		virtual int Process(const IceDogPlatform::MessageType& msgType,const float& pm0,const float& pm1) 
		{
			if (c_procFunc) { return c_procFunc(msgType, pm0, pm1); }
			return 0; 
		}

	private:
		// the process function bind to this process
		std::function<int(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1)> c_procFunc;
		// the message receive priority
		MessagePriority c_msgPriority;
	};

}

