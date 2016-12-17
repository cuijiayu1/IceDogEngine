#include "ActorComponent.h"
#include "../../Engine.h"

using namespace IceDogGameplay;

ActorComponent::ActorComponent()
{
	SetEnable();
}


ActorComponent::~ActorComponent()
{
}

void IceDogGameplay::ActorComponent::Tick(float deltaTime)
{
	if (c_actorTickFunc)
	{
		c_actorTickFunc(deltaTime);
	}
}

void IceDogGameplay::ActorComponent::SetEnable()
{
	c_componentEnable = true;
	IceDogEngine::Engine::GetEngine()->RegistLogicData(this);
}

void IceDogGameplay::ActorComponent::SetDisable()
{
	c_componentEnable = false;
	IceDogEngine::Engine::GetEngine()->UnRegistLogicData(this);
}

void IceDogGameplay::ActorComponent::RegistOwningActorTick(std::function<void(float)> tickFunc)
{
	c_actorTickFunc = tickFunc;
}
