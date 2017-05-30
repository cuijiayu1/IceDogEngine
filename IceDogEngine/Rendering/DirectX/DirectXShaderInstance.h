#pragma once
#include "../ShaderInstance.h"
#include "../PlatformDependenceRenderResource.h"

namespace IceDogRendering
{
	class DirectXShaderInstance : public ShaderInstance
	{
	public:
		DirectXShaderInstance(std::string url, ShaderType st);
		~DirectXShaderInstance();

		/* call this to release the resource */
		void Close() override;

		/* initialize the shader and create shader */
		void Init(PlatformDependenceRenderResource, std::string x86_compiler, std::string x64_compiler) override;

		/* get the raw shader ptr */
		void* GetRawShaderPtr() override;

	private:
		// the ptr to the device shader
		ID3D11DeviceChild* r_deviceShaderPtr;
		// the platform dependence render resource
		PlatformDependenceRenderResource r_pdrr;
	};
}

