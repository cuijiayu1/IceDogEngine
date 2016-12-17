#pragma once
#include "..\Platform\Platform.h"
#include "..\Core\EngineCore.h"
#include "..\Rendering\RenderingAdapter.h"
#include "..\Logic\LogicAdapter.h"

namespace IceDogEngine
{
	// this class player the box role, which pack everthing up
	class Engine
	{
	public:
		Engine(std::ostream& errorLog, IceDogPlatform::PlatformWindow plfWindow);

		/* init the engine */
		void Init();

		/* run the engine */
		void Run();

		/* regist the render data */
		void RegistRenderData(std::shared_ptr<IceDogRendering::RenderData> rd, IceDogRendering::RenderPipeType rpt);

		/* regist the logic data */
		void RegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld);

	private:
		// the heart of the engine control the tick
		IceDogCore::EngineCore r_engineCore;

		// the platform that this engine play on
		IceDogPlatform::Platform r_enginePlatform;

		// the render adapter run on it`s own thread, handle the render task
		IceDogRendering::RenderingAdapter r_renderAdapter;

		// the logic adapter run on it`s own thread, handle the logic task
		IceDogLogic::LogicAdapter r_logicAdapter;

		// the log output port
		std::ostream& s_errorlogOutStream;
	};
}

