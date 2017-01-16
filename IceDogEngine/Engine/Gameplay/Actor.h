#pragma once
#include "../../Utils/Common/UtilBasic.h"
#include "Components/ActorComponent.h"

namespace IceDogGameplay
{
	// actor will auto regist to the engine, we don`t need to
	class Actor
	{
	public:
		Actor();
		~Actor();

		virtual void Tick(float deltaTime);

		virtual void SetEnable();

		virtual void SetDisable();

		virtual void RegistComponentToActor(std::shared_ptr<Component> comp);

	private:
		// the actor component that every actor have one. just the default one
		ActorComponent r_defaultActorComponent;
		// hold the components
		std::vector<std::shared_ptr<Component>> r_holdComponents;
	};
}

