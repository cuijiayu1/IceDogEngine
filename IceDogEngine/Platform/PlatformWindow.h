#pragma once
#include <functional>

#if defined(__WINDOWS__)
#include <Windows.h>
#endif // defined(__WINDOWS__)

// for platform independent 
namespace IceDogPlatform
{
	struct PlatformWindow
	{
#if defined(__WINDOWS__)
		HINSTANCE c_windowInstance;
		HWND c_window;
#endif // defined(__WINDOWS__)
		// this two value only used in initilize stage
		int width=800;
		int height=600;
	};
}