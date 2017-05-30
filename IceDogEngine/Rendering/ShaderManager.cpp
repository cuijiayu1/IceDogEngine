#include "ShaderManager.h"
#include "ShaderInstance.h"
#include "DirectX/DirectXShaderInstance.h"
#include "../Engine/Engine.h"

using namespace IceDogRendering;

void ShaderManager::Init(PlatformDependenceRenderResource pdrr)
{
	rapidxml::file<> materialDesc("Config/ShaderManager.cfg");
	rapidxml::xml_document<> doc;
	doc.parse<0>(materialDesc.data());

	r_pdrr = pdrr;

	auto rootNode = doc.first_node();
#ifdef __DIRECTX__
	rootNode = rootNode->first_node("DirectX");
#endif
	auto engine_shader = rootNode->first_node("engine_shader");

	IceDogEngine::Engine::GetEngine()->AmazingText(std::cout, "Begin Compile Shader");
	while (engine_shader->first_node("shader"))
	{
		CreateShaderFromNode(engine_shader->first_node("shader"), rootNode->first_node("compiler_x86")->value(), rootNode->first_node("compiler_x64")->value());
		engine_shader->remove_first_node();
	}
}

IceDogRendering::ShaderManager::ShaderManager()
{

}

IceDogRendering::ShaderManager::~ShaderManager()
{

}

void IceDogRendering::ShaderManager::Close()
{
	for (auto i:r_shaderInstances)
		i.second->Close();
	r_shaderInstances.clear();
}

ShaderInstance* IceDogRendering::ShaderManager::GetShaderByAlias(std::string alias)
{
	return r_shaderInstances[alias].get();
}

void ShaderManager::CreateShaderFromNode(rapidxml::xml_node<char>* node,std::string x86_compiler, std::string x64_compiler)
{
	std::string url = node->first_node("url")->value();
	std::string alias = node->first_node("alias")->value();
	std::string type_str = node->first_node("type")->value();
	ShaderType shaderType;
	switch (type_str[0])
	{
	case 'V':
		shaderType = ShaderType::VS;
		break;
	case 'P':
		shaderType = ShaderType::PS;
		break;
	case 'G':
		shaderType = ShaderType::GS;
		break;
	case 'C':
		shaderType = ShaderType::CS;
		break;
	}
#ifdef __DIRECTX__
	std::shared_ptr<ShaderInstance> si = std::make_shared<DirectXShaderInstance>(url, shaderType);
#endif
	si->Init(r_pdrr,x86_compiler,x64_compiler);
	r_shaderInstances[alias] = si;
}
