#include "DynamicVoxelData.h"

void IceDogRendering::DynamicVoxelData::MarkDataMapStateDirty(DataMapDirtyFlag dmdf)
{
	c_dataMapFlag = dmdf;
}

bool IceDogRendering::DynamicVoxelData::GetIsDynamicBuffer()
{
	return true;
}
