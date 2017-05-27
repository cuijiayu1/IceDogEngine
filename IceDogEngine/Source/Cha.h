#pragma once
#include "../Engine/Gameplay/Camera.h"

class Cha:public IceDogGameplay::Camera
{
public:
	Cha();
	~Cha();
	
	int OnLeftDown(float x, float y);
	int OnLeftUp(float x, float y);
	int OnMove(float x, float y);
	int OnKeyDown(int id);
	int OnKeyUp(int id);

protected:
	void Tick(float deltaTime) override;

private:
	IceDogUtils::float2 mouseStart;
	IceDogUtils::float3 sourceRotation;
	bool enableMove;
	int forward;
	int right;
	float speed;
};

