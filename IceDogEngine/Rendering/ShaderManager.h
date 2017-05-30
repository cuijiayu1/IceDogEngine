#pragma once
#include "../Utils/Common/UtilBasic.h"
#include "PlatformDependenceRenderResource.h"

namespace IceDogRendering
{
	class ShaderInstance;

	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();

	public:
		/* init with a shader list */
		void Init(PlatformDependenceRenderResource pdrr);

		/* call this function to release the resource */
		void Close();

		/* get shader by alias name */
		ShaderInstance* GetShaderByAlias(std::string alias);

	private:
		void CreateShaderFromNode(rapidxml::xml_node<char>* node, std::string x86_compiler, std::string x64_compiler);

	private:
		// the shader instance that this shader manager hold
		std::unordered_map<std::string, std::shared_ptr<ShaderInstance>> r_shaderInstances;
		// the pdrr
		PlatformDependenceRenderResource r_pdrr;
	};
}

