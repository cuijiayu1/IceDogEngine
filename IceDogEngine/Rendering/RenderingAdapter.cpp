#include "RenderingAdapter.h"

#if defined __DIRECTX__
#include "DirectX\DirectXRenderingManager.h"
#include "DirectX\DirectXDeferredPipe.h"
#endif

using namespace IceDogRendering;

void RenderingAdapter::TickRendering()
{
	if (r_renderingManager != nullptr)
	{
		r_renderingManager->TickRenderingManager();
	}
}

void RenderingAdapter::RegistRenderData(std::shared_ptr<RenderDataBase> rd, RenderPipeType rpt)
{
	switch (rpt)
	{
	case IceDogRendering::RenderPipeType::Scene:
		r_renderingManager->RegistSceneRenderData(rd);
		break;
	case IceDogRendering::RenderPipeType::UI:
		r_renderingManager->RegistUIRenderData(rd);
		break;
	default:
		break;
	}
}

void IceDogRendering::RenderingAdapter::Close()
{
	r_renderingManager->Close();
	for (auto i:r_materialDatas)
	{
		i->Close();
		delete i;
	}
	std::cout << "RenderAdapter Closed" << std::endl;
}

void IceDogRendering::RenderingAdapter::UnRegistRenderData(std::shared_ptr<RenderDataBase> rd, RenderPipeType rpt)
{
	switch (rpt)
	{
	case IceDogRendering::RenderPipeType::Scene:
		r_renderingManager->UnRegistSceneRenderData(rd);
		break;
	case IceDogRendering::RenderPipeType::UI:
		r_renderingManager->UnRegistUIRenderData(rd);
		break;
	default:
		break;
	}
}

void IceDogRendering::RenderingAdapter::RegisterMaterialData(IceDogRendering::MaterialData* matdata)
{
	r_materialDatas.push_back(matdata);
	matdata->LoadMaterial(r_renderingManager->GetPDRR());
}

void IceDogRendering::RenderingAdapter::UnRegisterMaterialData(IceDogRendering::MaterialData* matdata)
{
	r_materialDatas.erase(std::find(r_materialDatas.begin(), r_materialDatas.end(), matdata));
	delete matdata;
}

void IceDogRendering::RenderingAdapter::RegisterLightData(std::shared_ptr<LightBase> ld, LightType ltp)
{
	r_renderingManager->RegistLightData(ld, ltp);
}

void IceDogRendering::RenderingAdapter::UnRegisterLightData(std::shared_ptr<LightBase> ld, LightType ltp)
{
	r_renderingManager->UnRegistLightData(ld, ltp);
}

void IceDogRendering::RenderingAdapter::RegistMainPipeView(std::shared_ptr<PipeView> pv)
{
	r_renderingManager->RegistMainPipeView(pv);
}

RenderingAdapter::RenderingAdapter(std::ostream& errOS):s_errorlogOutStream(errOS)
{

}

void RenderingAdapter::Init(IceDogPlatform::PlatformWindow pfWindow)
{
#if defined __DIRECTX__
	r_renderingManager = std::make_shared<DirectXRenderingManager>(s_errorlogOutStream);
#endif

	assert(r_renderingManager != nullptr);
	r_renderingManager->InitRenderManager(pfWindow);
}

