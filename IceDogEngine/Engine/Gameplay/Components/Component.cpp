
#include "../Actor.h"

namespace IceDogGameplay
{
	Component::Component(Actor* owner)
	{
		c_owner = owner;
		if (c_owner != nullptr)
		{
			c_owner->RegistComponentToActor(std::shared_ptr<Component>(this));
		}
		c_componentEnable = true;
	}

	void Component::SetEnable()
	{
		c_componentEnable = true;
	}

	void Component::SetDisable()
	{
		c_componentEnable = false;
	}

}