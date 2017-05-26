#include "DynamicMeshData.h"

using namespace IceDogRendering;

bool DynamicMeshData::GetIsDynamicBuffer()
{
	return true;
}

void DynamicMeshData::MarkDataMapStateDirty(DataMapDirtyFlag dmdf)
{
	c_dataMapFlag = dmdf;
}