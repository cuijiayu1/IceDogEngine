#pragma once
#include "../Utils/Common/UtilBasic.h"

namespace IceDogRendering
{
	enum class ShaderType {
		PS,
		VS,
		CS,
		GS
	};

	struct PlatformDependenceRenderResource;

	class ShaderInstance
	{
	public:
		ShaderInstance(std::string url, ShaderType st);
		virtual ~ShaderInstance();

		/* call this function to release the resource */
		virtual void Close();

		/* initialize this shader */
		virtual void Init(PlatformDependenceRenderResource, std::string x86_compiler, std::string x64_compiler);

	protected:
		// the shader file url
		std::string c_url;
		// the shader type
		ShaderType c_shaderType;
	};
}
