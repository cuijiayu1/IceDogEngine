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
