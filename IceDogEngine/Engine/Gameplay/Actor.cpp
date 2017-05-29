#include "Actor.h"
#include "../Engine.h"

using namespace IceDogGameplay;
using namespace IceDogEngine;

Actor::Actor()
{
	r_defaultActorComponent = new ActorComponent(this);
	r_defaultEventComponent = new EventComponent(this);
	r_defaultActorCoordinate = std::make_unique<IceDogCore::VectorSpace>();
	// enable the default actor component 
	r_defaultActorComponent->RegistOwningActorTick(std::bind(&IceDogGameplay::Actor::LayerTick, this, std::placeholders::_1));
}

void IceDogGameplay::Actor::SetEnable()
{
	r_defaultActorComponent->SetEnable();
	r_defaultEventComponent->SetEnable();
	// enable all component
	for (auto& i:r_holdComponents)
	{
		i->SetEnable();
	}
}

void IceDogGameplay::Actor::SetDisable()
{
	r_defaultActorComponent->SetDisable();
	r_defaultEventComponent->SetDisable();
	// disable all component
	for (auto& i:r_holdComponents)
	{
		i->SetDisable();
	}
}

void IceDogGameplay::Actor::Destroy()
{
	SetDisable();
	Close();
	IceDogEngine::Engine::GetEngine()->RemoveActor(this);
}

void IceDogGameplay::Actor::RegistComponentToActor(std::shared_ptr<Component> comp)
{
	r_holdComponents.push_back(comp);
}

void IceDogGameplay::Actor::LayerTick(float deltaTime)
{
	// before the tick will have a before tick for pr process
	BeforTick(deltaTime);

	Tick(deltaTime);
	// after tick will have another tick
	AfterTick(deltaTime);
}

void IceDogGameplay::Actor::BeforTick(float deltaTime)
{

}

void IceDogGameplay::Actor::AfterTick(float deltaTime)
{
	// do space update here
	if (r_defaultActorCoordinate->IsCoordinateDirty())
	{
		for (auto& i:r_holdComponents)
		{
			i->Update();
		}
		r_defaultActorCoordinate->MarkCoordinateClean();
	}
}

void IceDogGameplay::Actor::MoveForward(float distance)
{
	r_defaultActorCoordinate->MoveForward(distance);
}

void IceDogGameplay::Actor::MoveRight(double distance)
{
	r_defaultActorCoordinate->MoveRight(distance);
}

IceDogGameplay::float3 IceDogGameplay::Actor::GetActorLocation()
{
	return r_defaultActorCoordinate->GetLocation();
}

void IceDogGameplay::Actor::SetActorLocation(const float3& newLocation)
{
	r_defaultActorCoordinate->SetLocation(newLocation);
}

IceDogGameplay::float3 IceDogGameplay::Actor::GetActorScale()
{
	return r_defaultActorCoordinate->GetScale();
}

IceDogGameplay::float3 IceDogGameplay::Actor::GetActorLookAt()
{
	return r_defaultActorCoordinate->GetLookAtLocation();
}

IceDogGameplay::float3 IceDogGameplay::Actor::GetActorUpVector()
{
	return r_defaultActorCoordinate->GetUp();
}

void IceDogGameplay::Actor::SetActorScale(const float3& newScale)
{
	r_defaultActorCoordinate->SetScale(newScale);
}

IceDogGameplay::float3 IceDogGameplay::Actor::GetActorRotation()
{
	return r_defaultActorCoordinate->GetRotation();
}

IceDogGameplay::float3 IceDogGameplay::Actor::GetActorRotationRad()
{
	return r_defaultActorCoordinate->GetRotationRad();
}

void IceDogGameplay::Actor::SetActorRotation(const float3& newRotate)
{
	r_defaultActorCoordinate->SetRotation(newRotate);
}

void IceDogGameplay::Actor::Close()
{
	for (auto i:r_holdComponents)
		i->Close();
	r_holdComponents.clear();
	std::cout << "Actor Closed" << std::endl;
}

Actor::~Actor()
{
	std::cout << "Actor Released" << std::endl;
}
