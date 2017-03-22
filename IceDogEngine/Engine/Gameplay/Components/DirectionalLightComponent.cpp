#include "DirectionalLightComponent.h"
#include "../../Engine.h"
#include "../../../Rendering/LightData/DirectionLightData.h"

IceDogGameplay::DirectionalLightComponent::DirectionalLightComponent(class Actor* owner):IceDogGameplay::LightComponent::LightComponent(owner)
{
	// construct the default light data
	r_holdLight = std::make_shared<IceDogRendering::DirectionLightData>();
}

IceDogGameplay::DirectionalLightComponent::~DirectionalLightComponent()
{

}

void IceDogGameplay::DirectionalLightComponent::SetEnable()
{
	Component::SetDisable();
	IceDogEngine::Engine::GetEngine()->RegisterLightData(r_holdLight, IceDogRendering::LightType::Directional);
}

void IceDogGameplay::DirectionalLightComponent::SetDisable()
{
	Component::SetDisable();
	IceDogEngine::Engine::GetEngine()->UnRegisterLightData(r_holdLight, IceDogRendering::LightType::Directional);
}

void IceDogGameplay::DirectionalLightComponent::SetLightDirection(IceDogUtils::float3 direction)
{
	std::dynamic_pointer_cast<IceDogRendering::DirectionLightData>(r_holdLight)->SetLightDirection(direction);
}

void IceDogGameplay::DirectionalLightComponent::Update()
{
	
}
