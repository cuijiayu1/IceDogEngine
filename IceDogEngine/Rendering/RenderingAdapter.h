#pragma once
#include "RenderingManager.h"
#include "RenderingPipe.h"
#include "RenderPipeType.h"

namespace IceDogRendering
{
	class RenderingAdapter
	{
	public:
		/* construct function */
		RenderingAdapter(std::ostream& errOS);

		/* init the adapter */
		void Init(IceDogPlatform::PlatformWindow pfWindow);

		/* rendering tick */
		void TickRendering();

		/* register/unregister Render data, to the rendering pipe line */
		void RegistRenderData(std::shared_ptr<RenderDataBase> rd, RenderPipeType rpt);
		void UnRegistRenderData(std::shared_ptr<RenderDataBase> rd, RenderPipeType rpt);

		/* register/unregister material data, to the rendering pipe line */
		void RegisterMaterialData(IceDogRendering::MaterialData* matdata);
		void UnRegisterMaterialData(IceDogRendering::MaterialData* matdata);

		/* register/unregister light data, to the rendering pipe line */
		void RegisterLightData(std::shared_ptr<LightBase> ld, LightType ltp);
		void UnRegisterLightData(std::shared_ptr<LightBase> ld, LightType ltp);

		/* register the pipe view */
		void RegistMainPipeView(std::shared_ptr<PipeView> pv);

	private:
		// the rendering manager
		std::shared_ptr<RenderingManager> r_renderingManager;
		// the log output port
		std::ostream& s_errorlogOutStream;
		// the material data
		std::vector<IceDogRendering::MaterialData*> r_materialDatas;
	};
}

