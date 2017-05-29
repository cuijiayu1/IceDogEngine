#include "ShaderInstance.h"
#include "PlatformDependenceRenderResource.h"

IceDogRendering::ShaderInstance::ShaderInstance(std::string url, ShaderType st)
{
	c_url = url;
	c_shaderType = st;
}

IceDogRendering::ShaderInstance::~ShaderInstance()
{

}

void IceDogRendering::ShaderInstance::Close()
{

}

void IceDogRendering::ShaderInstance::Init(PlatformDependenceRenderResource pdrr, std::string x86_compiler, std::string x64_compiler)
{

}
