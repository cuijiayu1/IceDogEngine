#pragma once

#include "../Utils/Common/UtilBasic.h"
#include "../Utils/Common/DDSTextureLoader.h"
#include "PlatformDependenceRenderResource.h"
#include "PDRenderingDef.h"

namespace IceDogRendering
{
	class MaterialData
	{
	public:
		MaterialData();
		~MaterialData();

		/* set the diffuse map location */
		void SetDiffuseMap(std::wstring url);

		/* load the material to the memory and create the shader resource view */
		bool LoadMaterial(IceDogRendering::PlatformDependenceRenderResource prr);

		/* get the diffuse shader resource view for rendering */
		ID3D11ShaderResourceView* GetDiffuseSRV();

	private:
		// the location of diffuse map
		std::wstring c_diffuseMap_url;

		// the shader resource view of diffuse map
		ID3D11ShaderResourceView* r_diffuseSRV;

		// the flag inidicated the material has loaded;
		bool c_loaded;
	};
}

