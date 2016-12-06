#include "Platform\Platform.h"
#include <stdlib.h>
#include "Core\EngineCore.h"
#include "Rendering\RenderingAdapter.h"
#include "Logic\LogicAdapter.h"
#include "Resources\Geometry\GeometryGenerator.h"
#include "Rendering\DynamicRenderData.h"

using namespace std;

#pragma warning(disable:4996)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);

	std::shared_ptr<IceDogRendering::RenderData> rd=std::make_shared<IceDogRendering::RenderData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateSphere(1, 30,30,rd);

	IceDogCore::EngineCore ec;
	ec.Init();

	IceDogPlatform::PlatformWindow pfwindow;
	pfwindow.c_windowInstance = hInstance;
	pfwindow.width = 800;
	pfwindow.height = 600;

	IceDogPlatform::Platform pf(pfwindow, std::cout);
	pf.InitPlatform();

	ec.RegistPlatformTick(std::bind(&IceDogPlatform::Platform::TickPlatform, &pf));
	pf.RegistMessageProcessChain(std::bind(&IceDogCore::EngineCore::ProcessMessageChain,&ec,std::placeholders::_1));

	IceDogRendering::RenderingAdapter ra(std::cout);
	ra.Init(pf.GetConstructedPlatformWindow());
	ra.RegistRenderData(rd, IceDogRendering::RenderPipeType::Scene);
	ec.RegistRenderingTick(std::bind(&IceDogRendering::RenderingAdapter::TickRendering, &ra));

	IceDogLogic::LogicAdapter la(std::cout);
	la.Init();
	ec.RegistLogicTick(std::bind(&IceDogLogic::LogicAdapter::TickLogic, &la,std::placeholders::_1));

	ec.Run();

	cout << "Hello Ice Dog Engine" << endl;
	system("pause");
	fclose(stdout);
	fclose(stdin);
	FreeConsole();
}