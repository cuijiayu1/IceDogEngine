#include "MyActor.h"
#include "../Engine/Engine.h"
#include "../Engine/Gameplay/Components/VoxelComponent.h"
#include "../Resources/Geometry/GeometryGenerator.h"
#include "../Utils/Noise/OtherNoise.h"

using namespace IceDogGameplay;

MyActor::MyActor() :sourceLocation(1, 1, 1)
{
	using IceDogAlgorithm::NoiseSampleInPlace;
	using IceDogUtils::float3;
	using IceDogUtils::float4;
	// test code
	VoxelComponent* vc = new VoxelComponent(this);
	std::shared_ptr<IceDogRendering::VoxelData> rd = std::make_shared<IceDogRendering::VoxelData>();
	//IceDogResources::IceDogGeometry::GeometryGenerator::CreateTeapot(10, 2, false, rd);

	r_defaultEventComponent.BindOnLeftDown(std::bind(&MyActor::OnLeftClick, this, std::placeholders::_1, std::placeholders::_2));

	SetActorRotation(float3(0, 0, 0));
	
	float3 src(20, 20, 20);
	IceDogRendering::VoxelVertex* vl = new IceDogRendering::VoxelVertex[64000];
	for (int i=0;i<40;++i)
	{
		for (int j=0;j<40;++j)
		{
			for (int k=0;k<40;++k)
			{
				vl[i * 1600 + j * 40 + k].pos = float3(i, j, k);
				vl[i * 1600 + j * 40 + k].val_f.x = IceDogUtils::float3length(float3(i, j, k) - src);
				vl[i * 1600 + j * 40 + k].val_f.y = IceDogUtils::float3length(float3(i+1, j, k) - src);
				vl[i * 1600 + j * 40 + k].val_f.z = IceDogUtils::float3length(float3(i+1, j+1, k) - src);
				vl[i * 1600 + j * 40 + k].val_f.w = IceDogUtils::float3length(float3(i, j+1, k) - src);
				vl[i * 1600 + j * 40 + k].val_b.x = IceDogUtils::float3length(float3(i, j, k+1) - src);
				vl[i * 1600 + j * 40 + k].val_b.y = IceDogUtils::float3length(float3(i+1, j, k+1) - src);
				vl[i * 1600 + j * 40 + k].val_b.z = IceDogUtils::float3length(float3(i+1, j+1, k+1) - src);
				vl[i * 1600 + j * 40 + k].val_b.w = IceDogUtils::float3length(float3(i, j+1, k+1) - src);
			}
		}
	}
	
	rd->SetIsoLevel(20);
	rd->SetVertexData(vl, 64000);
	//rd->SetMaterial(IceDogEngine::Engine::GetEngine()->LoadMaterialFromUrl("Source/Material/Mat_wood.IDAssets"));

	vc->SetVoxelData(rd);
	SetActorScale(IceDogUtils::float3(1/40.0,1/40.0,1/40.0));
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
