#include "VoxelComponent.h"
#include "../../Engine.h"

IceDogGameplay::VoxelComponent::VoxelComponent(class Actor* owner):Component::Component(owner)
{

}

IceDogGameplay::VoxelComponent::~VoxelComponent()
{

}

void IceDogGameplay::VoxelComponent::Close()
{
	r_renderData->Close();
	Component::Close();
}

void IceDogGameplay::VoxelComponent::SetVoxelData(std::shared_ptr<IceDogRendering::VoxelData> rd)
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

void IceDogGameplay::VoxelComponent::SetEnable()
{
	Component::SetEnable();
	IceDogEngine::Engine::GetEngine()->RegistRenderData(r_renderData, IceDogRendering::RenderPipeType::Scene);
}

void IceDogGameplay::VoxelComponent::SetDisable()
{
	Component::SetDisable();
	IceDogEngine::Engine::GetEngine()->UnRegistRenderData(r_renderData, IceDogRendering::RenderPipeType::Scene);
}

void IceDogGameplay::VoxelComponent::Update()
{
	IceDogRendering::float4x4& worldMat = IceDogRendering::float4x4::FromSRT(c_owner->GetActorScale(), c_owner->GetActorRotation(), c_owner->GetActorLocation());
	r_renderData->SetWorldMatrix(worldMat);
}
