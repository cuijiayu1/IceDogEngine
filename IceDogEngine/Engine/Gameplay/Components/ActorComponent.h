#pragma once

#include "Component.h"
#include "../../../Logic/LogicData.h"

namespace IceDogGameplay
{
	// the actor component inherit from both component and logic data, handle the usual game play logic
	class ActorComponent:public Component
	{
	public:
		ActorComponent(class Actor* owner);
		~ActorComponent();

	public:
		/* close the component */
		virtual void Close() override;

		/* tick the logic */
		virtual void Tick(float deltaTime) {};

		/* enable this component ,add to logic data list */
		void SetEnable() override;

		/* disable this component ,remove from logic data list */
		void SetDisable() override;

		/* the owning actor call this to regist the tick */
		void RegistOwningActorTick(std::function<void(float)> tickFunc);

	private:
		/* the border of tick for tick function to call */
		void TickBorder(float deltaTime);

	private:
		std::shared_ptr<IceDogLogic::LogicData> r_logicData;
	};
}

