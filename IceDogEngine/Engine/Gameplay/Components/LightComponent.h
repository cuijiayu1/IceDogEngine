#pragma once

#include "Component.h"
#include "../../../Rendering/LightData/LightBase.h"

namespace IceDogGameplay
{
	class LightComponent : public Component
	{
	public:
		LightComponent(class Actor* owner);
		~LightComponent();

	protected:
		std::shared_ptr<IceDogRendering::LightBase> r_holdLight;
	};
}

