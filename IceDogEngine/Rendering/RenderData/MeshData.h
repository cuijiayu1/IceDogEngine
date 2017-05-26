#pragma once

#include "RenderDataBase.h"

namespace IceDogRendering
{
	class MeshData:public RenderDataBase
	{
	public:
		MeshData();
		virtual ~MeshData();
		/* close the mesh data */
		virtual void Close() override;
		/* set the vertex data */
		void SetVertexData(Vertex* buffer, int count);
		/* set the index data */
		void SetIndexData(unsigned int* indexBuffer, int triangleCount);
		/* get the vertex data */
		std::shared_ptr<Vertex> GetVertexData() { return c_vertexDatas; }
		/* get the index data */
		std::shared_ptr<unsigned int> GetIndexData() { return c_indexDatas; }

#if defined __DIRECTX__
		/* get the buffer desc */
		D3D11_BUFFER_DESC GetBufferDesc() { return c_bufferDesc; }
		/* get the sub resource data */
		D3D11_SUBRESOURCE_DATA GetSubResource() { return c_subresourceData; }
		/* create the buffer */
		bool CreateBuffer(ID3D11Device* d3dDevice) override;
		/* get vertex buffer */
		ID3D11Buffer* GetVertexBuffer();
		/* get index buffer */
		ID3D11Buffer* GetIndexBuffer();
#endif
		/* get triangle count */
		int GetTriangleCount();

	private:
#if defined __DIRECTX__
		/* call for update the vertex buffer desc,usually called after buffer change */
		void UpdateVertexBufferDesc();
		/* call for update the index buffer desc,usually called after index buffer change */
		void UpdateIndexBufferDesc();
#endif

	protected:
		// hold the vertex data in memory
		std::shared_ptr<Vertex> c_vertexDatas;
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

