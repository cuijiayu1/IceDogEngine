#include "Plane.h"
#include "../Engine/Engine.h"
#include "../Engine/Gameplay/Components/StaticMeshComponent.h"
#include "../Resources/Geometry/GeometryGenerator.h"

using namespace IceDogGameplay;


Plane::Plane()
{
	// test code
	StaticMeshComponent* st = new StaticMeshComponent(this);
	std::shared_ptr<IceDogRendering::MeshData> rd = std::make_shared<IceDogRendering::MeshData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateGrid(2, 2, 2, 2, rd);
	st->SetStaticMesh(rd);
	SetEnable();
}


Plane::~Plane()
{
}
