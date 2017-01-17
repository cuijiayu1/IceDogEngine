#include "Camera.h"

namespace IceDogGameplay
{

	Camera::Camera()
	{
		r_cameraComponent = new CameraComponent(this);
		SetEnable();
	}

	Camera::~Camera()
	{
		delete r_cameraComponent;
	}

}
