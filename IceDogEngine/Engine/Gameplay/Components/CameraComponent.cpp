
#include "CameraComponent.h"
#include "../../Engine.h"

namespace IceDogGameplay
{
	CameraComponent::CameraComponent(class Actor* owner) :Component(owner),PipeView(IceDogEngine::Engine::GetEngine()->GetAspectRatio())
	{
		IceDogEngine::Engine::GetEngine()->RegistMainPipeView(std::shared_ptr<PipeView>(this));

		// bind the message process function
		r_msgProc.BindProcessor(std::bind(&CameraComponent::ProcessMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	CameraComponent::~CameraComponent()
	{
	}

	void CameraComponent::Update()
	{
		SetEyePosition(c_owner->GetActorLocation());
		SetFocusPosition(c_owner->GetActorLookAt());
		SetUpDirection(c_owner->GetActorUpVector());
		UpdateViewMatrix();
	}

	int CameraComponent::ProcessMessage(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1)
	{
		// when aspect ratio change , update the projection matrix
		if (msgType == IceDogPlatform::MessageType::aspectRatioChange)
		{
			SetAspectRatio(pm0);
		}
		return 0;
	}

}