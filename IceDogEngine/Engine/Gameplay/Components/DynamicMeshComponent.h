#pragma once
#include "Component.h"
#include "../../../Rendering/RenderData/DynamicMeshData.h"

namespace IceDogGameplay 
{
	class DynamicMeshComponent:public Component
	{
	public:
		DynamicMeshComponent(class Actor* owner);
		~DynamicMeshComponent();
	private:
		std::shared_ptr<IceDogRendering::DynamicMeshData> r_renderData;
	};
}

