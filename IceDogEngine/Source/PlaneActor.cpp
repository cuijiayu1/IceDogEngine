#include "PlaneActor.h"
#include "../Engine/Engine.h"
#include "../Engine/Gameplay/Components/StaticMeshComponent.h"
#include "../Resources/Geometry/GeometryGenerator.h"

using namespace IceDogGameplay;


PlaneActor::PlaneActor()
{
	// test code
	StaticMeshComponent* st = new StaticMeshComponent(this);
	std::shared_ptr<IceDogRendering::MeshData> rd = std::make_shared<IceDogRendering::MeshData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateGrid(2, 2, 2, 2, rd);

	//rd->SetMaterial(IceDogEngine::Engine::GetEngine()->LoadMaterialFromUrl("Source/Material/Mat_land.IDAssets"));

	st->SetStaticMesh(rd);
	SetEnable();
}


PlaneActor::~PlaneActor()
{
}
