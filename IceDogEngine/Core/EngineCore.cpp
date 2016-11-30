#include "EngineCore.h"

using namespace IceDogCore;

EngineCore::EngineCore()
{

}

void EngineCore::Init()
{

}

void EngineCore::ProcessMessageChain(IceDogPlatform::Message msg)
{
	// handle the message
	std::cout << (int)msg.c_messageType << " " << msg.c_param0 << " " << msg.c_param1 << std::endl;
}

void EngineCore::RegistPlatformTick(std::function<void()> pfTick)
{
	c_platformTickPort = pfTick;
}

void EngineCore::RegistRenderingTick(std::function<void()> redTick)
{
	c_renderingTickPort = redTick;
}

void EngineCore::Run()
{
	// main loop
	while (true)
	{
		// tick the timer first
		r_coreTimer.Tick();
		// tick platform and update the message
		if (c_platformTickPort) { c_platformTickPort(); }
		if (c_renderingTickPort) { c_renderingTickPort(); }
		if (testTick) { testTick(); }
	}
}