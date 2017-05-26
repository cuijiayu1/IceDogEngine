#pragma once
#include "../Engine/Gameplay/Actor.h"

class FakePhysicBall :public IceDogGameplay::Actor
{
public:
	FakePhysicBall();
	~FakePhysicBall();

	void SetInitSpeed(IceDogUtils::float3);

private:
	IceDogUtils::float3 _currentSpeed;

	void PhyCheck(float deltaTime);

protected:
	void Tick(float deltaTime) override;
};

