#include <stdlib.h>
#include "Engine\Engine.h"
#include "Source\LoadTestAct.h"
#include "Source\MyActor.h"
#include "Source\Cha.h"
#include "Source\PlaneActor.h"
#include "Source\Sphere.h"
#include "Source\MotherSun.h"
#include "Source\Plane.h"
#include "Source\FakePhysicBall.h"

#include <random>

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
	eg.AmazingText(std::cout, "Hello IceDogEngine");

	Cha* cha = eg.ConstructActor<Cha>(new Cha());
	MotherSun* sun = eg.ConstructActor<MotherSun>(new MotherSun());

	PlaneActor* plan = eg.ConstructActor<PlaneActor>(new PlaneActor());
	plan->SetActorLocation(IceDogUtils::float3(0, -1.05, 0));
	MyActor* actor = eg.ConstructActor<MyActor>(new MyActor());
	actor->SetActorLocation(IceDogUtils::float3(0, 0, 0));

	for (int i=0;i<2;++i)
	{
		for (int j=0;j<2;++j)
		{
			for (int k=0;k<2;++k)
			{
				Sphere* sps = eg.ConstructActor<Sphere>(new Sphere());
				sps->SetActorLocation(IceDogUtils::float3(i, j, k));
			}
		}
	}
	LoadTestAct Lt;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(-1, 1);
	for (int i = 0; i < 12; ++i)
	{
		FakePhysicBall* fpb = eg.ConstructActor<FakePhysicBall>(new FakePhysicBall());
		fpb->SetInitSpeed(IceDogUtils::float3(dis(gen), dis(gen), dis(gen)) * 30);
	}

	eg.Run();

	EndConsole();
}