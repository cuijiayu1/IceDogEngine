#pragma once

#include "../PlatformWindowHandle.h"

namespace IceDogPlatform
{
	class WindowsWindowHandle :public PlatformWindowHandle
	{
	public:
		WindowsWindowHandle(int width, int height, std::ostream& oss)
:						 PlatformWindowHandle(width, height, oss) {}

		/* init the window, setup message loop */
		bool InitWindow(PlatformWindow pfWindow) override;

		/* Regist the message handle to the window */
		void RegistMessageHandle(std::shared_ptr<class PlatformMsgHandle> msgHandle) override;
	};
}

