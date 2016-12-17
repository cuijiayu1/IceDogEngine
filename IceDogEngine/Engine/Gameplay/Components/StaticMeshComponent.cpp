#include "StaticMeshComponent.h"
#include "../../Engine.h"

using namespace IceDogGameplay;

StaticMeshComponent::StaticMeshComponent()
{
}


StaticMeshComponent::~StaticMeshComponent()
{
}

void IceDogGameplay::StaticMeshComponent::SetStaticMesh(std::shared_ptr<IceDogRendering::RenderData> rd)
{
	if (rd == nullptr)
	{
		//unregist the data
		r_renderData = nullptr;
	}
	else
	{
		r_renderData = rd;
		IceDogEngine::Engine::GetEngine()->RegistRenderData(rd,IceDogRendering::RenderPipeType::Scene);
	}
}
