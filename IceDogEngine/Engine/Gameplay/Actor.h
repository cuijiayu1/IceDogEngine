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

	private:
		// the actor component that every actor have one. just the default one
		ActorComponent r_defaultActorComponent;
	};
}

