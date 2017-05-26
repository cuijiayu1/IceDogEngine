#pragma once
#include "Component.h"
#include "../../../Rendering/RenderData/DynamicMeshData.h"

namespace IceDogGameplay 
{
	class DynamicMeshComponent:public Component
	{
	public:
		DynamicMeshComponent(class Actor* owner);
		virtual ~DynamicMeshComponent();

		/* call to close this resource */
		virtual void Close() override;

	private:
		std::shared_ptr<IceDogRendering::DynamicMeshData> r_renderData;
	};
}

