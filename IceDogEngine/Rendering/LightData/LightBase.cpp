#include "LightBase.h"

IceDogRendering::LightBase::LightBase():c_lightTextureDirty(true)
{
}

IceDogRendering::LightBase::~LightBase()
{
	r_shadowMapRTV.Release();
	r_shadowMapSRV.Release();
	r_shadowTexture2d.Release();
	r_depthStencilView.Release();
	r_depthStencilTexture2d.Release();
}

void IceDogRendering::LightBase::SetLightDef(std::shared_ptr<LightDef> lightDef)
{
	r_lightDef = lightDef;
}

std::shared_ptr<IceDogRendering::LightDef> IceDogRendering::LightBase::GetLightDef()
{
	return r_lightDef;
}

IceDogRendering::PIDRenderTargetView IceDogRendering::LightBase::GetShadowMapRTV()
{
	return r_shadowMapRTV;
}

IceDogRendering::PIDShaderResourceView IceDogRendering::LightBase::GetShadowMapSRV()
{
	return r_shadowMapSRV;
}

IceDogRendering::PIDDepthStencilView IceDogRendering::LightBase::GetDepthStencilView()
{
	return r_depthStencilView;
}

IceDogRendering::float4x4 IceDogRendering::LightBase::GetProjectionMatrix()
{
	return c_projectionMatrix;
}

IceDogRendering::float4x4 IceDogRendering::LightBase::GetViewMatrix()
{
	return c_viewMatrix;
}

IceDogRendering::PIDViewport& IceDogRendering::LightBase::GetViewport()
{
	return r_viewport;
}

float IceDogRendering::LightBase::GetShadowMapSize()
{
	return c_shadowMapSize;
}

