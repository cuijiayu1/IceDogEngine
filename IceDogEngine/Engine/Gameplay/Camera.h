#pragma once

#include "Actor.h"
#include "Components/CameraComponent.h"

namespace IceDogGameplay
{
	class Camera:public Actor
	{
	public:
		Camera();
		~Camera();
	private:
		CameraComponent* r_cameraComponent;
	};
}

