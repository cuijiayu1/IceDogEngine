#pragma once
#include "../../../Core/VectorSpace.h"

namespace IceDogGameplay
{
	class CameraComponent
	{
	public:
		CameraComponent();
		~CameraComponent();

	protected:
		// the basic coordinate
		IceDogCore::VectorSpace c_coordinate;
	};
}