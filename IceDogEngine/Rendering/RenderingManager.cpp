#include "RenderingManager.h"
#include "DirectX/DirectXDeferredPipe.h"

IceDogRendering::RenderingManager::RenderingManager(std::ostream& logOs) :s_errorlogOutStream(logOs)
{
	r_shaderManager = std::make_shared<ShaderManager>();
}

void IceDogRendering::RenderingManager::Close()
{
	if(r_scenePipe!=nullptr)
		r_scenePipe->Close();
	if(r_uiPipe!=nullptr)
		r_uiPipe->Close();
	for (auto &i: r_sceneRenderData)
		i->Close();
	r_sceneRenderData.clear();
	for (auto &i : r_uiRenderData)
		i->Close();
	r_uiRenderData.clear();
	r_shaderManager->Close();
}

bool IceDogRendering::RenderingManager::InitRenderManager(IceDogPlatform::PlatformWindow pfWindow)
{
	// init the shader manager
	r_shaderManager->Init(GetPDRR());

#if defined __DIRECTX__
	r_scenePipe = std::make_shared<DirectXDeferredPipe>(s_errorlogOutStream);
#endif
	assert(r_scenePipe != nullptr);
	r_scenePipe->InitPlatformDepedence(GetPDRR());
	r_scenePipe->InitPipe(pfWindow, r_shaderManager.get());
	return true;
}
