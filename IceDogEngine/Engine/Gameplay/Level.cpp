#include "Level.h"

using namespace IceDogGameplay;

Level::Level()
{
}

Level::~Level()
{
}

void Level::RegistRenderData(std::shared_ptr<IceDogRendering::RenderData> rd)
{
	r_renderDatas.push_back(rd);
}

void Level::RegistActor(std::shared_ptr<IceDogGameplay::Actor> ac)
{
	r_actors.push_back(ac);
}

void Level::RegistLogicData(IceDogLogic::LogicData* ld)
{
	r_logicDatas.push_back(ld);
}

void IceDogGameplay::Level::UnRegistLogicData(IceDogLogic::LogicData* ld)
{
	// remove from the logic data list
	r_logicDatas.erase(std::find(r_logicDatas.begin(), r_logicDatas.end(), ld));
}
