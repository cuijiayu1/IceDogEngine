#pragma once
#include "../Utils/Common/UtilBasic.h"
#include "../Platform/Message.h"
#include "MessageProc.h"
#include "Timer.h"

// the engine core is the heart of this engine but not the engine itself, so the core should not consist of other parts
namespace IceDogCore
{
	class EngineCore
	{
	public:
		/* the construct function */
		EngineCore();
		/* deconstruct engine */
		~EngineCore();
		/* call close to release the resource */
		void Close();
		/* init the engine */
		void Init();
		/* call the engine run */
		void Run();
		/* call the core to shutdown */
		void Shutdown();
		/* register the platform tick function */
		void RegistPlatformTick(std::function<void()> pfTick);
		/* register the rendering tick function */
		void RegistRenderingTick(std::function<void()> redTick);
		/* register the logic tick function */
		void RegistLogicTick(std::function<void(float)> logicTick);
		/* process the message in a message chain chain, let the message flow in a chain that any registered processor can have ability to receive it*/
		void ProcessMessageChain(IceDogPlatform::Message msg);
		/* register the message processor */
		void RegistMessageProc(MessageProc* msprc);
		/* unregister the message processor */
		void UnRegistMessageProc(MessageProc* msprc);
		/* mark message line dirty */
		void MarkMessageChainDirty();
		/* board cast fps message */
		void BoardCastFPSMessage();

		std::function<void()> testTick;

	private:
		/* mark message line dirty */
		void MarkMessageChainClean();
		
	private:
		// the timer of this core
		Timer r_coreTimer;
		// engine run flag
		bool c_runEngine;

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
