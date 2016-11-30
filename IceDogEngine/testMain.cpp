#include "Platform\Platform.h"
#include <stdlib.h>
#include "Core\EngineCore.h"
#include "Rendering\RenderingAdapter.h"
#include "Geometry\GeometryGenerator.h"
#include "Rendering\DynamicRenderData.h"

using namespace std;

#pragma warning(disable:4996)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);
	
	std::shared_ptr<IceDogRendering::RenderData> rd=std::make_shared<IceDogRendering::DynamicRenderData>();
	IceDogGeometry::GeometryGenerator::CreateSphere(2, 20,20, rd);

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

	ec.Run();

	cout << "Hello Ice Dog Engine" << endl;
	system("pause");
	fclose(stdout);
	fclose(stdin);
	FreeConsole();
}