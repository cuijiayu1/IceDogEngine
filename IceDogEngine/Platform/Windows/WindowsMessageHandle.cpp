#include "WindowsMessageHandle.h"

using namespace IceDogPlatform;

WindowsMessageHandle::WindowsMessageHandle()
{
}

void WindowsMessageHandle::CheckMessage()
{
	// read windows message
	MSG msg = { 0 };
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

MessageResult WindowsMessageHandle::ProcessMessage(MessageParam params)
{
	MessageResult res;
	res.c_result = 0;
	Message msg;
	switch (params.c_message)
	{
	case WM_LBUTTONDBLCLK: // left button double click
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::leftButtonDoubleClick;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_LBUTTONUP:  // left button up
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::leftButtonUp;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_LBUTTONDOWN: //left button down
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::leftButtonDown;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_RBUTTONDBLCLK: // right button double click
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::rightButtonDoubleClick;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_RBUTTONUP:  //right button up
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::rightButtonUp;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_RBUTTONDOWN: // right button down
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::rightButtonDown;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_MOUSEMOVE:  //mouse move, position in window space
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::mouseMove;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_MOUSEWHEEL:
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::mouseWheel;
		msg.c_param0 = HIWORD(params.c_wparam)>500;
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_MBUTTONDBLCLK:
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::midButtonDoubleClick;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_MBUTTONDOWN:
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::midButtonDown;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_MBUTTONUP:
		msg.c_messageAuthority = MessageAuthority::USER;
		msg.c_messageType = MessageType::midButtonUp;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	case WM_SIZE:
		msg.c_messageAuthority = MessageAuthority::SYSTEM;
		msg.c_messageType = MessageType::systemResize;
		msg.c_param0 = LOWORD(params.c_lparam);
		msg.c_param1 = HIWORD(params.c_lparam);
		if (c_messageProcessChain) { c_messageProcessChain(msg); }
		break;
	default:
		res.c_result = DefWindowProc(params.c_windowInstance, params.c_message, params.c_wparam, params.c_lparam);
		break;
	}
	return res;
}