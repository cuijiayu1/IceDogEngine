#pragma once
#include "../ShaderInstance.h"

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

		/* update the buffer from current definition, if buffer has not created create it */
		void UpdateApplyBuffer(int cm) override;

		/* apply the shader to the pipeline */
		void ApplyShader() override;

		/* unload the shader from the pipeline */
		void UnloadShader() override;

		/* set the data */
		void SetViriable(const std::string& alias, void* ptr, const int cm) override;

		/* check has constant buffer */
		bool HasConstantBuffer() override;

		/* get buffer raw ptr */
		void* GetBufferRawPtr(int cm) override;

	private:
		/* create the buffer */
		void CreateBuffer(int cm);

		/* update the constant buffer */
		void UpdateCB(int cm);

		void* GetDataCope(int cm);

	private:
		// the ptr to the device shader
		ID3D11DeviceChild* r_deviceShaderPtr;
		// the constant buffer
		ID3D11Buffer* r_constantBuffer[2];
	};
}

