#pragma once
#include "../Utils/Common/UtilBasic.h"
#include "../Utils/Common/DDSTextureLoader.h"
#include "PlatformDependenceRenderResource.h"

namespace IceDogRendering
{
	class CubeMapSource
	{
	public:
		CubeMapSource();
		~CubeMapSource();

		/* load the cube map from a DDS file */
		void LoadFromFile(std::wstring url, IceDogRendering::PlatformDependenceRenderResource prr);

		/* get whether the shader resource is valid or not */
		bool IsDirty();

		/* get resource view */
		PIDShaderResourceView GetCubeMapSRV();

	private:
		// indicate whether the shader resource is valid or not
		bool c_isDirty;

		// the cube map shader resource view
		PIDShaderResourceView r_cubemapSRV;
	};
}

