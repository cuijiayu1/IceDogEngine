#pragma once

namespace IceDogGameplay
{
	class Component
	{
	public:
		Component() { c_componentEnable = true; }
	public:
		/* enable or disable this component */
		virtual void SetEnable() { c_componentEnable = true; }
		virtual void SetDisable() { c_componentEnable = false; }
	protected:
		// whether this component work or not
		bool c_componentEnable;
	};
}