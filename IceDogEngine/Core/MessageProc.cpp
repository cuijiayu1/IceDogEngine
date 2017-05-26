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

	void MessageProc::CloseProc()
	{
		IceDogEngine::Engine::GetEngine()->GetEngineCore().UnRegistMessageProc(this);
	}

	void MessageProc::Close()
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

	void MessageProc::BindProcessor(std::function<int(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1)> proc)
	{
		c_procFunc = proc;
	}

	int MessageProc::Process(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1)
	{
		if (c_procFunc) { return c_procFunc(msgType, pm0, pm1); }
		return 0;
	}

}