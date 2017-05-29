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
	// compile the shader
	std::cout << "Compile Shader:" << c_url << std::endl;
	std::string compile_cmd;
	std::string compile_mode = "";
	std::string compile_type = "";
	std::string compile_tag = c_url;
	compile_tag = compile_tag.replace(compile_tag.find(".hlsl"), std::string(".hlsl").size(), ".cso");
#ifdef _WIN32
	compile_cmd = x86_compiler+" ";
#endif
#ifdef _WIN64
	compile_cmd = x64_compiler;
#endif 
#ifdef _DEBUG
	compile_mode = "/Od /Zi ";
#endif
	std::cout << "Using Compiler:" << compile_cmd << std::endl;
	
	switch (c_shaderType)
	{
	case IceDogRendering::ShaderType::PS:
		compile_type = "ps_5_0 ";
		break;
	case IceDogRendering::ShaderType::VS:
		compile_type = "vs_5_0 ";
		break;
	case IceDogRendering::ShaderType::CS:
		compile_type = "cs_5_0 ";
		break;
	case IceDogRendering::ShaderType::GS:
		compile_type = "gs_5_0 ";
		break;
	}

	compile_cmd += compile_mode;
	compile_cmd += "/T ";
	compile_cmd += compile_type;
	compile_cmd += "/Fo ";
	compile_cmd += compile_tag + " ";
	compile_cmd += c_url;

	if (system(compile_cmd.c_str()))
	{
		std::cout << "Compile Failed!" << std::endl;
		return;
	}
	std::cout << std::endl;
}
