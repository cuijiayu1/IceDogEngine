#include "StaticMeshComponent.h"
#include "../../Engine.h"

using namespace IceDogGameplay;

StaticMeshComponent::~StaticMeshComponent()
{
}

IceDogGameplay::StaticMeshComponent::StaticMeshComponent(class Actor* owner):Component::Component(owner),c_renderDataRegistered(false)
{
}

void IceDogGameplay::StaticMeshComponent::Close()
{
	c_renderDataRegistered = false;
	r_renderData->Close();
	Component::Close();
}

void IceDogGameplay::StaticMeshComponent::SetStaticMesh(std::shared_ptr<IceDogRendering::MeshData> rd)
{
	// if it`s the same, just return
	if (rd == r_renderData)
		return;
	// if is a nullptr, unregister the current
	if (c_renderDataRegistered)
	{
		IceDogEngine::Engine::GetEngine()->UnRegistRenderData(r_renderData, IceDogRendering::RenderPipeType::Scene);
		c_renderDataRegistered = false;
	}
	if (rd != nullptr)
	{
		IceDogEngine::Engine::GetEngine()->RegistRenderData(rd, IceDogRendering::RenderPipeType::Scene);
		c_renderDataRegistered = true;
	}	
	r_renderData = rd;
}

void IceDogGameplay::StaticMeshComponent::SetEnable()
{
	Component::SetEnable();
	if (r_renderData != nullptr && !c_renderDataRegistered)
	{
		IceDogEngine::Engine::GetEngine()->RegistRenderData(r_renderData, IceDogRendering::RenderPipeType::Scene);
		c_renderDataRegistered = true;
	}
}

void IceDogGameplay::StaticMeshComponent::SetDisable()
{
	Component::SetDisable();
	if (r_renderData != nullptr && c_renderDataRegistered)
	{
		IceDogEngine::Engine::GetEngine()->UnRegistRenderData(r_renderData, IceDogRendering::RenderPipeType::Scene);
		c_renderDataRegistered = false;
	}
}

void IceDogGameplay::StaticMeshComponent::Update()
{
	IceDogRendering::float4x4& worldMat = IceDogRendering::float4x4::FromSRT(c_owner->GetActorScale(), c_owner->GetActorRotationRad(), c_owner->GetActorLocation());
	r_renderData->SetWorldMatrix(worldMat);
}
