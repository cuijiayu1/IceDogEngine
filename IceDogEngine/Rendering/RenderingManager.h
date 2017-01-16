#pragma once
#include "../Utils/Common/UtilBasic.h"
#include "PlatformDependenceRenderResource.h"
#include "RenderingPipe.h"
#include "RenderData.h"
#include "../Platform/PlatformWindow.h"

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
		RenderingManager(std::ostream& logOs) :s_errorlogOutStream(logOs) {}
		/* set the driver type */
		virtual void SetDriverType(RenderDriverType type) = 0;
		/* init the render manager */
		virtual bool InitRenderManager(IceDogPlatform::PlatformWindow pfWindow) = 0;
		/* get the platform dependence resource */
		virtual PlatformDependenceRenderResource GetPDRR() = 0;
		/* regist the scene pipe */
		virtual void RegistScenePipe(std::shared_ptr<RenderingPipe> sPip) { r_scenePipe = sPip; };
		/* regist the ui pipe */
		virtual void RegistUIPipe(std::shared_ptr<RenderingPipe> uiPip) { r_uiPipe = uiPip; };
		/* regist the ui pipe render data */
		virtual void RegistUIRenderData(std::shared_ptr<IceDogRendering::RenderData> rd) = 0;
		/* regist the scene pipe render data */
		virtual void RegistSceneRenderData(std::shared_ptr<IceDogRendering::RenderData> rd)=0;
		/* unregist the scene render data */
		virtual void UnRegistSceneRenderData(std::shared_ptr<IceDogRendering::RenderData> rd) = 0;
		/* unregist the UI render data */
		virtual void UnRegistUIRenderData(std::shared_ptr<IceDogRendering::RenderData> rd) = 0;
		/* tick */
		virtual void TickRenderingManager()=0;
	
	protected:
		// the main pipe, usually the 3d scene
		std::shared_ptr<RenderingPipe> r_scenePipe;
		// the ui pipe
		std::shared_ptr<RenderingPipe> r_uiPipe;
		// the scenePipe render data
		std::vector<std::shared_ptr<RenderData>> r_sceneRenderData;
		// the ui render data
		std::vector<std::shared_ptr<RenderData>> r_uiRenderData;
		// the error log output port
		std::ostream& s_errorlogOutStream; 
	};
}

