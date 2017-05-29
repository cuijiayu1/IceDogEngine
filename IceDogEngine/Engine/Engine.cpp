#include "Engine.h"

using namespace IceDogEngine;

Engine* Engine::r_egPtr = nullptr; 

Engine::Engine(std::ostream& errorLog, IceDogPlatform::PlatformWindow plfWindow)
	:s_errorlogOutStream(errorLog),
	r_enginePlatform(plfWindow,errorLog),
	r_renderAdapter(errorLog),
	r_logicAdapter(errorLog),
	r_aspectRatio(static_cast<float>(plfWindow.width)/static_cast<float>(plfWindow.height))
{
	r_egPtr = this;
	// the platform window will be further construct, we will not hold it.
}

void Engine::Init()
{
	// init the engine core
	r_engineCore.Init();
	// init the platform
	r_enginePlatform.InitPlatform();
	// register the platform event
	r_engineCore.RegistPlatformTick(std::bind(&IceDogPlatform::Platform::TickPlatform, &r_enginePlatform));
	r_enginePlatform.RegistMessageProcessChain(std::bind(&IceDogCore::EngineCore::ProcessMessageChain, &r_engineCore, std::placeholders::_1));
	// init the render adapter
	r_renderAdapter.Init(r_enginePlatform.GetConstructedPlatformWindow());
	r_engineCore.RegistRenderingTick(std::bind(&IceDogRendering::RenderingAdapter::TickRendering, &r_renderAdapter));
	// init the logic adapter
	r_logicAdapter.Init();
	r_engineCore.RegistLogicTick(std::bind(&IceDogLogic::LogicAdapter::TickLogic, &r_logicAdapter, std::placeholders::_1));
	// init the message processor
	r_msgProc = IceDogCore::MessageProc::Create(IceDogCore::MessagePriority::EM_5);
	r_msgProc->BindProcessor(std::bind(&Engine::EventProcessor, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	// init the level
	r_defaultLevel.Init();
}

void Engine::RegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd,IceDogRendering::RenderPipeType rpt)
{
	// add data to level
	r_defaultLevel.RegistRenderData(rd);
	r_renderAdapter.RegistRenderData(rd, rpt);
}

void IceDogEngine::Engine::UnRegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd, IceDogRendering::RenderPipeType rpt)
{
	// remove from level
	r_defaultLevel.UnRegistRenderData(rd);
	r_renderAdapter.UnRegistRenderData(rd, rpt);
}


void IceDogEngine::Engine::RegisterLightData(std::shared_ptr<class IceDogRendering::LightBase> ld, IceDogRendering::LightType ltp)
{
	// add to level
	r_defaultLevel.RegistLightData(ld);
	r_renderAdapter.RegisterLightData(ld, ltp);
}

void IceDogEngine::Engine::UnRegisterLightData(std::shared_ptr<class IceDogRendering::LightBase> ld, IceDogRendering::LightType ltp)
{
	// remove from level
	r_defaultLevel.UnRegistLightData(ld);
	r_renderAdapter.UnRegisterLightData(ld, ltp);
}

void Engine::RegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld)
{
	// add data to level
	r_defaultLevel.RegistLogicData(ld);
	r_logicAdapter.RegistLogicData(ld);
}

void Engine::UnRegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld)
{
	// remove 
	r_defaultLevel.UnRegistLogicData(ld);
	r_logicAdapter.UnRegistLogicData(ld);
}

void IceDogEngine::Engine::RemoveActor(IceDogGameplay::Actor* ac)
{
	r_defaultLevel.UnRegistActorByAddress(ac);
}

void IceDogEngine::Engine::RegistMainPipeView(std::shared_ptr<IceDogRendering::PipeView> pv)
{
	r_renderAdapter.RegistMainPipeView(pv);
}

float IceDogEngine::Engine::GetAspectRatio()
{
	return r_aspectRatio;
}

IceDogCore::EngineCore& IceDogEngine::Engine::GetEngineCore()
{
	return r_engineCore;
}

int IceDogEngine::Engine::EventProcessor(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1)
{
	if (msgType == IceDogPlatform::MessageType::systemResize)
	{
		r_aspectRatio = pm0 / pm1;
		IceDogPlatform::Message msg;
		msg.c_messageAuthority = IceDogPlatform::MessageAuthority::SYSTEM;
		msg.c_messageType = IceDogPlatform::MessageType::aspectRatioChange;
		msg.c_param0 = r_aspectRatio;
		r_engineCore.ProcessMessageChain(msg);
	}
	if (msgType == IceDogPlatform::MessageType::fps)
	{
		r_enginePlatform.SetWindowTitle("IceDogEngine            fps:" + std::to_string(int(pm0)));
	}
	return 0;
}

IceDogRendering::MaterialData* IceDogEngine::Engine::LoadMaterialFromUrl(std::string url)
{
	// create a material object
	IceDogRendering::MaterialData* matdata = new IceDogRendering::MaterialData();
	// load the material description
	rapidxml::file<> materialDesc(url.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(materialDesc.data());

	if (doc.first_node()->first_node("diffuse")->first_attribute()->value()[0] == '1')
	{
		matdata->SetDiffuseMap(IceDogUtils::char2wchar(doc.first_node()->first_node("diffuse")->first_node("url")->value()));
	}
	if (doc.first_node()->first_node("normal")->first_attribute()->value()[0] == '1')
	{
		matdata->SetNormalMap(IceDogUtils::char2wchar(doc.first_node()->first_node("normal")->first_node("url")->value()));
	}
	if (doc.first_node()->first_node("parallax")->first_attribute()->value()[0] == '1')
	{
		float scale = atof(doc.first_node()->first_node("parallax")->first_attribute("scale")->value());
		float layers = atof(doc.first_node()->first_node("parallax")->first_attribute("layer")->value());
		float quility = atof(doc.first_node()->first_node("parallax")->first_attribute("quility")->value());
		matdata->SetParallaxMap(IceDogUtils::char2wchar(doc.first_node()->first_node("parallax")->first_node("url")->value()), scale, layers, quility);
	}
	if (doc.first_node()->first_node("srme")->first_attribute()->value()[0]=='1')
	{
		matdata->SetSRMEMap(IceDogUtils::char2wchar(doc.first_node()->first_node("srme")->first_node("url")->value()));
	}
	r_renderAdapter.RegisterMaterialData(matdata);
	return matdata;
}

IceDogEngine::Engine* IceDogEngine::Engine::GetEngine()
{
	return r_egPtr;
}

void IceDogEngine::Engine::Close()
{
	AmazingText(std::cout, "Stop rendering");
	r_engineCore.Shutdown();

	AmazingText(std::cout, "Close level");
	r_defaultLevel.Close();
	AmazingText(std::cout, "Close engine core");
	r_engineCore.Close();
	AmazingText(std::cout, "Close render adapter");
	r_renderAdapter.Close();
	AmazingText(std::cout, "Close logic adapter");
	r_logicAdapter.Close();
	AmazingText(std::cout, "Close message chain");
	r_msgProc->Close();
	AmazingText(std::cout, "Close platform");
	r_enginePlatform.Close();
	AmazingText(std::cout, "Engine shutdown");
}

void IceDogEngine::Engine::Run()
{
	// call the engine core to run. update the tick
	r_engineCore.Run();
}

void IceDogEngine::Engine::AmazingText(std::ostream& os, std::string str)
{
	std::cout << "----------------------------------------------------------------------" << std::endl;
	std::cout << "\t\t\t" << str << std::endl;
	std::cout << "----------------------------------------------------------------------" << std::endl;
}
