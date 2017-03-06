#pragma once

#include "RenderDataBase.h"

namespace IceDogRendering
{
	class VoxelData:public RenderDataBase
	{
	public:
		VoxelData();
		virtual ~VoxelData();

#if defined __DIRECTX__
		/* create buffer */
		virtual bool CreateBuffer(ID3D11Device* d3dDevice) override;
		/* get vertex buffer */
		ID3D11Buffer* GetVertexBuffer();
#endif
		/* set the vertex data */
		void SetVertexData(VoxelVertex* buffer, int count);
		/* get the vertex data */
		std::shared_ptr<VoxelVertex> GetVertexData() { return c_vertexDatas; }
		/* set the iso level */
		void SetIsoLevel(float isolevel);
		/* get the iso level */
		float GetIsoLevel() { return c_isoLevel; }

	private:
#if defined __DIRECTX__
		/* update the vertex buffer description for voxelVertex data buffer creation */
		void UpdateVertexBufferDesc();
#endif

	private:
		// hold the vertex data in memory
		std::shared_ptr<VoxelVertex> c_vertexDatas;
		// the isolevel used when render this voxel object
		float c_isoLevel;
#if defined __DIRECTX__
		// the voxel vertex buffer desc
		D3D11_BUFFER_DESC c_bufferDesc;
		// the vertex sub buffer for this data
		D3D11_SUBRESOURCE_DATA c_subresourceData;
		// the vertex buffer
		ID3D11Buffer* r_vertexBuffer;
#endif
		
	};
}