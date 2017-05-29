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
	vc = new IceDogGameplay::StaticMeshComponent(this);
	rd = std::make_shared<IceDogRendering::MeshData>();
	tp = std::make_shared<IceDogRendering::MeshData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateSphere(0.05, 20, 20, rd);
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateTeapot(10, 0.2, false, tp);

	_currentSpeed = float3(0, 0, 0);

	vc->SetStaticMesh(rd);
	SetActorScale(IceDogUtils::float3(1, 1, 1));

	r_defaultEventComponent->BindOnKeyDown(std::bind(&FakePhysicBall::KeyDown, this, std::placeholders::_1));

	SetEnable();
}


FakePhysicBall::~FakePhysicBall()
{
}


void FakePhysicBall::SetInitSpeed(IceDogUtils::float3 spd)
{
	_currentSpeed = spd;
}

int FakePhysicBall::KeyDown(int id)
{
	switch (id)
	{
	case 18:
		Destroy();
		break;
	case 16:
		if (!testFlag)
			vc->SetStaticMesh(tp);
		else
			vc->SetStaticMesh(rd);
		testFlag = !testFlag;
		break;
	}
	return 0;
}

void FakePhysicBall::PhyCheck(float deltaTime)
{
	float3 location = GetActorLocation();
	if (abs(location.x) > 1)
	{
		_currentSpeed = 0.9*float3(-_currentSpeed.x, _currentSpeed.y, _currentSpeed.z);
		SetActorLocation(float3(location.x /abs(location.x), location.y, location.z));
	}
	else if (abs(location.y) > 1)
	{
		_currentSpeed = 0.9*float3(_currentSpeed.x, -_currentSpeed.y, _currentSpeed.z);
		SetActorLocation(float3(location.x , location.y / abs(location.y), location.z));
	}
	else if (abs(location.z) > 1)
	{
		_currentSpeed = 0.9*float3(_currentSpeed.x, _currentSpeed.y, -_currentSpeed.z);
		SetActorLocation(float3(location.x, location.y , location.z / abs(location.z)));
	}
	else {}
	_currentSpeed = _currentSpeed + float3(0, -3, 0)*deltaTime;
}

void FakePhysicBall::Tick(float deltaTime)
{
	PhyCheck(deltaTime);
	SetActorLocation(GetActorLocation() + _currentSpeed*deltaTime);
}
