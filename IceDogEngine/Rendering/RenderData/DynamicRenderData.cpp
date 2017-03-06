#include "DynamicRenderData.h"

using namespace IceDogRendering;

bool DynamicRenderData::GetIsDynamicBuffer()
{
	return true;
}

void DynamicRenderData::MarkDataMapStateDirty(DataMapDirtyFlag dmdf)
{
	c_dataMapFlag = dmdf;
}