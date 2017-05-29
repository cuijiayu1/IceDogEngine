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

		/* initialize the shader and create shader */
		void Init(PlatformDependenceRenderResource, std::string x86_compiler, std::string x64_compiler) override;
	};
}

