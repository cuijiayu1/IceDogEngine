#include "LoadTestAct.h"
#include "../Engine/Engine.h"
#include "../Utils/Loader/ObjMeshLoader.h"

LoadTestAct::LoadTestAct()
{
	IceDogGameplay::StaticMeshComponent* st = new IceDogGameplay::StaticMeshComponent(this);

	IceDogUtils::ObjMeshLoader oml;
	std::shared_ptr<IceDogRendering::MeshData> md = std::make_shared<IceDogRendering::MeshData>();
	oml.LoadObj2Mesh("./Source/Model/MatPreviewMesh_Internal.obj", md);

	st->SetStaticMesh(md);

	//md->SetMaterial(IceDogEngine::Engine::GetEngine()->LoadMaterialFromUrl("Source/Material/Mat_Solider.IDAssets"));

	SetActorRotation(IceDogUtils::float3(0,90, 0));
	SetActorScale(IceDogUtils::float3(0.005, 0.005, 0.005));
	SetActorLocation(IceDogUtils::float3(-0.7,-0.5,-1));
	SetEnable();
}

LoadTestAct::~LoadTestAct()
{
}
