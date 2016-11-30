#include "Platform\Platform.h"
#include <stdlib.h>
#include "Core\EngineCore.h"
#include "Rendering\RenderingAdapter.h"

using namespace std;

#pragma warning(disable:4996)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);

	IceDogRendering::Vertex vertexs[] =
	{
		{ IceDogRendering::float3(-1.0f,-1.0f,-1.0f),IceDogRendering::float4(IceDogRendering::Color::Red) },
		{ IceDogRendering::float3(-1.0f,1.0f,-1.0f),IceDogRendering::float4(IceDogRendering::Color::Green) },
		{ IceDogRendering::float3(1.0f,1.0f,-1.0f),IceDogRendering::float4(IceDogRendering::Color::Blue) },
		{ IceDogRendering::float3(1.0f,-1.0f,-1.0f),IceDogRendering::float4(IceDogRendering::Color::Black) },
		{ IceDogRendering::float3(-1.0f,-1.0f,1.0f),IceDogRendering::float4(IceDogRendering::Color::Cyan) },
		{ IceDogRendering::float3(-1.0f,1.0f,1.0f),IceDogRendering::float4(IceDogRendering::Color::LightSteelBlue) },
		{ IceDogRendering::float3(1.0f,1.0f,1.0f),IceDogRendering::float4(IceDogRendering::Color::Magenta) },
		{ IceDogRendering::float3(1.0f,-1.0f,1.0f),IceDogRendering::float4(IceDogRendering::Color::Silver) },
	};
	unsigned int indices[36] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	std::shared_ptr<IceDogRendering::RenderData> rd=std::make_shared<IceDogRendering::RenderData>();
	rd->SetVertexData(vertexs, 8);
	rd->SetIndexData(indices,12);

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