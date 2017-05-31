#include "VoxelData.h"

using namespace IceDogRendering;

VoxelData::VoxelData():c_isoLevel(0.5)
{
	c_dataType = DataType::Voxel;
	r_vertexBuffer = nullptr;
}

VoxelData::~VoxelData()
{
#if defined __DIRECTX__
	ReleaseCOM(r_vertexBuffer);
#endif
}

void IceDogRendering::VoxelData::SetIsoLevel(float isolevel)
{
	c_isoLevel = isolevel;
}

void VoxelData::SetVertexData(VoxelVertex* buffer, int count)
{
	c_vertexDatas = std::shared_ptr<VoxelVertex>(buffer, [](VoxelVertex* data) {delete[]data; });
	c_vertexCount = count;
#if defined __DIRECTX__
	UpdateVertexBufferDesc();
#endif
}

#if defined __DIRECTX__
bool VoxelData::CreateBuffer(ID3D11Device* d3dDevice)
{
	ReleaseCOM(r_vertexBuffer);
	r_vertexBuffer = nullptr;

	auto result = d3dDevice->CreateBuffer(&c_bufferDesc, &c_subresourceData, &r_vertexBuffer);
	return (!ISFAILED(result));
}
void VoxelData::UpdateVertexBufferDesc()
{
	if (GetIsDynamicBuffer())
		c_bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	else
		c_bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	c_bufferDesc.ByteWidth = sizeof(VoxelVertex)*c_vertexCount;
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

void IceDogRendering::VoxelData::Close()
{
	ReleaseCOM(r_vertexBuffer);
	IceDogRendering::RenderDataBase::Close();
}

ID3D11Buffer* IceDogRendering::VoxelData::GetVertexBuffer()
{
	return r_vertexBuffer;
}

#endif