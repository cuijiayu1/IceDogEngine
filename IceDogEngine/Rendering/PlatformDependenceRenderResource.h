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

	// all of the following resource should not be auto release
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

	class PIDRenderTargetView
	{
	public:
		PIDRenderTargetView();
#if defined __DIRECTX__
		ID3D11RenderTargetView*& GetRenderTargetView();
		void SetRenderTargetView(ID3D11RenderTargetView* res);
#endif
		/* release the hold resource */
		void Release();
	private:
#if defined __DIRECTX__
		ID3D11RenderTargetView* r_renderTargetView;
#endif
	};

	class PIDDepthStencilView
	{
	public:
		PIDDepthStencilView();
#if defined __DIRECTX__
		ID3D11DepthStencilView*& GetDepthStencilView();
		void SetDepthStencilView(ID3D11DepthStencilView* res);
#endif
		/* release the hold resource */
		void Release();
	private:
#if defined __DIRECTX__
		ID3D11DepthStencilView* r_depthStencilView;
#endif
	};

	class PIDTexture2D
	{
	public:
		PIDTexture2D();
#if defined __DIRECTX__
		ID3D11Texture2D*& GetTexture();
		void SetTexture(ID3D11Texture2D* res);
#endif
		/* release the hold resource */
		void Release();

	private:
#if defined __DIRECTX__
		ID3D11Texture2D* r_texture2d;
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

	struct PIDViewport
	{
		float c_topLeftX;
		float c_topLeftY;
		float c_width;
		float c_height;
		float c_minDepth;
		float c_maxDepth;
	};
}