#pragma once
#include "../Utils/Common/UtilBasic.h"
#include "../Platform/Message.h"
#include "MessageProc.h"
#include "Timer.h"

// the engine core is the heart of this engine but not the engine itself, so the core should not consisit of other parts
namespace IceDogCore
{
	class EngineCore
	{
	public:
		/* the construct function */
		EngineCore();
		/* init the engine */
		void Init();
		/* call the engine run */
		void Run();
		/* regist the platform tick function */
		void RegistPlatformTick(std::function<void()> pfTick);
		/* regist the rendering tick function */
		void RegistRenderingTick(std::function<void()> redTick);
		/* regist the logic tick function */
		void RegistLogicTick(std::function<void(float)> logicTick);
		/* process the message in a message chain chain */
		void ProcessMessageChain(IceDogPlatform::Message msg);
		/* regist the message processor */
		void RegistMessageProc(MessageProc* msprc);
		/* unregist the message processor */
		void UnRegistMessageProc(MessageProc* msprc);
		/* mark message line dirty */
		void MarkMessageChainDirty();

		std::function<void()> testTick;

	private:
		/* mark message line dirty */
		void MarkMessageChainClean();
		
	private:
		// the timer of this core
		Timer r_coreTimer;

	private:
		// is message line dirty
		bool c_messageChainDirty=true;
		// the message process units
		std::vector<MessageProc*> r_messageProcs;
		// the platform tick port, call this to call platform tick
		std::function<void()> c_platformTickPort;
		// the rendering tick port,call this to call the rendering tick
		std::function<void()> c_renderingTickPort;
		// the logic tick port, call this to call the logic tick to update all the logic
		std::function<void(float)> c_logicTickPort;
	};
}
