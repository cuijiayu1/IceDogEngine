#pragma once
#include "Component.h"
#include "../../../Core/MessageProc.h"

namespace IceDogGameplay
{
	class EventComponent:public Component,public IceDogCore::MessageProc
	{
	public:
		EventComponent(class Actor* owner);
		~EventComponent();

		void SetEnable() override;

		void SetDisable() override;

		int Process(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1) override;

		void BindOnLeftDoubleClick(std::function<int(float, float)> func) { onLeftDoubleClick = func; }
		void BindOnLeftDown(std::function<int(float, float)> func) { onLeftDown = func; }
		void BindOnLeftUp(std::function<int(float, float)> func) { onLeftUp = func; }
		void BindOnMidDoubleClick(std::function<int(float, float)> func) { onMidDoubleClick = func; }
		void BindOnMidDown(std::function<int(float, float)> func) { onMidDown = func; }
		void BindOnMidUp(std::function<int(float, float)> func) { onMidUp = func; }
		void BindOnMouseMove(std::function<int(float, float)> func) { onMouseMove = func; }
		void BindOnMouseWheel(std::function<int(bool)> func) { onMouseWheel = func; }
		void BindOnRightDoubleClick(std::function<int(float, float)> func) { onRightDoubleClick = func; }
		void BindOnRightDown(std::function<int(float, float)> func) { onRightDown = func; }
		void BindOnRightUp(std::function<int(float, float)> func) { onRightUp = func; }
		void BindOnKeyDown(std::function<int(int)> func) { onKeyDown = func; }
		void BindOnKeyUp(std::function<int(int)> func) { onKeyUp = func; }
		void BindOnAspectRatioChange(std::function<int(float)> func) { onAspectRatioChange = func; }

	private:
		std::function<int(float, float)> onLeftDoubleClick;
		std::function<int(float, float)> onLeftDown;
		std::function<int(float, float)> onLeftUp;
		std::function<int(float, float)> onMidDoubleClick;
		std::function<int(float, float)> onMidDown;
		std::function<int(float, float)> onMidUp;
		std::function<int(float, float)> onMouseMove;
		std::function<int(bool)>         onMouseWheel;
		std::function<int(float, float)> onRightDoubleClick;
		std::function<int(float, float)> onRightDown;
		std::function<int(float, float)> onRightUp;
		std::function<int(int)>			 onKeyDown;
		std::function<int(int)>			 onKeyUp;
		std::function<int(float)>		 onAspectRatioChange;
	};
}

