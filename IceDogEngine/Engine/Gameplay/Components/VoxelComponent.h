#pragma once

#include "Component.h"
#include "../../../Rendering/RenderData/VoxelData.h"

namespace IceDogGameplay 
{
	class VoxelComponent : public Component
	{
	public:
		VoxelComponent(class Actor* owner);
		~VoxelComponent();

		/* set the render data */
		void SetVoxelData(std::shared_ptr<IceDogRendering::VoxelData> rd);

		/* get the render data */
		const std::shared_ptr<IceDogRendering::VoxelData> GetVoxelData() { return r_renderData; }

		/* set enable */
		virtual void SetEnable() override;
		/* set disable */
		virtual void SetDisable() override;

		/* update the location from owner */
		virtual void Update() override;

	private:
		// the ptr to the hold voxel data
		std::shared_ptr<IceDogRendering::VoxelData> r_renderData;
	};
}

