#pragma once

#include "../Utils/Common/UtilBasic.h"

namespace IceDogLogic
{
	class LogicData
	{
	public:
		// the release function
		virtual ~LogicData();
		// the tick function source
		virtual void Tick(float deltaTime);
		// close the logic data
		virtual void Close();
		// add a tick function to tick list
		void AddTickFunction(std::function<void(float)> tick);

	private:
		std::vector<std::function<void(float)>> r_tickList;
	};
}

