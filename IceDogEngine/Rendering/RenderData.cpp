#include "RenderData.h"

using namespace IceDogRendering;

RenderData::RenderData()
{
	c_worldMatrix = float4x4::Identity();
	r_indexBuffer = nullptr;
	r_vertexBuffer = nullptr;
}

int RenderData::GetTriangleCount()
{
	return c_triangleCount;
}

int RenderData::GetVertexCount()
{
	return c_vertexCount;
}

bool RenderData::GetIsDynamicBuffer()
{
	return false;
}

void RenderData::MarkDataMapStateClean()
{
	c_dataMapFlag = DataMapDirtyFlag::None;
}

DataMapDirtyFlag RenderData::GetDataMapFlag()
{
	return c_dataMapFlag;
}

void RenderData::MarkDataStateDirty()
{
	c_isDataClean = false;
}

void RenderData::MarkDataStateClean()
{
	c_isDataClean = true;
}

bool RenderData::GetDataIsClean()
{
	return c_isDataClean;
}

RenderData::~RenderData()
{
#if defined __DIRECTX__
	ReleaseCOM(r_indexBuffer);
	ReleaseCOM(r_vertexBuffer);
#endif
}

void RenderData::SetVertexData(Vertex* buffer, int num)
{
	c_vertexDatas = std::shared_ptr<Vertex>(buffer, [](Vertex* data) {delete[]data; });
	c_vertexCount = num;
#if defined __DIRECTX__
	UpdateVertexBufferDesc();
#endif
}

void RenderData::SetIndexData(unsigned int* indexBuffer, int triangleCount)
{
	c_indexDatas = std::shared_ptr<unsigned int>(indexBuffer, [](unsigned int* data) {delete[]data; });
	c_triangleCount = triangleCount;
#if defined __DIRECTX__
	UpdateIndexBufferDesc();
#endif
}

#if defined __DIRECTX__
bool RenderData::CreateVertexBufferWithIndexBuffer(ID3D11Device* d3dDevice)
{
	ReleaseCOM(r_vertexBuffer);
	ReleaseCOM(r_indexBuffer);
	r_vertexBuffer = nullptr;
	r_indexBuffer = nullptr;

	auto result = d3dDevice->CreateBuffer(&c_bufferDesc, &c_subresourceData, &r_vertexBuffer);
	auto indexResult = d3dDevice->CreateBuffer(&c_indexBufferDesc, &c_indexSubResourceData, &r_indexBuffer);
	return (!ISFAILED(result)) && (!ISFAILED(indexResult));
}
ID3D11Buffer* RenderData::GetIndexBuffer()
{
	return r_indexBuffer;
}

ID3D11Buffer* RenderData::GetVertexBuffer()
{
	return r_vertexBuffer;
}
void RenderData::UpdateIndexBufferDesc()
{
	if (GetIsDynamicBuffer())
		c_indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	else
		c_indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	c_indexBufferDesc.ByteWidth = sizeof(UINT)*c_triangleCount * 3;
	c_indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	if (GetIsDynamicBuffer())
		c_indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else
		c_indexBufferDesc.CPUAccessFlags = 0;
	c_indexBufferDesc.MiscFlags = 0;
	c_indexBufferDesc.StructureByteStride = 0;

	c_indexSubResourceData.pSysMem = c_indexDatas.get();

	ReleaseCOM(r_indexBuffer);
	r_indexBuffer = nullptr;
}

void RenderData::UpdateVertexBufferDesc()
{
	if (GetIsDynamicBuffer())
		c_bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	else
		c_bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	c_bufferDesc.ByteWidth = sizeof(Vertex)*c_vertexCount;
	c_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (GetIsDynamicBuffer())
		c_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else
		c_bufferDesc.CPUAccessFlags = 0;
	c_bufferDesc.MiscFlags = 0;
	c_bufferDesc.StructureByteStride = 0;

	c_subresourceData.pSysMem = c_vertexDatas.get();

	ReleaseCOM(r_vertexBuffer);
	r_vertexBuffer = nullptr;
}
#endif
