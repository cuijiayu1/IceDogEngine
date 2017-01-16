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

void IceDogGameplay::Level::UnRegistRenderData(std::shared_ptr<IceDogRendering::RenderData> rd)
{
	r_renderDatas.erase(std::find(r_renderDatas.begin(), r_renderDatas.end(), rd));
}

void Level::RegistActor(IceDogGameplay::Actor* ac)
{
	r_actors.push_back(ac);
}

void IceDogGameplay::Level::UnRegistActor(IceDogGameplay::Actor* ac)
{
	r_actors.erase(std::find(r_actors.begin(), r_actors.end(), ac));
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
