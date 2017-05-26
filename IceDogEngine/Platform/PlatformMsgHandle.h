#pragma once

#include "PlatformConfig.h"
#include "PlatformMessage.h"
#include "Message.h"

namespace IceDogPlatform
{
	// to handle the platform indenpendence message, abstract class
	class PlatformMsgHandle
	{
	public:

		/* check the message from specially platform */
		virtual void CheckMessage()=0;

		/* call close function to release the resource */
		virtual void Close() = 0;

		/* process the platform independence message, the message usually comes from window */
		virtual MessageResult ProcessMessage(MessageParam params)=0;

		/* register message process chain */
		virtual void RegistMessageProcessChain(std::function<void(Message)> msProcChain) { c_messageProcessChain = msProcChain; }

	protected:
		// message process chain, to handle the message
		std::function<void(Message)> c_messageProcessChain;
	};
}

