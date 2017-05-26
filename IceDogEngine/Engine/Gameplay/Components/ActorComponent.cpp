#include "ActorComponent.h"
#include "../../Engine.h"

using namespace IceDogGameplay;

ActorComponent::ActorComponent(class Actor* owner):Component(owner)
{
	r_logicData = std::make_shared<IceDogLogic::LogicData>();
	r_logicData->AddTickFunction(std::bind(&IceDogGameplay::ActorComponent::TickBorder, this, std::placeholders::_1));
}

ActorComponent::~ActorComponent()
{
}

void IceDogGameplay::ActorComponent::TickBorder(float deltaTime)
{
	Tick(deltaTime);
}

void IceDogGameplay::ActorComponent::Close()
{
	r_logicData->Close();
	Component::Close();
}

void IceDogGameplay::ActorComponent::SetEnable()
{
	Component::SetEnable();
	IceDogEngine::Engine::GetEngine()->RegistLogicData(r_logicData);
}

void IceDogGameplay::ActorComponent::SetDisable()
{
	Component::SetDisable();
	IceDogEngine::Engine::GetEngine()->UnRegistLogicData(r_logicData);
}

void IceDogGameplay::ActorComponent::RegistOwningActorTick(std::function<void(float)> tickFunc)
{
	r_logicData->AddTickFunction(tickFunc);
}