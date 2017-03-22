#include "RenderingPipe.h"

void IceDogRendering::RenderingPipe::RegistLightData(std::shared_ptr<LightBase> ld, LightType ltp)
{
	switch (ltp)
	{
	case IceDogRendering::LightType::Directional:
		r_defaultLG.AddDirectionalLight(std::dynamic_pointer_cast<DirectionLightData>(ld));
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
