
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
		r_msgProc = IceDogCore::MessageProc::Create(IceDogCore::MessagePriority::SYSTEM_3);
	}

	Component::~Component()
	{
		std::cout << "Component Released" << std::endl;
	}

	void Component::Close()
	{
		r_msgProc->Close();
		std::cout << "Component Closed" << std::endl;
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