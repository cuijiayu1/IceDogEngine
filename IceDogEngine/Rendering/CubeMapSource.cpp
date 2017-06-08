#include "CubeMapSource.h"



IceDogRendering::CubeMapSource::CubeMapSource():c_isDirty(true)
{
}


IceDogRendering::CubeMapSource::~CubeMapSource()
{
	r_cubemapSRV.Release();
}

void IceDogRendering::CubeMapSource::LoadFromFile(std::wstring url, IceDogRendering::PlatformDependenceRenderResource prr)
{
	PIDShaderResource texResource_0;
	c_isDirty = ISFAILED(DirectX::CreateDDSTextureFromFile(prr.r_device, url.c_str(), &texResource_0.GetResource(), &r_cubemapSRV.GetResourceView()));
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* ptr = (ID3D11Texture2D*)texResource_0.GetResource();
	ptr->GetDesc(&desc);
	c_cubemapSize = desc.Width;
	texResource_0.Release();
}

bool IceDogRendering::CubeMapSource::IsDirty()
{
	return c_isDirty;
}

void IceDogRendering::CubeMapSource::Close()
{
	r_cubemapSRV.Release();
}

IceDogRendering::PIDShaderResourceView IceDogRendering::CubeMapSource::GetCubeMapSRV()
{
	return r_cubemapSRV;
}

void IceDogRendering::CubeMapSource::ResetCubeMapSRV(PIDShaderResourceView pidSRV, int cubemapSize)
{
	c_cubemapSize = cubemapSize;
	r_cubemapSRV.Release();
	r_cubemapSRV = pidSRV;
	c_isDirty = true;
}

int IceDogRendering::CubeMapSource::GetCubemapSize()
{
	return c_cubemapSize;
}
