#include <stdlib.h>
#include "Engine\Engine.h"
#include "Resources\Geometry\GeometryGenerator.h"
#include "Rendering\DynamicRenderData.h"

using namespace std;

#pragma warning(disable:4996)

void BeginConsole()
{
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);
}

void EndConsole()
{
	system("pause");
	fclose(stdout);
	fclose(stdin);
	FreeConsole();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	BeginConsole();

	IceDogPlatform::PlatformWindow pfwindow;
	pfwindow.c_windowInstance = hInstance;
	pfwindow.width = 800;
	pfwindow.height = 600;
	
	IceDogEngine::Engine eg(std::cout, pfwindow);
	eg.Init();
	cout << "Hello Ice Dog Engine" << endl;

	std::shared_ptr<IceDogRendering::RenderData> rd=std::make_shared<IceDogRendering::RenderData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateSphere(1, 30,30,rd);

	eg.RegistRenderData(rd, IceDogRendering::RenderPipeType::Scene);

	eg.Run();


	EndConsole();
}