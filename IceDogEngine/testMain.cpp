#include <stdlib.h>
#include "Engine\Engine.h"
#include "Source\MyActor.h"
#include "Source\Cha.h"
#include "Source\PlaneActor.h"

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

	Cha cha;
	cha.SetActorLocation(IceDogUtils::float3(0, 1, -1));

	PlaneActor plan;
	plan.SetActorLocation(IceDogUtils::float3(0, -0.5, 0));
	MyActor actor;

	eg.Run();


	EndConsole();
}