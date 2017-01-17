#include "MessageProc.h"
#include "../Engine/Engine.h"

namespace IceDogCore
{
	MessageProc::MessageProc()
	{
		c_msgPriority = MessagePriority::INPUT_1;
	}


	MessageProc::MessageProc(const MessagePriority& priority):c_msgPriority(priority)
	{

	}

	MessageProc::~MessageProc()
	{
	}

	void MessageProc::Init()
	{
		IceDogEngine::Engine::GetEngine()->GetEngineCore().RegistMessageProc(this);
	}

	void MessageProc::SetPriority(const MessagePriority& priority)
	{
		c_msgPriority = priority;
		IceDogEngine::Engine::GetEngine()->GetEngineCore().MarkMessageChainDirty();
	}
	MessagePriority MessageProc::GetPriority()
	{
		return c_msgPriority;
	}
}