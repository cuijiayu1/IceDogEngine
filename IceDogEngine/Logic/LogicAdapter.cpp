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

void IceDogLogic::LogicAdapter::RegistLogicData(LogicData* ld)
{
	// now just simply put in
	r_logicDatas.push_back(ld);
}

void IceDogLogic::LogicAdapter::UnRegistLogicData(LogicData* ld)
{
	r_logicDatas.erase(std::find(r_logicDatas.begin(), r_logicDatas.end(), ld));
}
