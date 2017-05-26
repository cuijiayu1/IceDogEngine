#include "FakePhysicBall.h"
#include "../Engine/Engine.h"
#include "../Engine/Gameplay/Components/StaticMeshComponent.h"
#include "../Resources/Geometry/GeometryGenerator.h"
#include "../Utils/Noise/OtherNoise.h"


using IceDogUtils::float3;
using IceDogUtils::float4;

FakePhysicBall::FakePhysicBall()
{
	// test code
	IceDogGameplay::StaticMeshComponent* vc = new IceDogGameplay::StaticMeshComponent(this);
	std::shared_ptr<IceDogRendering::MeshData> rd = std::make_shared<IceDogRendering::MeshData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateSphere(0.05, 20, 20, rd);
	//IceDogResources::IceDogGeometry::GeometryGenerator::CreateTeapot(10, 2, false, rd);

	_currentSpeed = float3(0.2, 0, 0);

	vc->SetStaticMesh(rd);
	SetActorScale(IceDogUtils::float3(1, 1, 1));
	SetEnable();
}


FakePhysicBall::~FakePhysicBall()
{
}


void FakePhysicBall::SetInitSpeed(IceDogUtils::float3 spd)
{
	_currentSpeed = spd;
}

void FakePhysicBall::PhyCheck(float deltaTime)
{
	float3 location = GetActorLocation();
	if (abs(location.x) > 1)
	{
		_currentSpeed = 0.9*float3(-_currentSpeed.x, _currentSpeed.y, _currentSpeed.z);
		SetActorLocation(float3(location.x /abs(location.x), location.y, location.z));
	}
	if (abs(location.y) > 1)
	{
		_currentSpeed = 0.9*float3(_currentSpeed.x, -_currentSpeed.y, _currentSpeed.z);
		SetActorLocation(float3(location.x , location.y / abs(location.y), location.z));
	}
	if (abs(location.z) > 1)
	{
		_currentSpeed = 0.9*float3(_currentSpeed.x, _currentSpeed.y, -_currentSpeed.z);
		SetActorLocation(float3(location.x, location.y , location.z / abs(location.z)));
	}
	_currentSpeed = _currentSpeed + float3(0, -3, 0)*deltaTime;
}

void FakePhysicBall::Tick(float deltaTime)
{
	PhyCheck(deltaTime);
	SetActorLocation(GetActorLocation() + _currentSpeed*deltaTime);
}
