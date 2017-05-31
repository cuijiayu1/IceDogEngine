#include "ShaderInstance.h"
#include "PlatformDependenceRenderResource.h"

IceDogRendering::ShaderInstance::ShaderInstance(std::string url, ShaderType st)
{
	c_url = url;
	c_shaderType = st;
	c_bufferSize[0] = 0;
	c_bufferSize[1] = 0;
}

IceDogRendering::ShaderInstance::~ShaderInstance()
{

}

void IceDogRendering::ShaderInstance::Close()
{

}

void IceDogRendering::ShaderInstance::Init(PlatformDependenceRenderResource pdrr, std::string x86_compiler, std::string x64_compiler)
{
	r_pdrr = pdrr;
}

void IceDogRendering::ShaderInstance::AddViriable(std::string alias, size_t viriableSize, int cm)
{
	r_orderSize[cm].push_back(viriableSize);
	r_orderAlias[cm].push_back(alias);
	c_bufferSize[cm] = CalculateTheBufferSize(cm);
}

size_t IceDogRendering::ShaderInstance::CalculateTheBufferSize(int cm)
{
	size_t tmp = 0;
	for (auto &i:r_orderSize[cm])
	{
		tmp += i;
	}
	tmp = ceil((float)(tmp) / 16) * 16;
	if (tmp < 128) { tmp = 128; }
	return tmp;
}
