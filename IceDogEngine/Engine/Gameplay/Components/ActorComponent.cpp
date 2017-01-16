#include "ActorComponent.h"
#include "../../Engine.h"

using namespace IceDogGameplay;

ActorComponent::ActorComponent(class Actor* owner):Component::Component(owner)
{
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
	Component::SetEnable();
	IceDogEngine::Engine::GetEngine()->RegistLogicData(this);
}

void IceDogGameplay::ActorComponent::SetDisable()
{
	Component::SetDisable();
	IceDogEngine::Engine::GetEngine()->UnRegistLogicData(this);
}

void IceDogGameplay::ActorComponent::RegistOwningActorTick(std::function<void(float)> tickFunc)
{
	c_actorTickFunc = tickFunc;
}
