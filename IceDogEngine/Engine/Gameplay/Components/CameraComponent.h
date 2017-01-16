#pragma once
#include "../../../Core/VectorSpace.h"
#include "../../../Rendering/PipeView.h"
#include "Component.h"

namespace IceDogGameplay
{
	class CameraComponent:public Component,public IceDogRendering::PipeView
	{
	public:
		CameraComponent(class Actor* owner);
		~CameraComponent();

		void Update();

	protected:
		// the basic coordinate
		IceDogCore::VectorSpace c_basicSpace;
	};
}