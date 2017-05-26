#pragma once

#include "VoxelData.h"

namespace IceDogRendering
{
	class DynamicVoxelData : public VoxelData
	{
	public:
		/* mark data map dirty, so the render manager will update the data using the data that this render data holds */
		void MarkDataMapStateDirty(DataMapDirtyFlag dmdf);
		/* get is dynamic will return true */
		bool GetIsDynamicBuffer() override;
	};
}

