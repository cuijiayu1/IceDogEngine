#pragma once

namespace IceDogCore
{
	enum class MessagePriority
	{
		// for system
		EM_5 = 5,
		PERFORMANCE_4 = 4,
		SYSTEM_3 = 3,

		// for user
		LOGIC_2 = 2,
		INPUT_1 = 1,
		USER_0 = 0,

		// for functional
		EXIT_N1 = -1
	};
}