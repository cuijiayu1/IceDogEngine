#include "RenderDataBase.h"

IceDogRendering::RenderDataBase::RenderDataBase()
{
	c_dataType = DataType::None;
	c_worldMatrix = float4x4::Identity();
	c_worldInverseTransposeMatrix = float4x4::Identity();
	c_materilaData = nullptr;
}

IceDogRendering::RenderDataBase::~RenderDataBase()
{
	std::cout << "RenderData Released" << std::endl;
}

void IceDogRendering::RenderDataBase::Close()
{
	std::cout << "RenderData Closed" << std::endl;
}

void IceDogRendering::RenderDataBase::SetWorldMatrix(const float4x4& worldMatrix)
{
	c_worldMatrix = worldMatrix;
#if defined __DIRECTX__
	XMMATRIX wMat(c_worldMatrix.m);
	wMat = XMMatrixInverse(nullptr, wMat);
	wMat = XMMatrixTranspose(wMat);
	XMStoreFloat4x4((XMFLOAT4X4*)c_worldInverseTransposeMatrix.m, wMat);
#endif
}

bool IceDogRendering::RenderDataBase::GetDataIsClean()
{
	return c_isDataClean;
}

IceDogRendering::DataMapDirtyFlag IceDogRendering::RenderDataBase::GetDataMapFlag()
{
	return c_dataMapFlag;
}

void IceDogRendering::RenderDataBase::MarkDataStateClean()
{
	c_isDataClean = true;
}

void IceDogRendering::RenderDataBase::MarkDataStateDirty()
{
	c_isDataClean = false;
}

void IceDogRendering::RenderDataBase::MarkDataMapStateClean()
{
	c_dataMapFlag = DataMapDirtyFlag::None;
}

bool IceDogRendering::RenderDataBase::GetIsDynamicBuffer()
{
	return false;
}

int IceDogRendering::RenderDataBase::GetVertexCount()
{
	return c_vertexCount;
}
