#pragma once
#include "../Engine/Gameplay/Actor.h"

namespace IceDogGameplay
{
	class StaticMeshComponent;
}

namespace IceDogRendering
{
	class MeshData;
}

class FakePhysicBall :public IceDogGameplay::Actor
{

public:
	FakePhysicBall();
	~FakePhysicBall();

	void SetInitSpeed(IceDogUtils::float3);

	int KeyDown(int id);

private:
	IceDogUtils::float3 _currentSpeed;

	IceDogGameplay::StaticMeshComponent* vc;

	std::shared_ptr<IceDogRendering::MeshData> rd;

	std::shared_ptr<IceDogRendering::MeshData> tp;

	void PhyCheck(float deltaTime);

protected:
	void Tick(float deltaTime) override;
};

