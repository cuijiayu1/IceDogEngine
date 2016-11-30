#include "WindowsWindowHandle.h"
#include "../PlatformMsgHandle.h"

using namespace IceDogPlatform;

namespace
{
	// the global msg handle
	std::shared_ptr<PlatformMsgHandle> globalMsgHandle = nullptr;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MessageParam mp;
	mp.c_windowInstance = hwnd;
	mp.c_message = msg;
	mp.c_wparam = wParam;
	mp.c_lparam = lParam;
	if (globalMsgHandle!=nullptr) //when message handle no bind, just use the default message process.
	{
		auto result = globalMsgHandle->ProcessMessage(mp);
		return result.c_result;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void WindowsWindowHandle::RegistMessageHandle(std::shared_ptr<class PlatformMsgHandle> msgHandle)
{
	globalMsgHandle = msgHandle;
}

bool WindowsWindowHandle::InitWindow(PlatformWindow pfWindow)
{
	r_platformWindow = pfWindow;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = r_platformWindow.c_windowInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		s_errorlogOutStream << "RegisterClass Failed." << std::flush;
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, c_windowWidth, c_windowHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	r_platformWindow.c_window = CreateWindow("D3DWndClassName", "LLEngineg",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, r_platformWindow.c_windowInstance, 0);
	if (!r_platformWindow.c_window)
	{
		s_errorlogOutStream << "CreateWindow Failed." << std::flush;
		return false;
	}

	ShowWindow(r_platformWindow.c_window, SW_SHOW);
	UpdateWindow(r_platformWindow.c_window);

	return true;
}