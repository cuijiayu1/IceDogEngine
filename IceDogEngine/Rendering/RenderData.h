#pragma once

#include "../Utils/Common/UtilBasic.h"
#include "PDRenderingDef.h"

namespace IceDogRendering
{
	class RenderData
	{
	public:
		RenderData();

		/* get world matrix */
		float4x4 GetWorldMatrix() { return c_worldMatrix; }
		/* set the vertex data */
		void SetVertexData(Vertex* buffer, int count);
		/* set the index data */
		void SetIndexData(unsigned int* indexBuffer, int triangleCount);

#if defined __DIRECTX__
		/* get the buffer desc */
		D3D11_BUFFER_DESC GetBufferDesc() { return c_bufferDesc; }
		/* get the sub resource data */
		D3D11_SUBRESOURCE_DATA GetSubResource() { return c_subresourceData; }
		/* create vertex buffer */
		bool CreateVertexBufferWithIndexBuffer(ID3D11Device* d3dDevice);
		/* get vertex buffer */
		ID3D11Buffer* GetVertexBuffer();
		/* get index buffer */
		ID3D11Buffer* GetIndexBuffer();
#endif
		/* get vertex count */
		int GetVertexCount();
		/* get triangle count */
		int GetTriangleCount();

	private:
#if defined __DIRECTX__
		/* call for update the vertex buffer desc,usually called after buffer change */
		void UpdateVertexBufferDesc();
		/* call for update the index buffer desc,usually called after index buffer change */
		void UpdateIndexBufferDesc();
#endif

	private:
		// the world matrix of this render data
		float4x4 c_worldMatrix;
		// hold the vertex data in memory
		std::shared_ptr<Vertex> c_vertexDatas;
		int c_vertexCount = 0;
#if defined __DIRECTX__
		// the vertex buffer description for this data
		D3D11_BUFFER_DESC c_bufferDesc;
		// the vertex sub buffer for this data
		D3D11_SUBRESOURCE_DATA c_subresourceData;
		// the vertex buffer
		ID3D11Buffer* r_vertexBuffer;
#endif

		// index buffer hold the index
		std::shared_ptr<unsigned int> c_indexDatas;
		int c_triangleCount = 0;
#if defined __DIRECTX__
		// the index buffer description for this data
		D3D11_BUFFER_DESC c_indexBufferDesc;
		// the index sub buffer  for this data
		D3D11_SUBRESOURCE_DATA c_indexSubResourceData;
		// the index buffer
		ID3D11Buffer* r_indexBuffer;
#endif
	};
}

