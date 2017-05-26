#pragma once

#include "../../../Utils/Common/UtilBasic.h"
#include "../../../Core/MessageProc.h"

namespace IceDogGameplay
{
	class Component
	{
	public:
		Component(class Actor* owner);
		virtual ~Component();
	public:
		/* call to close component */
		virtual void Close();

		/* enable or disable this component */
		virtual void SetEnable();
		virtual void SetDisable();

		/* update the data */
		virtual void Update();
	protected:
		// whether this component work or not
		bool c_componentEnable;

		// the owner indicated who own this component
		Actor* c_owner;

		// the event processor which use this to receive and process event
		IceDogCore::MessageProc r_msgProc;
		
	};
}