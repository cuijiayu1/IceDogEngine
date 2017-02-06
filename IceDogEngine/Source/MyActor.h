#pragma once
#include "../Engine/Gameplay/Actor.h"

class MyActor :public IceDogGameplay::Actor
{
public:
	MyActor();
	~MyActor();

	int OnLeftClick(float x, float y);

protected:
	void Tick(float deltaTime) override;

private:
	IceDogCore::float3 sourceLocation;
};

