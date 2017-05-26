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

		/* call to close this resource */
		virtual void Close() override;

	protected:
		std::shared_ptr<IceDogRendering::LightBase> r_holdLight;
	};
}

