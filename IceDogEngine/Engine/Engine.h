#pragma once
#include "..\Platform\Platform.h"
#include "..\Core\EngineCore.h"
#include "..\Rendering\RenderingAdapter.h"
#include "..\Logic\LogicAdapter.h"
#include "Gameplay\Level.h"
#include "../Rendering/MaterialData.h"

namespace IceDogEngine
{
	// this class player the box role, which pack everything up
	class Engine
	{
	private:
		static Engine* r_egPtr;
	public:
		// return the engine instance
		static Engine* GetEngine();
		Engine(std::ostream& errorLog, IceDogPlatform::PlatformWindow plfWindow);
	public:
		/* init the engine */
		void Init();

		/* run the engine */
		void Run();

		/* register/unregister the render data */
		void RegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd, IceDogRendering::RenderPipeType rpt);
		void UnRegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd, IceDogRendering::RenderPipeType rpt);

		/* register/unregister the light data */
		void RegisterLightData(std::shared_ptr<class IceDogRendering::LightBase> ld, IceDogRendering::LightType ltp);
		void UnRegisterLightData(std::shared_ptr<class IceDogRendering::LightBase> ld, IceDogRendering::LightType ltp);

		/* register/unregister the logic data */
		void RegistLogicData(IceDogLogic::LogicData* ld);
		void UnRegistLogicData(IceDogLogic::LogicData* ld);

		/* register/unregister the actor */
		void RegistActor(IceDogGameplay::Actor* ac);
		void UnRegistActor(IceDogGameplay::Actor* ac);

		/* register the main pipe view */
		void RegistMainPipeView(std::shared_ptr<IceDogRendering::PipeView> pv);

		/* get the aspect ratio of this platform */
		float GetAspectRatio();

		/* return the engine core ref */
		IceDogCore::EngineCore& GetEngineCore();

		/* message processor function */
		int EventProcessor(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1);

		/* load material data from file */
		IceDogRendering::MaterialData* LoadMaterialFromUrl(std::string url);

	private:
		// the total aspect ratio of this platform
		float r_aspectRatio;

		// the current level that this hold
		IceDogGameplay::Level r_defaultLevel;

		// the heart of the engine control the tick
		IceDogCore::EngineCore r_engineCore;

		// the platform that this engine play on
		IceDogPlatform::Platform r_enginePlatform;

		// the render adapter run on it`s own thread, handle the render task
		IceDogRendering::RenderingAdapter r_renderAdapter;

		// the logic adapter run on it`s own thread, handle the logic task
		IceDogLogic::LogicAdapter r_logicAdapter;

		// the message processor
		IceDogCore::MessageProc r_msgProc;

		// the log output port
		std::ostream& s_errorlogOutStream;
	};
}

