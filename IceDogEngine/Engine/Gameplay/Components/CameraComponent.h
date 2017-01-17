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

		/* update the view matrix */
		void Update();

		/* process the message that received */
		int ProcessMessage(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1);

	protected:
		// the basic coordinate
		IceDogCore::VectorSpace c_basicSpace;
	};
}