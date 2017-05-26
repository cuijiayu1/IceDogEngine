#pragma once

#include "../../Utils/Common/UtilBasic.h"
#include "../MaterialData.h"
#include "../PDRenderingDef.h"

namespace IceDogRendering
{
	enum class DataMapDirtyFlag
	{
		None,
		IndexBuffer,
		VertexBuffer,
		Both
	};

	class RenderDataBase
	{
	public:
		RenderDataBase();
		virtual ~RenderDataBase();

		/* close the renderData */
		virtual void Close();

		/* create vertex buffer */
		virtual bool CreateBuffer(ID3D11Device* d3dDevice)=0;
		/* set the world matrix */
		void SetWorldMatrix(const float4x4& worldMatrix);
		/* get world matrix */
		float4x4 GetWorldMatrix() { return c_worldMatrix; }
		/* get the world inverse transpose matrix for lighting calculation */
		float4x4 GetWorldInverseTransposeMatrix() { return c_worldInverseTransposeMatrix; }
		/* set the material data */
		void SetMaterial(MaterialData* matData) { c_materilaData = matData; }
		/* get the texture enabled description for shader usage */
		IceDogRendering::float4 GetTextureEnableDesc() { return c_materilaData->GetTextureEnableDesc(); }
		/* get the material that this render data current hold */
		MaterialData* GetMaterialData() { return c_materilaData; }
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
		/* get vertex count */
		int GetVertexCount();

	protected:
		// the material info, just hold it ,has no authority to delete it
		MaterialData* c_materilaData;
		// this mark whether the data should bu update from the resource that this render data holds. this is quite different from the data clean
		DataMapDirtyFlag c_dataMapFlag = DataMapDirtyFlag::None;
		// the mark whether the data should be re create, if it`s dirty ,the system would release the old data and create a new buffer from the data that this render data hold
		bool c_isDataClean = false;
		// the world matrix of this render data
		float4x4 c_worldMatrix;
		// the world inverse transpose matrix
		float4x4 c_worldInverseTransposeMatrix;
		// the number of vertex
		int c_vertexCount = 0;
	};
}

