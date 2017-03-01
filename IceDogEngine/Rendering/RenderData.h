#pragma once

#include "../Utils/Common/UtilBasic.h"
#include "MaterialData.h"
#include "PDRenderingDef.h"

namespace IceDogRendering
{
	enum class DataMapDirtyFlag
	{
		None,
		IndexBuffer,
		VertexBuffer,
		Both
	};

	class RenderData
	{
	public:
		RenderData();
		~RenderData();
		/* get world matrix */
		float4x4 GetWorldMatrix() { return c_worldMatrix; }
		/* get the world inverse transpose matrix for lighting calculation */
		float4x4 GetWorldInverseTransposeMatrix() { return c_worldInverseTransposeMatrix; }
		/* get the material def of this obj */
		Material GetMaterial() { return c_material; }
		/* set the world matrix */
		void SetWorldMatrix(const float4x4& worldMatrix) { c_worldMatrix = worldMatrix; }
		/* set the vertex data */
		void SetVertexData(Vertex* buffer, int count);
		/* set the index data */
		void SetIndexData(unsigned int* indexBuffer, int triangleCount);
		/* get the vertex data */
		std::shared_ptr<Vertex> GetVertexData() { return c_vertexDatas; }
		/* get the index data */
		std::shared_ptr<unsigned int> GetIndexData() { return c_indexDatas; }
		/* set the material data */
		void SetMaterial(MaterialData* matData) { c_materilaData = matData; }
		/* get the diffuse shader resource view */
		ID3D11ShaderResourceView* GetDiffuseSRV();

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
		/* get whether the data was clean or not */
		bool GetDataIsClean();
		/* get whether the data map was clean */
		DataMapDirtyFlag GetDataMapFlag();
		/* mark the data state clean so will no be create buffer again */
		void MarkDataStateClean();
		/* mark the data to dirty, so when the rendering manager find it will re create it */
		void MarkDataStateDirty();
		/* mark the data map state clean so will no be update */
		void MarkDataMapStateClean();
		/* get whether is Dynamic Buffer or not */
		virtual bool GetIsDynamicBuffer();

	private:
#if defined __DIRECTX__
		/* call for update the vertex buffer desc,usually called after buffer change */
		void UpdateVertexBufferDesc();
		/* call for update the index buffer desc,usually called after index buffer change */
		void UpdateIndexBufferDesc();
#endif

	protected:
		// the material info
		MaterialData* c_materilaData;
		Material c_material;
		// this mark whether the data should bu update from the resource that this render data holds. this is quite different from the data clean
		DataMapDirtyFlag c_dataMapFlag = DataMapDirtyFlag::None;
		// the mark whether the data should be re create, if it`s dirty ,the system would release the old data and create a new buffer from the data that this render data hold
		bool c_isDataClean = false;
		// the world matrix of this render data
		float4x4 c_worldMatrix;
		// the world inverse transpose matrix
		float4x4 c_worldInverseTransposeMatrix;
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

