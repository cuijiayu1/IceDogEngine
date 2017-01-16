#pragma once

#include "Component.h"
#include "../../../Logic/LogicData.h"

namespace IceDogGameplay
{
	// the actor component inherit from both component and logic data, handle the usual game play logic
	class ActorComponent:public Component,public IceDogLogic::LogicData
	{
	public:
		ActorComponent(class Actor* owner);
		~ActorComponent();

	public:
		/* tick the logic */
		void Tick(float deltaTime) override;

		/* enable this component ,add to logic data list */
		void SetEnable() override;

		/* disable this component ,remove from logic data list */
		void SetDisable() override;

		/* the owning actor call this to regist the tick */
		void RegistOwningActorTick(std::function<void(float)> tickFunc);

	private:
		// the tick function from owning actor that have this tick
		std::function<void(float)> c_actorTickFunc;
	};
}

