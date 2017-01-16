#pragma once
#include "RenderingManager.h"
#include "RenderingPipe.h"
#include "RenderPipeType.h"

namespace IceDogRendering
{
	class RenderingAdapter
	{
	public:
		/* consturct function */
		RenderingAdapter(std::ostream& errOS);

		/* init the adapter */
		void Init(IceDogPlatform::PlatformWindow pfWindow);

		/* rendering tick */
		void TickRendering();

		/* regist/unregist Render data, to the rendering pipe line */
		void RegistRenderData(std::shared_ptr<RenderData> rd, RenderPipeType rpt);
		void UnRegistRenderData(std::shared_ptr<RenderData> rd, RenderPipeType rpt);

		/* regist the pipeview */
		void RegistMainPipeView(std::shared_ptr<PipeView> pv);

	private:
		// the rendering manager
		std::shared_ptr<RenderingManager> r_renderingManager;
		// the log output port
		std::ostream& s_errorlogOutStream;
	};
}

