
#include "CameraComponent.h"
#include "../../Engine.h"

namespace IceDogGameplay
{
	CameraComponent::CameraComponent(class Actor* owner) :Component(owner)
	{
		r_pipeView = std::make_shared<IceDogRendering::PipeView>(IceDogEngine::Engine::GetEngine()->GetAspectRatio());
		IceDogEngine::Engine::GetEngine()->RegistMainPipeView(r_pipeView);

		// bind the message process function
		r_msgProc.BindProcessor(std::bind(&CameraComponent::ProcessMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	CameraComponent::~CameraComponent()
	{
	}

	void CameraComponent::Close()
	{
		Component::Close();
	}

	void CameraComponent::Update()
	{
		r_pipeView->SetEyePosition(c_owner->GetActorLocation());
		r_pipeView->SetFocusPosition(c_owner->GetActorLookAt());
		r_pipeView->SetUpDirection(c_owner->GetActorUpVector());
		r_pipeView->UpdateViewMatrix();
	}

	int CameraComponent::ProcessMessage(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1)
	{
		// when aspect ratio change , update the projection matrix
		if (msgType == IceDogPlatform::MessageType::aspectRatioChange)
		{
			r_pipeView->SetAspectRatio(pm0);
		}
		return 0;
	}

}