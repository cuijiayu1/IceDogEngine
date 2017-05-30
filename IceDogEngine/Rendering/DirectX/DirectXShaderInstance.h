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
		void UpdateBuffer(ContinuousMode cm) override;

	private:
		/* create the buffer */
		void CreateBuffer(ContinuousMode cm);

		/* update the constant buffer */
		void UpdateCB(ContinuousMode cm);

	private:
		// the ptr to the device shader
		ID3D11DeviceChild* r_deviceShaderPtr;
		// the constant buffer perFrame
		ID3D11Buffer* r_cbPerFrame;
		// the constant buffer perObject;
		ID3D11Buffer* r_cbPerObject;
	};
}

