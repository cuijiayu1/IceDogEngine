#pragma once
#include "RenderingManager.h"
#include "RenderingPipe.h"

namespace IceDogRendering
{
	enum class RenderPipeType
	{
		Scene,
		UI
	};

	class RenderingAdapter
	{
	public:
		/* consturct function */
		RenderingAdapter(std::ostream& errOS);

		/* init the adapter */
		void Init(IceDogPlatform::PlatformWindow pfWindow);

		/* rendering tick */
		void TickRendering();

		/* regist Render data, to the rendering pipe line */
		void RegistRenderData(std::shared_ptr<RenderData> rd, RenderPipeType rpt);

	private:
		// the rendering manager
		std::shared_ptr<RenderingManager> r_renderingManager;
		// the log output port
		std::ostream& s_errorlogOutStream;
	};
}

