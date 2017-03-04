#include "Cha.h"

Cha::Cha()
{
	SetActorRotation(IceDogUtils::float3(40, 0, 0));
	SetActorLocation(IceDogUtils::float3(0, 2, -4));
	r_defaultEventComponent.BindOnLeftDown(std::bind(&Cha::OnLeftDown, this, std::placeholders::_1, std::placeholders::_2));
	r_defaultEventComponent.BindOnLeftUp(std::bind(&Cha::OnLeftUp, this, std::placeholders::_1, std::placeholders::_2));
	r_defaultEventComponent.BindOnMouseMove(std::bind(&Cha::OnMove, this, std::placeholders::_1, std::placeholders::_2));
	r_defaultEventComponent.BindOnKeyDown(std::bind(&Cha::OnKeyDown, this, std::placeholders::_1));
	r_defaultEventComponent.BindOnKeyUp(std::bind(&Cha::OnKeyUp, this, std::placeholders::_1));
}

Cha::~Cha()
{
}

int Cha::OnLeftDown(float x, float y)
{
	sourceRotation = GetActorRotation();
	mouseStart = IceDogUtils::float2(x, y);
	enableMove = true;
	return 0;
}

int Cha::OnLeftUp(float x, float y)
{
	enableMove = false;
	return 0;
}

int Cha::OnMove(float x, float y)
{
	if (!enableMove) {return 0; }
	SetActorRotation(IceDogUtils::float3(sourceRotation.x+(y-mouseStart.y)/6.28,sourceRotation.y+(x-mouseStart.x)/6.28,sourceRotation.z));
	return 0;
}

int Cha::OnKeyDown(int id)
{
	switch (id)
	{
	case 17:
		forward = 1;
		break;
	case 31:
		forward = -1;
		break;
	case 30:
		right = -1;
		break;
	case 32:
		right = 1;
		break;
	default:
		break;
	}
	return 0;
}

int Cha::OnKeyUp(int id)
{
	switch (id)
	{
	case 17:case 31:
		forward = 0;
		break;
	case 30:case 32:
		right = 0;
		break;
	default:
		break;
	}
	return 0;
}

void Cha::Tick(float deltaTime)
{
	if (forward!=0)
	{
		MoveForward(forward*deltaTime);
	}
	if (right!=0)
	{
		MoveRight(right*deltaTime);
	}
}
