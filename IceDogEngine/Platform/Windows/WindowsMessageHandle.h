#pragma once

#include "../PlatformMsgHandle.h"

namespace IceDogPlatform
{
	class WindowsMessageHandle : public PlatformMsgHandle
	{
	public:
		WindowsMessageHandle();
		
		// check the message from windows platform
		void CheckMessage() override;
		// process the message from windows platform
		MessageResult ProcessMessage(MessageParam params) override;
	};
}

