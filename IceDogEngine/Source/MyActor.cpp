#include "MyActor.h"
#include "../Engine/Gameplay/Components/StaticMeshComponent.h"
#include "../Resources/Geometry/GeometryGenerator.h"

using namespace IceDogGameplay;

MyActor::MyActor():sourceLocation(1,1,1)
{
	// test code
	StaticMeshComponent* st = new StaticMeshComponent(this);
	std::shared_ptr<IceDogRendering::RenderData> rd = std::make_shared<IceDogRendering::RenderData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateSphere(1, 20, 20, rd);

	r_defaultEventComponent.BindOnLeftDown(std::bind(&MyActor::OnLeftClick,this,std::placeholders::_1,std::placeholders::_2));

	st->SetStaticMesh(rd);
	SetEnable();
	//SetDisable();
}


MyActor::~MyActor()
{
}

int MyActor::OnLeftClick(float x, float y)
{
	return 0;
}

void MyActor::Tick(float deltaTime)
{
	sourceLocation = sourceLocation + IceDogCore::float3(0, 0.0001, 0);
	SetActorScale(sourceLocation);
}
