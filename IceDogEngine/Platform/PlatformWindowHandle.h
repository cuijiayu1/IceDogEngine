#pragma once

#include "../Utils/Common/UtilBasic.h"
#include "PlatformWindow.h"

namespace IceDogPlatform
{
	// platform independence window abstract class
	class PlatformWindowHandle
	{
	public:
		PlatformWindowHandle(int width, int height, std::ostream& oss)
			: c_windowWidth(width),
			c_windowHeight(height),
			s_errorlogOutStream(oss)
		{}

		/* init the window, setup message loop */
		virtual bool InitWindow(PlatformWindow pfWindow) = 0;

		/* bind the message handle to receive the window message */
		virtual void RegistMessageHandle(std::shared_ptr<class PlatformMsgHandle> msgHandle) = 0;

		/* get the final Platform window, this must called after window init */
		PlatformWindow GetFullPlatformWindow() { return r_platformWindow; }

		/* Set Window Title */
		virtual void SetWindowTitle(std::string title) = 0;

	protected:
		// the width of the window
		int c_windowWidth;
		// the height of the window
		int c_windowHeight;

	protected:
		// the platform window ref
		PlatformWindow r_platformWindow;

	protected:
		// error log out put port
		std::ostream& s_errorlogOutStream;
	};
}