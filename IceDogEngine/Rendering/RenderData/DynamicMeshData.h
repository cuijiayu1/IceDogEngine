#pragma once

#include "MeshData.h"

namespace IceDogRendering
{
	class DynamicMeshData: public MeshData
	{
	public:
		/* mark data map dirty, so the render manager will update the data using the data that this render data holds */
		void MarkDataMapStateDirty(DataMapDirtyFlag dmdf);
		/* get is dynamic will return true */
		bool GetIsDynamicBuffer() override;
	};
}

