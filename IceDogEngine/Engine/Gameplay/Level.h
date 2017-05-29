#pragma once
#include "../../Utils/Common/UtilBasic.h"
#include "../../Rendering/RenderData/RenderDataBase.h"
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

		/* init the level */
		void Init();

		/* close the level */
		void Close();

		/* register the render data */
		void RegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd);
		void UnRegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd);

		/* register the logic data */
		void RegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld);
		void UnRegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld);

		/* register the actor */
		void RegistActor(std::shared_ptr<IceDogGameplay::Actor> ac);
		void UnRegistActor(std::shared_ptr<IceDogGameplay::Actor> ac);
		void UnRegistActorByAddress(IceDogGameplay::Actor* ac);

		/* register the light data */
		void RegistLightData(std::shared_ptr<class IceDogRendering::LightBase> ld);
		void UnRegistLightData(std::shared_ptr<class IceDogRendering::LightBase> ld);

	private:
		// the render data that this level hold
		std::vector<std::shared_ptr<IceDogRendering::RenderDataBase>> r_renderDatas;
		// the logic data that this level hold
		std::vector<std::shared_ptr<IceDogLogic::LogicData>> r_logicDatas;
		// the actor that this level hold
		std::vector<std::shared_ptr<IceDogGameplay::Actor>> r_actors;
		// the light data this level hold
		std::vector<std::shared_ptr<class IceDogRendering::LightBase>> r_lightDatas;
	};
}

