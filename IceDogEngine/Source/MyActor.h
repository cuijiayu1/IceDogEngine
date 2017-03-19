#pragma once
#include "../Rendering/RenderData/DynamicVoxelData.h"
#include "../Engine/Gameplay/Components/VoxelComponent.h"
#include "../Engine/Gameplay/Actor.h"

class MyActor :public IceDogGameplay::Actor
{
public:
	MyActor();
	~MyActor();

	int OnLeftClick(float x, float y);

private:
	float MetaBall(float r, IceDogCore::float3 start, IceDogCore::float3 center);

protected:
	void Tick(float deltaTime) override;

private:
	IceDogRendering::VoxelVertex* vl;
	IceDogGameplay::VoxelComponent* r_vc;
	IceDogCore::float3 sourceLocation;

	IceDogCore::float3 s_1;
	IceDogCore::float3 s_2;

	float time = 0;

	std::shared_ptr<IceDogRendering::DynamicVoxelData> rd;
};

