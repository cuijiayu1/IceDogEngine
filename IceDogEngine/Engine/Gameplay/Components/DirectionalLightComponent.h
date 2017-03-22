#pragma once

#include "LightComponent.h"

namespace IceDogGameplay
{
	class DirectionalLightComponent : public LightComponent
	{
	public:
		DirectionalLightComponent(class Actor* owner);
		~DirectionalLightComponent();

		/* set enable */
		virtual void SetEnable() override;
		/* set disable */
		virtual void SetDisable() override;
		/* set direction */
		void SetLightDirection(IceDogUtils::float3 direction);

		/* update the location from owner */
		virtual void Update() override;
	};
}

