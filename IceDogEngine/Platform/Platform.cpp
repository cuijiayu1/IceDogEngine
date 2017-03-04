#include "Platform.h"

using namespace IceDogPlatform;

Platform::Platform(PlatformWindow pfWindow, std::ostream& errorLog):s_errorlogOutStream(errorLog)
{
	r_pfWindow = pfWindow;
	// for windows 
#if defined __WINDOWS__
	r_platformMsgHandle = std::make_shared<WindowsMessageHandle>();
	r_platformWindowHandle = std::make_unique<WindowsWindowHandle>(r_pfWindow.width, r_pfWindow.height, errorLog);
#endif

	// when nullptr, no correct platform config
	if (r_platformMsgHandle == nullptr) 
	{
		s_errorlogOutStream << "No correct platform config" << std::endl;
	}
	assert(r_platformMsgHandle != nullptr);
	assert(r_platformWindowHandle != nullptr);
}

void Platform::InitPlatform()
{
	// register the message handle
	r_platformWindowHandle->RegistMessageHandle(r_platformMsgHandle);
	// init the window
	r_platformWindowHandle->InitWindow(r_pfWindow);
	// update the current platform window
	r_pfWindow = r_platformWindowHandle->GetFullPlatformWindow();
}

void Platform::TickPlatform()
{
	// check the message
	r_platformMsgHandle->CheckMessage();
}

void Platform::RegistMessageProcessChain(std::function<void(Message)> msProcChain)
{
	// bind the message process chain to the message handle
	r_platformMsgHandle->RegistMessageProcessChain(msProcChain);
}

void IceDogPlatform::Platform::SetWindowTitle(std::string title)
{
	r_platformWindowHandle->SetWindowTitle(title);
}
