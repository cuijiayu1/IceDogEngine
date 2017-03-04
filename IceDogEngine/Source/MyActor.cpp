#include "MyActor.h"
#include "../Engine/Engine.h"
#include "../Engine/Gameplay/Components/StaticMeshComponent.h"
#include "../Resources/Geometry/GeometryGenerator.h"
#include "../Utils/Voxel/MarchingCubeLT.h"

using namespace IceDogGameplay;

MyActor::MyActor() :sourceLocation(1, 1, 1)
{
	// test code
	StaticMeshComponent* st = new StaticMeshComponent(this);
	std::shared_ptr<IceDogRendering::RenderData> rd = std::make_shared<IceDogRendering::RenderData>();
	//IceDogResources::IceDogGeometry::GeometryGenerator::CreateTeapot(10, 2, false, rd);

	r_defaultEventComponent.BindOnLeftDown(std::bind(&MyActor::OnLeftClick, this, std::placeholders::_1, std::placeholders::_2));

	SetActorRotation(IceDogUtils::float3(0, 0, 0));

	std::vector<std::vector<std::vector<float>>> da(20);
	for (int i=0;i<20;i++)
	{
		da[i].resize(20);
	}
	for (int i=0;i<20;i++)
	{
		for (int j =0;j<20;j++)
		{
			da[i][j].resize(20);
		}
	}
	IceDogUtils::float3 target(10, 10, 10);
	for (int i=0;i<20;i++)
	{
		for (int j=0;j<20;j++)
		{
			for (int k=0;k<20;k++)
			{
				IceDogUtils::float3 f3 = IceDogUtils::float3(i, j, k) - target;
				if (float3length(f3) < 5) 
					da[i][j][k] = 1;
				else
					da[i][j][k] = 0;
			}
		}
	}

	int numTri= IceDogAlgorithm::PolygoniseArray(da, 0.3, rd);
	//rd->SetMaterial(IceDogEngine::Engine::GetEngine()->LoadMaterialFromUrl("Source/Material/Mat_wood.IDAssets"));

	st->SetStaticMesh(rd);
	SetActorScale(IceDogUtils::float3(1 / 10.0, 1 / 10.0, 1 / 10.0));
	SetEnable();
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
	float tar = (GetActorRotation().y + 0.01);
	if (tar > 360) { tar = 0; }
	//SetActorRotation(IceDogUtils::float3(0, tar, 0));
}
