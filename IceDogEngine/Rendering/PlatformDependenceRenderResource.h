#pragma once
#include "../Utils/Common/UtilBasic.h"

namespace IceDogRendering
{
	// for platform independence 
	struct PlatformDependenceRenderResource
	{
#if defined __WINDOWS__
#if defined __DIRECTX__
		ID3D11Device* r_device;
		ID3D11DeviceContext* r_deviceContext;
#endif
#endif
	}; 
}