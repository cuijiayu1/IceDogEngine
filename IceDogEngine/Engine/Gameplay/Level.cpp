#include "Level.h"

using namespace IceDogGameplay;

Level::Level()
{
}

Level::~Level()
{
	std::cout << "Level Released" << std::endl;
}

void Level::RegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd)
{
	r_renderDatas.push_back(rd);
}

void IceDogGameplay::Level::Init()
{

}

void IceDogGameplay::Level::Close()
{
	// close all render data
	for (auto i:r_renderDatas)
		i->Close();
	r_renderDatas.clear();
	// close all logic data
	for (auto i : r_logicDatas)
		i->Close();
	r_logicDatas.clear();
	// close all actor
	for (auto i : r_actors)
		i->Close();
	r_actors.clear();
	std::cout << "Level Closed" << std::endl;
}

void IceDogGameplay::Level::UnRegistRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd)
{
	r_renderDatas.erase(std::find(r_renderDatas.begin(), r_renderDatas.end(), rd));
}

void Level::RegistActor(std::shared_ptr<IceDogGameplay::Actor> ac)
{
	r_actors.push_back(ac);
}

void IceDogGameplay::Level::UnRegistActor(std::shared_ptr<IceDogGameplay::Actor> ac)
{
	r_actors.erase(std::find(r_actors.begin(), r_actors.end(), ac));
}

void IceDogGameplay::Level::UnRegistActorByAddress(IceDogGameplay::Actor* ac)
{
	for (std::vector<std::shared_ptr<IceDogGameplay::Actor>>::iterator iter=r_actors.begin();iter!=r_actors.end();++iter)
	{
		if ((*iter).get() == ac)
		{
			r_actors.erase(iter);
			break;
		}
	}
}

void IceDogGameplay::Level::RegistLightData(std::shared_ptr<class IceDogRendering::LightBase> ld)
{
	r_lightDatas.push_back(ld);
}

void IceDogGameplay::Level::UnRegistLightData(std::shared_ptr<class IceDogRendering::LightBase> ld)
{
	r_lightDatas.erase(std::find(r_lightDatas.begin(), r_lightDatas.end(), ld));
}

void Level::RegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld)
{
	r_logicDatas.push_back(ld);
}

void IceDogGameplay::Level::UnRegistLogicData(std::shared_ptr<IceDogLogic::LogicData> ld)
{
	// remove from the logic data list
	r_logicDatas.erase(std::find(r_logicDatas.begin(), r_logicDatas.end(), ld));
}
