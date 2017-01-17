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
	// if dirty sort the queue
	if (c_messageChainDirty)
	{
		std::sort(r_messageProcs.begin(), r_messageProcs.end(), [](MessageProc* m0, MessageProc* m1)->bool
		{
			return m0->GetPriority() > m1->GetPriority();
		});
		MarkMessageChainClean();
	}

	// for priority and authority separation
	using IceDogPlatform::MessageAuthority;
	if (msg.c_messageAuthority==MessageAuthority::SYSTEM)
	{
		for (auto& i:r_messageProcs)
		{
			// if no priority and no one process just exit
			if (i->GetPriority() < MessagePriority::SYSTEM_3) { return; }
			// if someone processed success just exit
			if (i->Process(msg.c_messageType, msg.c_param0, msg.c_param1)) { return; }
		}
	}
	else
	{
		for (auto& i : r_messageProcs)
		{
			// if someone processed success just exit
			if (i->Process(msg.c_messageType, msg.c_param0, msg.c_param1)) { return; }
		}
	}
	
	// handle the message
	//std::cout << (int)msg.c_messageType << " " << msg.c_param0 << " " << msg.c_param1 << std::endl;
}

void EngineCore::RegistPlatformTick(std::function<void()> pfTick)
{
	c_platformTickPort = pfTick;
}

void EngineCore::RegistRenderingTick(std::function<void()> redTick)
{
	c_renderingTickPort = redTick;
}

void IceDogCore::EngineCore::RegistLogicTick(std::function<void(float)> logicTick)
{
	c_logicTickPort = logicTick;
}

void IceDogCore::EngineCore::RegistMessageProc(MessageProc* msprc)
{
	r_messageProcs.push_back(msprc);
	MarkMessageChainDirty();
}

void IceDogCore::EngineCore::UnRegistMessageProc(MessageProc* msprc)
{
	r_messageProcs.erase(std::find(r_messageProcs.begin(), r_messageProcs.end(), msprc));
}

void IceDogCore::EngineCore::MarkMessageChainDirty()
{
	c_messageChainDirty = true;
}

void IceDogCore::EngineCore::MarkMessageChainClean()
{
	c_messageChainDirty = false;
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
		if (c_logicTickPort) { c_logicTickPort(r_coreTimer.GetDeltaTime()); }
		if (testTick) { testTick(); }
	}
}