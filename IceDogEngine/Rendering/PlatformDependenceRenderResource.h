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

	class PIDShaderResourceView
	{
	public:
		PIDShaderResourceView();
#if defined __DIRECTX__
		ID3D11ShaderResourceView*& GetResourceView();
		void SetResourceView(ID3D11ShaderResourceView* res);
#endif
		/* release the hold resource */
		void Release();
	private:
#if defined __DIRECTX__
		ID3D11ShaderResourceView* r_shaderResourceView;
#endif
	};

	class PIDShaderResource
	{
	public:
		PIDShaderResource();
#if defined __DIRECTX__
		ID3D11Resource*& GetResource();
		void SetResource(ID3D11Resource* res);
#endif
		/* release the hold resource */
		void Release();
	private:
#if defined __DIRECTX__
		ID3D11Resource* r_shaderResource;
#endif
	};
}