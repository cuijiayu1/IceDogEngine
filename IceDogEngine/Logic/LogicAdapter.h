#pragma once

#include "../Utils/Common/UtilBasic.h"
#include "LogicData.h"

// logic adapter run on it`s own thread, handle the logic request

namespace IceDogLogic
{
	class LogicAdapter
	{
	public:
		/* the construct function */
		LogicAdapter(std::ostream& errOs);
		~LogicAdapter();
		/* init the logic adapter */
		void Init();
		/* call close to release the resource */
		void Close();
		/* tick the logic adapter, than tick all registered logic data */
		void TickLogic(float deltaTime);
		/* register/unRegister the logic data */
		void RegistLogicData(std::shared_ptr<LogicData> ld);
		void UnRegistLogicData(std::shared_ptr<LogicData> ld);

	private:
		// the log output port
		std::ostream& s_errorlogOutStream;
		// the logic data that this adapter holds
		std::vector<std::shared_ptr<LogicData>> r_logicDatas;
	};
}

