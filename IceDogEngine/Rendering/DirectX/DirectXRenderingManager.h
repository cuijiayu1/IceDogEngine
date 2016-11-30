#pragma once

#include "../RenderingManager.h"

namespace IceDogRendering
{
	class DirectXRenderingManager :public RenderingManager
	{
	public:
		DirectXRenderingManager(std::ostream& logOs):RenderingManager(logOs),
			c_driverType(D3D_DRIVER_TYPE_HARDWARE)
		{}
		/* init the render manager */
		bool InitRenderManager(IceDogPlatform::PlatformWindow pfWindow) override;

		/* set the driver type */
		void SetDriverType(RenderDriverType type) override;

		/* get the platform dependence resource */
		PlatformDependenceRenderResource GetPDRR() override;

		/* tick */
		void TickRenderingManager() override;

		/* regist the ui pipe render data */
		void RegistUIRenderData(std::shared_ptr<IceDogRendering::RenderData> rd) override;

		/* regist the scene pipe render data */
		void RegistSceneRenderData(std::shared_ptr<IceDogRendering::RenderData> rd) override;

	private:
		/* update the RenderData index buffer */
		void UpdateRenderDataIndexBuffer(std::shared_ptr<IceDogRendering::RenderData> rd);

		/* update the RenderData vertex buffer */
		void UpdateRenderDataVertexBuffer(std::shared_ptr<IceDogRendering::RenderData> rd);

	private:
		D3D_DRIVER_TYPE c_driverType; //c--> config
		ID3D11Device* r_device; //r --> resource
		ID3D11DeviceContext* r_deviceContext;
	};
}

