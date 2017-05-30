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
	r_pdrr = pdrr;
}

void IceDogRendering::ShaderInstance::AddViriable(std::string alias, size_t viriableSize, ContinuousMode cm)
{
	switch (cm)
	{
	case IceDogRendering::ContinuousMode::PerObject:
		r_aliasSizeMap_po[alias] = viriableSize;
		r_orderAlias_po.push_back(alias);
		break;
	case IceDogRendering::ContinuousMode::PerFrame:
		r_aliasSizeMap_pf[alias] = viriableSize;
		r_orderAlias_pf.push_back(alias);
		break;
	}
}
