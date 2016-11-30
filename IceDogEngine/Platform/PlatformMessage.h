#pragma once

#include "../Utils/Common/UtilBasic.h"

// for platform independent 
namespace IceDogPlatform
{
	struct MessageParam
	{
#if defined(__WINDOWS__)
		HWND c_windowInstance;
		UINT c_message;
		WPARAM c_wparam;
		LPARAM c_lparam;
#endif // defined(__WINDOWS__)
	};

	struct MessageResult
	{
#if defined(__WINDOWS__)
		LRESULT c_result;
#endif // defined(__WINDOWS__)
	};
}