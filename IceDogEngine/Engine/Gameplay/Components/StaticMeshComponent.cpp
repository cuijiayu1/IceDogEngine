#include "StaticMeshComponent.h"
#include "../../Engine.h"

using namespace IceDogGameplay;

StaticMeshComponent::~StaticMeshComponent()
{
}

IceDogGameplay::StaticMeshComponent::StaticMeshComponent(class Actor* owner):Component::Component(owner)
{
}

void IceDogGameplay::StaticMeshComponent::SetStaticMesh(std::shared_ptr<IceDogRendering::MeshData> rd)
{
	if (rd == nullptr)
	{
		//unregister the data
		r_renderData = nullptr;
	}
	else
	{
		r_renderData = rd;
	}
}

void IceDogGameplay::StaticMeshComponent::SetEnable()
{
	Component::SetEnable();
	IceDogEngine::Engine::GetEngine()->RegistRenderData(r_renderData, IceDogRendering::RenderPipeType::Scene);
}

void IceDogGameplay::StaticMeshComponent::SetDisable()
{
	Component::SetDisable();
	IceDogEngine::Engine::GetEngine()->UnRegistRenderData(r_renderData,IceDogRendering::RenderPipeType::Scene);
}

void IceDogGameplay::StaticMeshComponent::Update()
{
	IceDogRendering::float4x4& worldMat = IceDogRendering::float4x4::FromSRT(c_owner->GetActorScale(), c_owner->GetActorRotationRad(), c_owner->GetActorLocation());
	r_renderData->SetWorldMatrix(worldMat);
}
