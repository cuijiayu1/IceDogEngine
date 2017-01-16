#pragma once

namespace IceDogGameplay
{
	class Component
	{
	public:
		Component(class Actor* owner);
	public:
		/* enable or disable this component */
		virtual void SetEnable();
		virtual void SetDisable();
	protected:
		// whether this component work or not
		bool c_componentEnable;

		// the owner indicated who own this component
		Actor* c_owner;
		
	};
}