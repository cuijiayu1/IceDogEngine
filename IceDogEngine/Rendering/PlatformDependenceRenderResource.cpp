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
