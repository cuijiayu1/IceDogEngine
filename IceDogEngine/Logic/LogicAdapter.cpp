#include "LogicAdapter.h"

IceDogLogic::LogicAdapter::LogicAdapter(std::ostream & errOs):s_errorlogOutStream(errOs)
{

}

IceDogLogic::LogicAdapter::~LogicAdapter()
{
	std::cout<<"LogicAdapter Released" << std::endl;
}

void IceDogLogic::LogicAdapter::Init()
{
}

void IceDogLogic::LogicAdapter::Close()
{
	for (auto &i : r_logicDatas)
		i->Close();
	r_logicDatas.clear();
	std::cout << "LogicAdapter Closed" << std::endl;
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

void IceDogLogic::LogicAdapter::UnRegistLogicData(std::shared_ptr<LogicData> ld)
{
	r_logicDatas.erase(std::find(r_logicDatas.begin(), r_logicDatas.end(), ld));
}
