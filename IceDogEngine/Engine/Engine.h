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

		/* close the engine */
		void Close();

		/* run the engine */
		void Run();

		/* amazing text */
		void AmazingText(std::ostream& os, std::string);

		/* register/unregister the render data */
		void RegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd, IceDogRendering::RenderPipeType rpt);
		void UnRegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd, IceDogRendering::RenderPipeType rpt);

		/* register/unregister the light data */
		void RegisterLightData(std::shared_ptr<class IceDogRendering::LightBase> ld, IceDogRendering::LightType ltp);
		void UnRegisterLightData(std::shared_ptr<class IceDogRendering::LightBase> ld, IceDogRendering::LightType ltp);

		/* register/unregister the logic data */
		void RegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld);
		void UnRegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld);

		/* remove the actor */
		void RemoveActor(IceDogGameplay::Actor* ac);

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

		/* construct actor function */
		template<class T> T* ConstructActor(IceDogGameplay::Actor* actor)
		{
			std::shared_ptr<IceDogGameplay::Actor> act_ptr(actor);
			r_defaultLevel.RegistActor(act_ptr);
			return (T*)(act_ptr.get());
		}

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
		std::shared_ptr<IceDogCore::MessageProc> r_msgProc;

		// the log output port
		std::ostream& s_errorlogOutStream;
	};
}

