#include "Actor.h"
#include "../Engine.h"

using namespace IceDogGameplay;
using namespace IceDogEngine;

Actor::Actor()
{
	// enable the default actor component 
	r_defaultActorComponent.RegistOwningActorTick(std::bind(&IceDogGameplay::Actor::Tick, this, std::placeholders::_1));
	r_defaultActorComponent.SetEnable();
	// regist the actor to engine
	Engine::GetEngine()->RegistActor(std::shared_ptr<Actor>(this));
}

void IceDogGameplay::Actor::Tick(float deltaTime)
{
	
}

Actor::~Actor()
{
	r_defaultActorComponent.SetDisable();
}
