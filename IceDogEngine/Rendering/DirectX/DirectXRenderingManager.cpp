#include "DirectXRenderingManager.h"

using namespace IceDogRendering;

void DirectXRenderingManager::SetDriverType(RenderDriverType type)
{
	if (type == RenderDriverType::hardware)
	{
		c_driverType = D3D_DRIVER_TYPE_HARDWARE;
	}
	else
	{
		c_driverType = D3D_DRIVER_TYPE_SOFTWARE;
	}
}

PlatformDependenceRenderResource DirectXRenderingManager::GetPDRR()
{
	PlatformDependenceRenderResource pdrr;
	pdrr.r_device = r_device;
	pdrr.r_deviceContext = r_deviceContext;
	return pdrr;
}

void DirectXRenderingManager::UpdateRenderDataIndexBuffer(std::shared_ptr<IceDogRendering::RenderDataBase> renderData)
{
	std::shared_ptr<IceDogRendering::MeshData> rd = std::dynamic_pointer_cast<IceDogRendering::MeshData>(renderData);
	if (!rd) { return; }
	// unmap the data and get the ptr
	D3D11_MAPPED_SUBRESOURCE mappedData;
	if (ISFAILED(r_deviceContext->Map(rd->GetIndexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData)))
	{
		s_errorlogOutStream << "Try map index buffer failed" << std::endl;
		return;
	}
	unsigned int* indexBuffer = reinterpret_cast<unsigned int*>(mappedData.pData);
	auto data = rd->GetIndexData();
	// update the data with data inside the rd
	for (int i = 0; i < rd->GetTriangleCount()*3; i++)
	{
		indexBuffer[i] = data.get()[i];
	}
	r_deviceContext->Unmap(rd->GetIndexBuffer(), 0);
}

void DirectXRenderingManager::UpdateRenderDataVertexBuffer(std::shared_ptr<IceDogRendering::RenderDataBase> renderData)
{
	if (std::dynamic_pointer_cast<IceDogRendering::MeshData>(renderData))
	{
		std::shared_ptr<IceDogRendering::MeshData> rd = std::dynamic_pointer_cast<IceDogRendering::MeshData>(renderData);
		// unmap the data and get the ptr
		D3D11_MAPPED_SUBRESOURCE mappedData;
		if (ISFAILED(r_deviceContext->Map(rd->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData)))
		{
			s_errorlogOutStream << "Try map vertex buffer failed" << std::endl;
			return;
		}
		Vertex* vertexBuffer = reinterpret_cast<Vertex*>(mappedData.pData);
		auto data = rd->GetVertexData();
		// update the data with data inside the rd
		for (int i = 0; i < rd->GetVertexCount(); i++)
		{
			vertexBuffer[i] = data.get()[i];
		}
		r_deviceContext->Unmap(rd->GetVertexBuffer(), 0);
		return;
	}

	if (std::dynamic_pointer_cast<IceDogRendering::VoxelData>(renderData))
	{
		std::shared_ptr<IceDogRendering::VoxelData> rd = std::dynamic_pointer_cast<IceDogRendering::VoxelData>(renderData);
		// unmap the data and get the ptr
		D3D11_MAPPED_SUBRESOURCE mappedData;
		if (ISFAILED(r_deviceContext->Map(rd->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData)))
		{
			s_errorlogOutStream << "Try map vertex buffer failed" << std::endl;
			return;
		}
		VoxelVertex* vertexBuffer = reinterpret_cast<VoxelVertex*>(mappedData.pData);
		auto data = rd->GetVertexData();
		// update the data with data inside the rd
		for (int i = 0; i < rd->GetVertexCount(); i++)
		{
			vertexBuffer[i] = data.get()[i];
		}
		r_deviceContext->Unmap(rd->GetVertexBuffer(), 0);
		return;
	}
}

void DirectXRenderingManager::TickRenderingManager()
{
	// check and create the render data buffer
	for (auto rd:r_sceneRenderData)
		if (!rd->GetDataIsClean()) { rd->CreateBuffer(r_device); rd->MarkDataStateClean(); }
	for (auto rd:r_uiRenderData)
		if (!rd->GetDataIsClean()) { rd->CreateBuffer(r_device); rd->MarkDataStateClean(); }
	
	for (auto rd : r_sceneRenderData)
	{
		if (rd->GetDataMapFlag()==DataMapDirtyFlag::None) { continue; }
		if (rd->GetDataMapFlag() == DataMapDirtyFlag::IndexBuffer)
			UpdateRenderDataIndexBuffer(rd);
		if (rd->GetDataMapFlag() == DataMapDirtyFlag::VertexBuffer)
			UpdateRenderDataVertexBuffer(rd);
		if (rd->GetDataMapFlag() == DataMapDirtyFlag::Both) { UpdateRenderDataIndexBuffer(rd); UpdateRenderDataVertexBuffer(rd); }
		rd->MarkDataMapStateClean();
	}

	for (auto rd : r_uiRenderData)
	{
		if (rd->GetDataMapFlag() == DataMapDirtyFlag::None) { continue; }
		if (rd->GetDataMapFlag() == DataMapDirtyFlag::IndexBuffer)
			UpdateRenderDataIndexBuffer(rd);
		if (rd->GetDataMapFlag() == DataMapDirtyFlag::VertexBuffer)
			UpdateRenderDataVertexBuffer(rd);
		if (rd->GetDataMapFlag() == DataMapDirtyFlag::Both) { UpdateRenderDataIndexBuffer(rd); UpdateRenderDataVertexBuffer(rd); }
		rd->MarkDataMapStateClean();
	}


	if (r_scenePipe != nullptr)
	{
		r_scenePipe->Render(r_sceneRenderData);
	}
	if (r_uiPipe != nullptr)
	{
		r_uiPipe->Render(r_uiRenderData);
	}
}

bool DirectXRenderingManager::InitRenderManager(IceDogPlatform::PlatformWindow pfWindow)
{
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,
		c_driverType,
		0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&r_device, &featureLevel, &r_deviceContext);

	// check if create device success
	if (ISFAILED(hr))
	{
		s_errorlogOutStream << "FatalError: Create device failed" << std::flush;
		return false;
	}

	// check if feature level==11 currently only support D3D11
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		s_errorlogOutStream << "FatalError: Current hardware support feature level is:" << featureLevel << " no supported" << std::flush;
		return false;
	}

	// init the shader manager
	r_shaderManager->Init(GetPDRR());

	return true;
}

/* register the ui pipe render data */
void DirectXRenderingManager::RegistUIRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd)
{
	r_uiRenderData.push_back(rd);
}

/* register the scene pipe render data */
void DirectXRenderingManager::RegistSceneRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd)
{
	r_sceneRenderData.push_back(rd);
}

void IceDogRendering::DirectXRenderingManager::Close()
{
	ReleaseCOM(r_device);
	ReleaseCOM(r_deviceContext);
	RenderingManager::Close();
}

void IceDogRendering::DirectXRenderingManager::UnRegistUIRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd)
{
	r_uiRenderData.erase(std::find(r_uiRenderData.begin(), r_uiRenderData.end(), rd));
}

void IceDogRendering::DirectXRenderingManager::UnRegistSceneRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd)
{
	r_sceneRenderData.erase(std::find(r_sceneRenderData.begin(), r_sceneRenderData.end(), rd));
}

void IceDogRendering::DirectXRenderingManager::RegistMainPipeView(std::shared_ptr<IceDogRendering::PipeView> pv)
{
	r_scenePipe->RegistMainPipeView(pv);
}

void IceDogRendering::DirectXRenderingManager::RegistLightData(std::shared_ptr<IceDogRendering::LightBase> ld, LightType ltp)
{
	r_scenePipe->RegistLightData(ld, ltp);
}

void IceDogRendering::DirectXRenderingManager::UnRegistLightData(std::shared_ptr<IceDogRendering::LightBase> ld, LightType ltp)
{
	r_scenePipe->UnRegistLightData(ld, ltp);
}
