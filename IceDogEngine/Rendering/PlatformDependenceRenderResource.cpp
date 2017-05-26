#include "PlatformDependenceRenderResource.h"

IceDogRendering::PIDShaderResourceView::PIDShaderResourceView()
{
#if defined __DIRECTX__
	r_shaderResourceView = nullptr;
#endif
}

ID3D11ShaderResourceView*& IceDogRendering::PIDShaderResourceView::GetResourceView()
{
	return r_shaderResourceView;
}

void IceDogRendering::PIDShaderResourceView::SetResourceView(ID3D11ShaderResourceView* res)
{
	r_shaderResourceView = res;
}

void IceDogRendering::PIDShaderResourceView::Release()
{
	if(r_shaderResourceView)
		ReleaseCOM(r_shaderResourceView);
	r_shaderResourceView = nullptr;
}

IceDogRendering::PIDShaderResource::PIDShaderResource()
{
#if defined __DIRECTX__
	r_shaderResource = nullptr;
#endif
}

ID3D11Resource*& IceDogRendering::PIDShaderResource::GetResource()
{
	return r_shaderResource;
}

void IceDogRendering::PIDShaderResource::SetResource(ID3D11Resource* res)
{
	r_shaderResource = res;
}

void IceDogRendering::PIDShaderResource::Release()
{
	if(r_shaderResource)
		ReleaseCOM(r_shaderResource);
	r_shaderResource = nullptr;
}

IceDogRendering::PIDRenderTargetView::PIDRenderTargetView()
{
#if defined __DIRECTX__
	r_renderTargetView = nullptr;
#endif
}

ID3D11RenderTargetView*& IceDogRendering::PIDRenderTargetView::GetRenderTargetView()
{
	return r_renderTargetView;
}

void IceDogRendering::PIDRenderTargetView::SetRenderTargetView(ID3D11RenderTargetView* res)
{
	r_renderTargetView = res;
}

void IceDogRendering::PIDRenderTargetView::Release()
{
	if (r_renderTargetView)
		ReleaseCOM(r_renderTargetView);
	r_renderTargetView = nullptr;
}

IceDogRendering::PIDTexture2D::PIDTexture2D()
{
#if defined __DIRECTX__
	r_texture2d = nullptr;
#endif
}

ID3D11Texture2D*& IceDogRendering::PIDTexture2D::GetTexture()
{
	return r_texture2d;
}

void IceDogRendering::PIDTexture2D::SetTexture(ID3D11Texture2D* res)
{
	r_texture2d = res;
}

void IceDogRendering::PIDTexture2D::Release()
{
	if (r_texture2d)
		ReleaseCOM(r_texture2d);
	r_texture2d = nullptr;
}

IceDogRendering::PIDDepthStencilView::PIDDepthStencilView()
{
	r_depthStencilView = nullptr;
}

ID3D11DepthStencilView*& IceDogRendering::PIDDepthStencilView::GetDepthStencilView()
{
	return r_depthStencilView;
}

void IceDogRendering::PIDDepthStencilView::SetDepthStencilView(ID3D11DepthStencilView* res)
{
	r_depthStencilView = res;
}

void IceDogRendering::PIDDepthStencilView::Release()
{
	if (r_depthStencilView)
		ReleaseCOM(r_depthStencilView);
	r_depthStencilView = nullptr;
}
