#pragma once
#include "Component.h"
#include "../../../Rendering/RenderData.h"

namespace IceDogGameplay
{
	class StaticMeshComponent:public Component
	{
	public:
		StaticMeshComponent();
		~StaticMeshComponent();

		/* set the render data */
		void SetStaticMesh(std::shared_ptr<IceDogRendering::RenderData> rd);

	private:
		std::shared_ptr<IceDogRendering::RenderData> r_renderData;
	};
}

