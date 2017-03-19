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
	texResource_0.Release();
}

bool IceDogRendering::CubeMapSource::IsDirty()
{
	return c_isDirty;
}

IceDogRendering::PIDShaderResourceView IceDogRendering::CubeMapSource::GetCubeMapSRV()
{
	return r_cubemapSRV;
}
