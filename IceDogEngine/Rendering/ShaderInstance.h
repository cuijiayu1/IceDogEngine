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

#define  ContinuousMode_PerFrame 0
#define  ContinuousMode_PerObject 1

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
		virtual void AddViriable(std::string alias, size_t viriableSize, int cm);

		/* update the buffer from current definition, if buffer has not created create it */
		virtual void UpdateApplyBuffer(int cm) = 0;

		/* apply the shader to the pipeline */
		virtual void ApplyShader() = 0;

		/* unload the shader to the pipeline */
		virtual void UnloadShader() = 0;

		/* set the data */
		virtual void SetViriable(const std::string& alias, void* ptr, const int cm) = 0;

		/* check has constant buffer */
		virtual bool HasConstantBuffer() = 0;

		/* get buffer raw ptr */
		virtual void* GetBufferRawPtr(int cm) = 0;

	private:
		/* calculate the buffer size */
		size_t CalculateTheBufferSize(int cm);

	protected:
		// the platform dependence resource
		PlatformDependenceRenderResource r_pdrr;
		// the shader file url
		std::string c_url;
		// the shader type
		ShaderType c_shaderType;
		// the shader code byte data
		std::vector<char> r_shaderCode;
		// the table of alias and ptr
		std::unordered_map<std::string, void*> r_aliasPtrMap[2];
		// the order vector
		std::vector<std::string> r_orderAlias[2];
		// the order size
		std::vector<size_t> r_orderSize[2];
		// this value will update every add variable.
		size_t c_bufferSize[2];
	};
}

