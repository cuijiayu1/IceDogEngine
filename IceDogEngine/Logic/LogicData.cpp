#include "LogicData.h"

IceDogLogic::LogicData::~LogicData()
{
	std::cout << "LogicData Released" << std::endl;
}

void IceDogLogic::LogicData::Tick(float deltaTime)
{
	for (auto i:r_tickList)
	{
		i(deltaTime);
	}
}

void IceDogLogic::LogicData::Close()
{
	r_tickList.clear();
}

void IceDogLogic::LogicData::AddTickFunction(std::function<void(float)> tick)
{
	r_tickList.push_back(tick);
}