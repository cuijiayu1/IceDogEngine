#pragma once
#include "../Utils/Common/UtilBasic.h"
#include "PlatformDependenceRenderResource.h"
#include "RenderingPipe.h"
#include "RenderData/RenderDataBase.h"
#include "../Platform/PlatformWindow.h"
#include "ShaderManager.h"

namespace IceDogRendering
{
	enum class RenderDriverType
	{
		hardware,
		software
	};

	class RenderingManager
	{
	public:
		/* the construct function */
		RenderingManager(std::ostream& logOs);
		/* call close to release the resource */
		virtual void Close();
		/* set the driver type */
		virtual void SetDriverType(RenderDriverType type) = 0;
		/* init the render manager */
		virtual bool InitRenderManager(IceDogPlatform::PlatformWindow pfWindow) = 0;
		/* get the platform dependence resource */
		virtual PlatformDependenceRenderResource GetPDRR() = 0;
		/* register the scene pipe */
		virtual void RegistScenePipe(std::shared_ptr<RenderingPipe> sPip) { r_scenePipe = sPip; };
		/* register the ui pipe */
		virtual void RegistUIPipe(std::shared_ptr<RenderingPipe> uiPip) { r_uiPipe = uiPip; };
		/* register the ui pipe render data */
		virtual void RegistUIRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd) = 0;
		/* register the scene pipe render data */
		virtual void RegistSceneRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd)=0;
		/* unregister the scene render data */
		virtual void UnRegistSceneRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd) = 0;
		/* unregister the UI render data */
		virtual void UnRegistUIRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd) = 0;
		/* register the main pipe view */
		virtual void RegistMainPipeView(std::shared_ptr<IceDogRendering::PipeView> pv) = 0;
		/* register the Light data */
		virtual void RegistLightData(std::shared_ptr<IceDogRendering::LightBase> ld, LightType ltp) = 0;
		/* unregister the light data */
		virtual void UnRegistLightData(std::shared_ptr<IceDogRendering::LightBase> ld, LightType ltp) = 0;
		/* tick */
		virtual void TickRenderingManager()=0;
	
	protected:
		// the main pipe, usually the 3d scene
		std::shared_ptr<RenderingPipe> r_scenePipe;
		// the ui pipe
		std::shared_ptr<RenderingPipe> r_uiPipe;
		// the shader manager
		std::shared_ptr<ShaderManager> r_shaderManager;
		// the scenePipe render data
		std::vector<std::shared_ptr<RenderDataBase>> r_sceneRenderData;
		// the ui render data
		std::vector<std::shared_ptr<RenderDataBase>> r_uiRenderData;
		// the error log output port
		std::ostream& s_errorlogOutStream; 
	};
}

