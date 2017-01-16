
#include "CameraComponent.h"
#include "../../Engine.h"

namespace IceDogGameplay
{
	CameraComponent::CameraComponent(class Actor* owner) :Component(owner),PipeView(IceDogEngine::Engine::GetEngine()->GetAspectRatio())
	{
		IceDogEngine::Engine::GetEngine()->RegistMainPipeView(std::shared_ptr<PipeView>(this));
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

}