#pragma once
#include "../Utils/Common/UtilBasic.h"
#include "PlatformDependenceRenderResource.h"


namespace IceDogRendering
{
	enum class ShaderType {
		PS,
		VS,
		CS,
		GS
	};

	enum class ContinuousMode
	{
		PerObject,
		PerFrame
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

		/* get the raw shader ptr */
		virtual void* GetRawShaderPtr() { return NULL; };

		/* get the shader code size */
		virtual size_t GetShaderCodeSize() { return r_shaderCode.size(); }

		/* get the shader code ptr */
		virtual char* GetShaderCodePtr() { return r_shaderCode.data(); }

		/* add one variable */
		virtual void AddViriable(std::string alias, size_t viriableSize, ContinuousMode cm);

		/* update the buffer from current definition, if buffer has not created create it */
		virtual void UpdateBuffer(ContinuousMode cm) = 0;

		/* update the buffer with new data */
		//virtual void UpdateBuffer(ContinuousMode cm) = 0;

	protected:
		// the platform dependence resource
		PlatformDependenceRenderResource r_pdrr;
		// the shader file url
		std::string c_url;
		// the shader type
		ShaderType c_shaderType;
		// the shader code byte data
		std::vector<char> r_shaderCode;
		// the table of alias and size
		std::unordered_map<std::string, size_t> r_aliasSizeMap_po;
		std::unordered_map<std::string, size_t> r_aliasSizeMap_pf;
		// the table of alias and ptr
		std::unordered_map<std::string, void*> r_aliasPtrMap_po;
		std::unordered_map<std::string, void*> r_aliasPtrMap_pf;
		// the order vector
		std::vector<std::string> r_orderAlias_po;
		std::vector<std::string> r_orderAlias_pf;
	};
}

