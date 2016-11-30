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

void DirectXRenderingManager::TickRenderingManager()
{
	if (r_scenePipe != nullptr)
	{
		r_scenePipe->Render();
	}
	if (r_uiPipe != nullptr)
	{
		r_uiPipe->Render();
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
		s_errorlogOutStream << "FatalError: Create device faild" << std::flush;
		return false;
	}

	// check if feature level==11 currently only support D3D11
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		s_errorlogOutStream << "FatalError: Current hardware support feature level is:" << featureLevel << " no supported" << std::flush;
		return false;
	}
	return true;
}

/* regist the ui pipe render data */
void DirectXRenderingManager::RegistUIPipeRenderData(std::shared_ptr<IceDogRendering::RenderData> rd)
{
	if (r_uiPipe != nullptr) 
	{
		r_uiPipe->RegistRenderData(rd);
	}
}

/* regist the scene pipe render data */
void DirectXRenderingManager::RegistScenePipeRenderData(std::shared_ptr<IceDogRendering::RenderData> rd)
{
	if (r_scenePipe != nullptr)
	{
		r_scenePipe->RegistRenderData(rd);
	}
}