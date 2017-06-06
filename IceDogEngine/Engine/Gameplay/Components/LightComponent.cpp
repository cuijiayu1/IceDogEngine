#include "LightComponent.h"

IceDogGameplay::LightComponent::LightComponent(class Actor* owner) :Component::Component(owner)
{

}

IceDogGameplay::LightComponent::~LightComponent()
{

}

void IceDogGameplay::LightComponent::Close()
{
	r_holdLight->Close();
}

void IceDogGameplay::LightComponent::SetIntensity(const float intensity)
{
	r_holdLight->SetIntensity(intensity);
}
