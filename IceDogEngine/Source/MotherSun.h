#pragma once

#include "../Rendering/RenderData/MeshData.h"
#include "../Engine/Gameplay/Components/DirectionalLightComponent.h"
#include "../Engine/Gameplay/Actor.h"

class MotherSun : public IceDogGameplay::Actor
{
public:
	MotherSun();
	~MotherSun();

	void SetDirection(IceDogUtils::float3);
	void SetInst(float val);

	IceDogGameplay::DirectionalLightComponent* st;
};

