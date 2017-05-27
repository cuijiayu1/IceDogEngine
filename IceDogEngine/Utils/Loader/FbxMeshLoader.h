#pragma once

#include "../Common/UtilBasic.h"
#include "../../Rendering/RenderData/MeshData.h"

namespace IceDogUtils
{
	class FbxMeshLoader
	{
	public:
		bool LoadFbx2Mesh(std::string url, std::shared_ptr<IceDogRendering::MeshData> rd);
	};
}

