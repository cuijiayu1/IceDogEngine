#include "Sphere.h"
#include "../Engine/Engine.h"
#include "../Engine/Gameplay/Components/StaticMeshComponent.h"
#include "../Resources/Geometry/GeometryGenerator.h"
#include "../Utils/Noise/OtherNoise.h"

Sphere::Sphere()
{
	using IceDogUtils::float3;
	using IceDogUtils::float4;
	// test code
	IceDogGameplay::StaticMeshComponent* vc = new IceDogGameplay::StaticMeshComponent(this);
	std::shared_ptr<IceDogRendering::MeshData> rd = std::make_shared<IceDogRendering::MeshData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateSphere(0.05, 5, 5,rd);
	//IceDogResources::IceDogGeometry::GeometryGenerator::CreateTeapot(10, 2, false, rd);

	vc->SetStaticMesh(rd);
	SetActorScale(IceDogUtils::float3(1, 1, 1));
	SetEnable();
}

Sphere::~Sphere()
{

}
