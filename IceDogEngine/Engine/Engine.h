#pragma once
#include "..\Platform\Platform.h"
#include "..\Core\EngineCore.h"
#include "..\Rendering\RenderingAdapter.h"
#include "..\Logic\LogicAdapter.h"
#include "Gameplay\Level.h"

namespace IceDogEngine
{
	// this class player the box role, which pack everthing up
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

		/* regist/unregist the render data */
		void RegistRenderData(std::shared_ptr<IceDogRendering::RenderData> rd, IceDogRendering::RenderPipeType rpt);
		void UnRegistRenderData(std::shared_ptr<IceDogRendering::RenderData> rd, IceDogRendering::RenderPipeType rpt);

		/* regist/unregist the logic data */
		void RegistLogicData(IceDogLogic::LogicData* ld);
		void UnRegistLogicData(IceDogLogic::LogicData* ld);

		/* regist/unregist the actor */
		void RegistActor(IceDogGameplay::Actor* ac);
		void UnRegistActor(IceDogGameplay::Actor* ac);

		/* regist the main pipe view */
		void RegistMainPipeView(std::shared_ptr<IceDogRendering::PipeView> pv);

		/* get the aspect ratio of this platform */
		float GetAspectRatio();

		/* return the engine core ref */
		IceDogCore::EngineCore& GetEngineCore();

		/* messaage processor function */
		int EventProcessor(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1);

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

