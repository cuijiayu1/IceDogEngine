#include "DirectXShaderInstance.h"

using namespace IceDogRendering;

DirectXShaderInstance::DirectXShaderInstance(std::string url, ShaderType st):ShaderInstance(url, st)
{

}

DirectXShaderInstance::~DirectXShaderInstance()
{

}

void DirectXShaderInstance::Init(PlatformDependenceRenderResource pdrr, std::string x86_compiler, std::string x64_compiler)
{
	D3D_SHADER_MACRO shader_macro;
	switch (c_shaderType)
	{
	case IceDogRendering::ShaderType::PS:
		break;
	case IceDogRendering::ShaderType::VS:
		break;
	case IceDogRendering::ShaderType::CS:
		break;
	case IceDogRendering::ShaderType::GS:
		break;
	}
}
