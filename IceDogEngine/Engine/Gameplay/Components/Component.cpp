
#include "../Actor.h"

namespace IceDogGameplay
{
	Component::Component(Actor* owner):r_msgProc(IceDogCore::MessagePriority::SYSTEM_3)
	{
		c_owner = owner;
		if (c_owner != nullptr)
		{
			c_owner->RegistComponentToActor(std::shared_ptr<Component>(this));
		}
		c_componentEnable = true;
		r_msgProc.Init();
	}

	void Component::SetEnable()
	{
		c_componentEnable = true;
	}

	void Component::SetDisable()
	{
		c_componentEnable = false;
	}

	void Component::Update()
	{

	}

}