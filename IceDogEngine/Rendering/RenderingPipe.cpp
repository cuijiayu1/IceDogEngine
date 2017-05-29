#include "RenderingPipe.h"

#define LIGHTGROUPSIZE 1

void IceDogRendering::RenderingPipe::RegistLightData(std::shared_ptr<LightBase> ld, LightType ltp)
{
	switch (ltp)
	{
	case IceDogRendering::LightType::Directional:
		AddDirectionalLightToLightGroup(std::dynamic_pointer_cast<DirectionLightData>(ld));
		break;
	case IceDogRendering::LightType::Point:
		//r_defaultLG.AddPointLight(std::dynamic_pointer_cast<Po>(ld))
		break;
	case IceDogRendering::LightType::Spot:
		break;
	default:
		break;
	}
}

void IceDogRendering::RenderingPipe::UnRegistLightData(std::shared_ptr<LightBase> ld, LightType ltp)
{

}

void IceDogRendering::RenderingPipe::AddDirectionalLightToLightGroup(std::shared_ptr<DirectionLightData> dl)
{
	if (r_lightGroups.size()==0)
	{
		r_lightGroups.push_back(IceDogRendering::LightingGroup<LIGHTGROUPSIZE>());
		r_lightGroups[0].AddDirectionalLight(dl);
	}
	else
	{
		if (r_lightGroups[r_lightGroups.size() - 1].HasDirectionalLightsSpace()) {
			r_lightGroups[r_lightGroups.size() - 1].AddDirectionalLight(dl);
		}
		else {
			r_lightGroups.push_back(IceDogRendering::LightingGroup<LIGHTGROUPSIZE>());
			r_lightGroups[r_lightGroups.size() - 1].AddDirectionalLight(dl);
		}
	}
}

void IceDogRendering::RenderingPipe::Close()
{
	for (auto& i:r_lightGroups)
		i.Close();
	r_lightGroups.clear();
	r_pipeViewList.clear();
	r_cubeMapSource.Close();
	r_messageProc->Close();
}
