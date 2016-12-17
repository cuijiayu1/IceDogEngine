#include "MyActor.h"
#include "../Engine/Gameplay/Components/StaticMeshComponent.h"
#include "../Resources/Geometry/GeometryGenerator.h"

using namespace IceDogGameplay;

MyActor::MyActor()
{
	// test code
	StaticMeshComponent* st = new StaticMeshComponent();
	std::shared_ptr<IceDogRendering::RenderData> rd = std::make_shared<IceDogRendering::RenderData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateSphere(1, 20, 20, rd);

	st->SetStaticMesh(rd);
}


MyActor::~MyActor()
{
}

void MyActor::Tick(float deltaTime)
{
	
}
