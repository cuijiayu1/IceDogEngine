#include "MaterialData.h"



IceDogRendering::MaterialData::MaterialData():c_loaded(false),c_DifNorParEmi(0,0,0,0)
{
}


IceDogRendering::MaterialData::~MaterialData()
{
	ReleaseCOM(r_diffuseSRV);
}

void IceDogRendering::MaterialData::SetDiffuseMap(std::wstring url)
{
	c_diffuseMap_url = url;
	c_loaded = false;
	c_DifNorParEmi.x = 1;
}

void IceDogRendering::MaterialData::SetNormalMap(std::wstring url)
{
	c_normalMap_url = url;
	c_loaded = false;
	c_DifNorParEmi.y = 1;
}

void IceDogRendering::MaterialData::SetParallaxMap(std::wstring url, float scale, float layers, float quility)
{
	c_parallaxMap_url = url;
	c_loaded = false;
	c_DifNorParEmi.z = 1;
	c_parallaxCfg.x = scale;
	c_parallaxCfg.y = layers;
	c_parallaxCfg.z = abs(1 - quility) < 0.01 ? 1 : 0;
}

bool IceDogRendering::MaterialData::LoadMaterial(IceDogRendering::PlatformDependenceRenderResource prr)
{
	c_loaded = true;
	if (c_DifNorParEmi.x == 1)
	{
		ID3D11Resource* texResource_0 = nullptr;
		bool result = !ISFAILED(DirectX::CreateDDSTextureFromFile(prr.r_device, c_diffuseMap_url.c_str(), &texResource_0, &r_diffuseSRV));
		ReleaseCOM(texResource_0); // view saves reference
		c_loaded = c_loaded&&result;
	}
	if (c_DifNorParEmi.y == 1)
	{
		ID3D11Resource* texResource_1 = nullptr;
		bool result = !ISFAILED(DirectX::CreateDDSTextureFromFile(prr.r_device, c_normalMap_url.c_str(), &texResource_1, &r_normalSRV));
		ReleaseCOM(texResource_1);
		c_loaded = c_loaded&&result;
	}
	if (c_DifNorParEmi.z == 1)
	{
		ID3D11Resource* texResource_1 = nullptr;
		bool result = !ISFAILED(DirectX::CreateDDSTextureFromFile(prr.r_device, c_parallaxMap_url.c_str(), &texResource_1, &r_parallaxSRV));
		ReleaseCOM(texResource_1);
		c_loaded = c_loaded&&result;
	}
	
	return c_loaded;
}

ID3D11ShaderResourceView* IceDogRendering::MaterialData::GetDiffuseSRV()
{
	if (!c_loaded) { return nullptr; }
	return r_diffuseSRV;
}

ID3D11ShaderResourceView* IceDogRendering::MaterialData::GetNormalSRV()
{
	if (!c_loaded) { return nullptr; }
	return r_normalSRV;
}

ID3D11ShaderResourceView* IceDogRendering::MaterialData::GetParallaxSRV()
{
	if (!c_loaded) { return nullptr; }
	return r_parallaxSRV;
}

IceDogUtils::float4 IceDogRendering::MaterialData::GetParallaxCfg()
{
	return c_parallaxCfg;
}

IceDogRendering::float4 IceDogRendering::MaterialData::GetTextureEnableDesc()
{
	return c_DifNorParEmi;
}
