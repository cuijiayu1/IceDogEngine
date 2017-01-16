#include "Actor.h"
#include "../Engine.h"

using namespace IceDogGameplay;
using namespace IceDogEngine;

Actor::Actor():r_defaultActorComponent(this)
{
	// enable the default actor component 
	r_defaultActorComponent.RegistOwningActorTick(std::bind(&IceDogGameplay::Actor::Tick, this, std::placeholders::_1));
}

void IceDogGameplay::Actor::Tick(float deltaTime)
{
	
}

void IceDogGameplay::Actor::SetEnable()
{
	r_defaultActorComponent.SetEnable();
	// enable all component
	for (auto& i:r_holdComponents)
	{
		i->SetEnable();
	}
	// regist the actor to engine
	Engine::GetEngine()->RegistActor(this);
}

void IceDogGameplay::Actor::SetDisable()
{
	r_defaultActorComponent.SetDisable();
	// disable all component
	for (auto& i:r_holdComponents)
	{
		i->SetDisable();
	}
	// unregist the actor to engine
	Engine::GetEngine()->UnRegistActor(this);
}

void IceDogGameplay::Actor::RegistComponentToActor(std::shared_ptr<Component> comp)
{
	r_holdComponents.push_back(comp);
}

Actor::~Actor()
{
	r_defaultActorComponent.SetDisable();
}
