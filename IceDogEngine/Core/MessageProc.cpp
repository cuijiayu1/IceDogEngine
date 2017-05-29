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
		SetPriority(MessagePriority::EXIT_N1);
	}

	void MessageProc::Close()
	{
		SetPriority(MessagePriority::EXIT_N1);
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

	std::shared_ptr<IceDogCore::MessageProc> MessageProc::Create(MessagePriority pri)
	{
		std::shared_ptr<IceDogCore::MessageProc> ret = std::make_shared<IceDogCore::MessageProc>(pri);
		IceDogEngine::Engine::GetEngine()->GetEngineCore().RegistMessageProc(ret);
		return ret;
	}

	std::shared_ptr<IceDogCore::MessageProc> MessageProc::Create()
	{
		std::shared_ptr<IceDogCore::MessageProc> ret = std::make_shared<IceDogCore::MessageProc>();
		IceDogEngine::Engine::GetEngine()->GetEngineCore().RegistMessageProc(ret);
		return ret;
	}

}