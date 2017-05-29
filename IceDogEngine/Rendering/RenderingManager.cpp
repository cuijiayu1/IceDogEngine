#include "RenderingManager.h"

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
