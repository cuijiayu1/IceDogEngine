#include "LoadTestAct.h"
#include "../Utils/Loader/ObjMeshLoader.h"

LoadTestAct::LoadTestAct()
{
	IceDogGameplay::StaticMeshComponent* st = new IceDogGameplay::StaticMeshComponent(this);

	IceDogUtils::ObjMeshLoader oml;
	std::shared_ptr<IceDogRendering::MeshData> md = std::make_shared<IceDogRendering::MeshData>();
	oml.LoadObj2Mesh("./Source/Model/fireAxe.obj", md);

	st->SetStaticMesh(md);

	SetActorScale(IceDogUtils::float3(0.01, 0.01, 0.01));
	SetActorLocation(IceDogUtils::float3(-1,0,0));
	SetEnable();
}

LoadTestAct::~LoadTestAct()
{
}
