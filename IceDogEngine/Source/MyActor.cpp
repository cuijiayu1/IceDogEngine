#include "MyActor.h"
#include "../Engine/Engine.h"
#include "../Rendering/RenderData/MeshData.h"
#include "../Rendering/RenderData/DynamicVoxelData.h"
#include "../Resources/Geometry/GeometryGenerator.h"
#include "../Utils/Noise/OtherNoise.h"

using namespace IceDogGameplay;

MyActor::MyActor() :sourceLocation(1, 1, 1)
{
	using IceDogAlgorithm::NoiseSampleInPlace;
	using IceDogUtils::float3;
	using IceDogUtils::float4;

	// test code
	r_vc= new VoxelComponent(this);
	rd = std::make_shared<IceDogRendering::DynamicVoxelData>();
	//IceDogResources::IceDogGeometry::GeometryGenerator::CreateTeapot(10, 2, false, rd);

	r_defaultEventComponent.BindOnLeftDown(std::bind(&MyActor::OnLeftClick, this, std::placeholders::_1, std::placeholders::_2));

	vl = new IceDogRendering::VoxelVertex[8000];
	SetActorRotation(float3(0, 0, 0));

	s_1 = float3(0, 10, 10);
	s_2 = float3(20, 10, 10);
	
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			for (int k = 0; k < 20; ++k)
			{
				vl[i * 400 + j * 20 + k].pos = float3(i, j, k);
				vl[i * 400 + j * 20 + k].val_f.x = min(IceDogUtils::float3length(float3(i, j, k) - s_1), IceDogUtils::float3length(float3(i, j, k) - s_2));
				vl[i * 400 + j * 20 + k].val_f.y = min(IceDogUtils::float3length(float3(i + 1, j, k) - s_1), IceDogUtils::float3length(float3(i + 1, j, k) - s_2));
				vl[i * 400 + j * 20 + k].val_f.z = min(IceDogUtils::float3length(float3(i + 1, j + 1, k) - s_1), IceDogUtils::float3length(float3(i + 1, j + 1, k) - s_2));
				vl[i * 400 + j * 20 + k].val_f.w = min(IceDogUtils::float3length(float3(i, j + 1, k) - s_1), IceDogUtils::float3length(float3(i, j + 1, k) - s_2));
				vl[i * 400 + j * 20 + k].val_b.x = min(IceDogUtils::float3length(float3(i, j, k + 1) - s_1), IceDogUtils::float3length(float3(i, j, k + 1) - s_2));
				vl[i * 400 + j * 20 + k].val_b.y = min(IceDogUtils::float3length(float3(i + 1, j, k + 1) - s_1), IceDogUtils::float3length(float3(i + 1, j, k + 1) - s_2));
				vl[i * 400 + j * 20 + k].val_b.z = min(IceDogUtils::float3length(float3(i + 1, j + 1, k + 1) - s_1), IceDogUtils::float3length(float3(i + 1, j + 1, k + 1) - s_2));
				vl[i * 400 + j * 20 + k].val_b.w = min(IceDogUtils::float3length(float3(i, j + 1, k + 1) - s_1), IceDogUtils::float3length(float3(i, j + 1, k + 1) - s_2));
			}
		}
	}

	rd->SetIsoLevel(5);
	rd->SetVertexData(vl, 8000);
	//rd->SetMaterial(IceDogEngine::Engine::GetEngine()->LoadMaterialFromUrl("Source/Material/Mat_wood.IDAssets"));

	r_vc->SetVoxelData(rd);
	SetActorScale(IceDogUtils::float3(1/20.0,1/ 20.0,1/ 20.0));
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
	//float isoLevel = r_vc->GetVoxelData()->GetIsoLevel();
	//r_vc->GetVoxelData()->SetIsoLevel(isoLevel + 10*deltaTime);
	float tar = (GetActorRotation().y + 0.01);
	if (tar > 360) { tar = 0; }
	//SetActorRotation(IceDogUtils::float3(0, tar, 0));
}
