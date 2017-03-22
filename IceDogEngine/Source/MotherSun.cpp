#include "MotherSun.h"



MotherSun::MotherSun()
{
	IceDogGameplay::DirectionalLightComponent* st = new IceDogGameplay::DirectionalLightComponent(this);

	SetEnable();
}


MotherSun::~MotherSun()
{
}
