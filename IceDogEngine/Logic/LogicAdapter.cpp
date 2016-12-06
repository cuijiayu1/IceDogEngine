#include "LogicAdapter.h"

IceDogLogic::LogicAdapter::LogicAdapter(std::ostream & errOs):s_errorlogOutStream(errOs)
{

}

void IceDogLogic::LogicAdapter::Init()
{
}

void IceDogLogic::LogicAdapter::TickLogic(float deltaTime)
{
	for (auto& ld:r_logicDatas)
	{
		ld->Tick(deltaTime);
	}
}

void IceDogLogic::LogicAdapter::RegistLogicData(std::shared_ptr<LogicData> ld)
{
	// now just simply put in
	r_logicDatas.push_back(ld);
}
