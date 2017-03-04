#pragma once

//
//    PlatformMessage->PlatformMsgHandle<------WindowsMessageHandle
//    PlatformWindow->PlatformWindowHandle<------WindowsWindowHandle
//

#include "PlatformMsgHandle.h"
#include "PlatformWindowHandle.h"

#if defined __WINDOWS__
#include "Windows\WindowsMessageHandle.h"
#include "Windows\WindowsWindowHandle.h"
#endif //windows platform

namespace IceDogPlatform
{
	class Platform final
	{
	public:
		Platform(PlatformWindow pfWindow,std::ostream& errorLog);

		/* init the platform both setup the message and window event */
		void InitPlatform();

		/* tick platform, update the information update the message loop */
		void TickPlatform();

		/* register message process chain */
		void RegistMessageProcessChain(std::function<void(Message)> msProcChain);

		/* get the constructed platformWindow */
		PlatformWindow GetConstructedPlatformWindow() { return r_pfWindow; }

		/* set window title */
		void SetWindowTitle(std::string title);

	private:
		// the ptr to the platform msg handle to handle the platform independence message
		std::shared_ptr<PlatformMsgHandle> r_platformMsgHandle;
		// the ptr to the platform windows handle to deal the window
		std::unique_ptr<PlatformWindowHandle> r_platformWindowHandle;
		// the platform window
		PlatformWindow r_pfWindow;
	private:
		// error out put port
		std::ostream& s_errorlogOutStream;
	};
}

