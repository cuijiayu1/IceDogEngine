#pragma once

#include "../../ThirdParty/rapidxml/rapidxml.hpp"
#include "../../ThirdParty/rapidxml/rapidxml_utils.hpp"
#include "../../ThirdParty/rapidxml/rapidxml_print.hpp"
#include "../../Platform/PlatformConfig.h"

#if defined(__WINDOWS__)
#include <d3d11.h>
#include <DirectXMath.h>
#include <Windows.h>
#include "DirectXTex.h"
#include "Model.h"
using namespace DirectX;
#endif

#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>


// directx Utils
#if defined(__WINDOWS__)
namespace IceDogUtils
{
	//---------------------------------------------------------------------------------------
	// Simple check if field
	//---------------------------------------------------------------------------------------
#define ISFAILED(hr) (((HRESULT)(hr)) < 0)

	//---------------------------------------------------------------------------------------
	// Simple d3d error checker.
	//---------------------------------------------------------------------------------------

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                               \
	{                                                           \
		HRESULT hr = (x);                                       \
		if(FAILED(hr))                                          \
		{                                                       \
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                       \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

	//---------------------------------------------------------------------------------------
	// Simple com release.
	//---------------------------------------------------------------------------------------
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

	namespace Colors
	{
		const float White[]{ 1.0f, 1.0f, 1.0f, 1.0f };
		const float Black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		const float Red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		const float Green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
		const float Blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		const float Yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		const float Cyan[] = { 0.0f, 1.0f, 1.0f, 1.0f };
		const float Magenta[] = { 1.0f, 0.0f, 1.0f, 1.0f };

		const float Silver[] = { 0.75f, 0.75f, 0.75f, 1.0f };
		const float LightSteelBlue[] = { 0.69f, 0.77f, 0.87f, 1.0f };
	}
}
#endif

namespace IceDogUtils
{
	/* convert char* to wcahr_t* */
	wchar_t* char2wchar(char* CStr);

	/* read all bytes */
	std::vector<char> ReadAllBytes(char const* filename);
}