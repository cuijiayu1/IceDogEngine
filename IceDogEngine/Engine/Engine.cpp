#include "Engine.h"

using namespace IceDogEngine;


Engine::Engine(std::ostream& errorLog, IceDogPlatform::PlatformWindow plfWindow)
	:s_errorlogOutStream(errorLog),
	r_enginePlatform(plfWindow,errorLog),
	r_renderAdapter(errorLog),
	r_logicAdapter(errorLog)
{
	// the platform window will be further construct, we will not hold it.
}

void Engine::Init()
{
	// init the engine core
	r_engineCore.Init();
	// init the platform
	r_enginePlatform.InitPlatform();
	// regist the platform event
	r_engineCore.RegistPlatformTick(std::bind(&IceDogPlatform::Platform::TickPlatform, &r_enginePlatform));
	r_enginePlatform.RegistMessageProcessChain(std::bind(&IceDogCore::EngineCore::ProcessMessageChain, &r_engineCore, std::placeholders::_1));
	// init the render adapter
	r_renderAdapter.Init(r_enginePlatform.GetConstructedPlatformWindow());
	r_engineCore.RegistRenderingTick(std::bind(&IceDogRendering::RenderingAdapter::TickRendering, &r_renderAdapter));
	// init the logic adapter
	r_logicAdapter.Init();
	r_engineCore.RegistLogicTick(std::bind(&IceDogLogic::LogicAdapter::TickLogic, &r_logicAdapter, std::placeholders::_1));
}

void Engine::RegistRenderData(std::shared_ptr<IceDogRendering::RenderData> rd,IceDogRendering::RenderPipeType rpt)
{
	r_renderAdapter.RegistRenderData(rd, rpt);
}

void Engine::RegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld)
{
	r_logicAdapter.RegistLogicData(ld);
}

void IceDogEngine::Engine::Run()
{
	// call the engine core to run. update the tick
	r_engineCore.Run();
}
