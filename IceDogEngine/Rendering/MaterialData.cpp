#include "MaterialData.h"



IceDogRendering::MaterialData::MaterialData():c_loaded(false)
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
}

bool IceDogRendering::MaterialData::LoadMaterial(IceDogRendering::PlatformDependenceRenderResource prr)
{
	ID3D11Resource* texResource = nullptr;
	bool result = !ISFAILED(DirectX::CreateDDSTextureFromFile(prr.r_device, c_diffuseMap_url.c_str(), &texResource, &r_diffuseSRV));
	ReleaseCOM(texResource); // view saves reference
	if (result) { c_loaded = true; }
	return result;
}

ID3D11ShaderResourceView* IceDogRendering::MaterialData::GetDiffuseSRV()
{
	if (!c_loaded) { return nullptr; }
	return r_diffuseSRV;
}
