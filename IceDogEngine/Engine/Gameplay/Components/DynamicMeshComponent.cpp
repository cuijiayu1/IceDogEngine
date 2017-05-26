#include "DynamicMeshComponent.h"

using namespace IceDogGameplay;

DynamicMeshComponent::DynamicMeshComponent(class Actor* owner):Component::Component(owner)
{
}


DynamicMeshComponent::~DynamicMeshComponent()
{
}

void IceDogGameplay::DynamicMeshComponent::Close()
{
	r_renderData->Close();
	Component::Close();
}
