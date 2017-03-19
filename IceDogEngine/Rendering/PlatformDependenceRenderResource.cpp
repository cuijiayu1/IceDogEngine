#include "PlatformDependenceRenderResource.h"

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
	ReleaseCOM(r_shaderResourceView);
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
	ReleaseCOM(r_shaderResource);
}
