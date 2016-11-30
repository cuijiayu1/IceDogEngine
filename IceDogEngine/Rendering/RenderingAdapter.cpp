#include "RenderingAdapter.h"

#if defined __DIRECTX__
#include "DirectX\DirectXRenderingManager.h"
#include "DirectX\DirectXRenderingPipe.h"
#endif

using namespace IceDogRendering;

void RenderingAdapter::TickRendering()
{
	if (r_renderingManager != nullptr)
	{
		r_renderingManager->TickRenderingManager();
	}
}

void RenderingAdapter::RegistRenderData(std::shared_ptr<RenderData> rd, RenderPipeType rpt)
{
	switch (rpt)
	{
	case IceDogRendering::RenderPipeType::Scene:
		r_renderingManager->RegistScenePipeRenderData(rd);
		break;
	case IceDogRendering::RenderPipeType::UI:
		r_renderingManager->RegistUIPipeRenderData(rd);
		break;
	default:
		break;
	}
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
	auto pdrr= r_renderingManager->GetPDRR();

	std::shared_ptr<RenderingPipe> scrp;
#if defined __DIRECTX__
	scrp = std::make_shared<DirectXRenderingPipe>(s_errorlogOutStream);
#endif
	assert(scrp != nullptr);
	scrp->InitPlatformDepedence(pdrr);
	scrp->InitPipe(pfWindow);

	r_renderingManager->RegistScenePipe(scrp);
}

