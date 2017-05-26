#include "MotherSun.h"



MotherSun::MotherSun()
{
	st = new IceDogGameplay::DirectionalLightComponent(this);

	SetEnable();
}


MotherSun::~MotherSun()
{
}

void MotherSun::SetDirection(IceDogUtils::float3 dir)
{
	st->SetLightDirection(dir);
}
