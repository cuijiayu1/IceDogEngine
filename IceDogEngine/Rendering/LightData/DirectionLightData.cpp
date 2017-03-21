#include "DirectionLightData.h"

IceDogRendering::DirectionLightData::DirectionLightData():c_shadowHeight(4),c_shadowWidth(4)
{
	r_lightDef = std::make_shared<DirectionalLight>();
	r_viewport.c_height = c_shadowMapSize;
	r_viewport.c_width = c_shadowMapSize;
	r_viewport.c_topLeftX = 0;
	r_viewport.c_topLeftY = 0;
	r_viewport.c_minDepth = 0;
	r_viewport.c_maxDepth = 1;

	c_projectionMatrix = IceDogRendering::float4x4::OrthographicLH(c_shadowWidth, c_shadowHeight, DEFAULT_NEARZ, DEFAULT_FARZ);
	c_viewMatrix = IceDogRendering::float4x4::LookAtLH(float3(0, 10, 0), float3(0, 0, 0), float3(1, 0, 0));
}

void IceDogRendering::DirectionLightData::SetIntensity(float intensity)
{
	((DirectionalLight*)r_lightDef.get())->intensity = intensity;
}

void IceDogRendering::DirectionLightData::UpdateShadowTexture(PlatformDependenceRenderResource pdrr)
{
	r_shadowTexture2d.Release();
	r_shadowMapRTV.Release();
	r_shadowMapSRV.Release();
	r_depthStencilTexture2d.Release();
	r_depthStencilView.Release();

	r_shadowTexture2d = PIDTexture2D();
	r_shadowMapRTV = PIDRenderTargetView();
	r_shadowMapSRV = PIDShaderResourceView();

	r_depthStencilTexture2d = PIDTexture2D();
	r_depthStencilView = PIDDepthStencilView();

	ID3D11Texture2D* tx;
	ID3D11RenderTargetView* rt;
	ID3D11ShaderResourceView* sr;
	c_lightTextureDirty = !CreateRenderTargetTexture2D(pdrr,c_shadowMapSize, c_shadowMapSize, tx, rt, sr, DXGI_FORMAT_R16_UNORM);
	r_shadowTexture2d.SetTexture(tx);
	r_shadowMapRTV.SetRenderTargetView(rt);
	r_shadowMapSRV.SetResourceView(sr);

	ID3D11Texture2D* dtx;
	ID3D11DepthStencilView* dstv;

	// create the depth stencil view
	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width = c_shadowMapSize;
	dsd.Height = c_shadowMapSize;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count = 1;
	dsd.SampleDesc.Quality = 0;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;

	// create the depth stencil buffer
	if (ISFAILED(pdrr.r_device->CreateTexture2D(&dsd, 0, &dtx)))
	{
		return;
	}
	// create the depth stencil render target view
	if (ISFAILED(pdrr.r_device->CreateDepthStencilView(dtx, 0, &dstv)))
	{
		return;
	}

	r_depthStencilTexture2d.SetTexture(dtx);
	r_depthStencilView.SetDepthStencilView(dstv);
}

bool IceDogRendering::DirectionLightData::IsDirty()
{
	return c_lightTextureDirty;
}

void IceDogRendering::DirectionLightData::CleanBuffer(PlatformDependenceRenderResource pdrr)
{
	pdrr.r_deviceContext->ClearDepthStencilView(r_depthStencilView.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	// clear other rt view
	pdrr.r_deviceContext->ClearRenderTargetView(r_shadowMapRTV.GetRenderTargetView(), IceDogRendering::Color::NONECOLOR);
}

bool IceDogRendering::DirectionLightData::CreateRenderTargetTexture2D(PlatformDependenceRenderResource pdrr,UINT width, UINT height, ID3D11Texture2D*& texture, ID3D11RenderTargetView*& rt, ID3D11ShaderResourceView*& sr, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width = width;
	dsd.Height = height;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = format;
	dsd.SampleDesc.Count = 1;
	dsd.SampleDesc.Quality = 0;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;

	// create the texture 2d
	if (ISFAILED(pdrr.r_device->CreateTexture2D(&dsd, 0, &texture)))
	{
		return false;
	}

	// create the render target view
	if (ISFAILED(pdrr.r_device->CreateRenderTargetView(texture, 0, &rt)))
	{
		return false;
	}

	// create the shader resource view
	if (ISFAILED(pdrr.r_device->CreateShaderResourceView(texture, 0, &sr)))
	{
		return false;
	}
	return true;
}
