#pragma once
#include "../Engine/Gameplay/Actor.h"

class MyActor:public IceDogGameplay::Actor
{
public:
	MyActor();
	~MyActor();

	void Tick(float deltaTime) override;
};

