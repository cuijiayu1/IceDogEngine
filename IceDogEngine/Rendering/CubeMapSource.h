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

		/* call close to release the resource */
		void Close();

		/* get resource view */
		PIDShaderResourceView GetCubeMapSRV();

		/* reset the shader resource view */
		void ResetCubeMapSRV(PIDShaderResourceView pidSRV, int cubemapSize);

		/* get the size of cube map */
		int GetCubemapSize();

	private:
		// indicate whether the shader resource is valid or not
		bool c_isDirty;

		// the cube map shader resource view
		PIDShaderResourceView r_cubemapSRV;

		int c_cubemapSize;
	};
}

