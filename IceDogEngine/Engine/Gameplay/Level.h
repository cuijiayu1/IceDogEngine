#pragma once
#include "../../Utils/Common/UtilBasic.h"
#include "../../Rendering/RenderData.h"
#include "../../Rendering/RenderingAdapter.h"
#include "../../Logic/LogicData.h"
#include "Actor.h"

namespace IceDogGameplay
{
	class Level
	{
	public:
		Level();
		~Level();

		/* regist the render data */
		void RegistRenderData(std::shared_ptr<IceDogRendering::RenderData> rd);

		/* regist the logic data */
		void RegistLogicData(IceDogLogic::LogicData* ld);
		void UnRegistLogicData(IceDogLogic::LogicData* ld);

		/* regist the actor */
		void RegistActor(std::shared_ptr<IceDogGameplay::Actor> ac);

	private:
		// the render data that this level hold
		std::vector<std::shared_ptr<IceDogRendering::RenderData>> r_renderDatas;
		// the logic data that this level hold
		std::vector<IceDogLogic::LogicData*> r_logicDatas;
		// the actor that this level hold
		std::vector<std::shared_ptr<IceDogGameplay::Actor>> r_actors;
	};
}

