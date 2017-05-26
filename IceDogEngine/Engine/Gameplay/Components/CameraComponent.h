#pragma once
#include "../../../Core/VectorSpace.h"
#include "../../../Rendering/PipeView.h"
#include "Component.h"

namespace IceDogGameplay
{
	class CameraComponent:public Component
	{
	public:
		CameraComponent(class Actor* owner);
		~CameraComponent();

		/* call to close this resource */
		virtual void Close() override;

		/* update the view matrix */
		void Update();

		/* process the message that received */
		int ProcessMessage(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1);

	protected:
		// the pipe view
		std::shared_ptr<IceDogRendering::PipeView> r_pipeView;
	};
}