#pragma once
#include "../RenderingPipe.h"

namespace IceDogRendering
{
	class DirectXRenderingPipe : public RenderingPipe
	{
	public:
		DirectXRenderingPipe(std::ostream& errOs) 
			: RenderingPipe(errOs)
		{
			c_msaaQuility = 0;
			c_backBufferHeight = 600;
			c_backBufferWidth = 800;
			c_swapChainBufferRefreshRate = 60;
			c_enableMsaa = true;
			r_backBufferDepthStencilView = nullptr;
			r_backBufferRenderTargetView = nullptr;
			r_backBufferDepthStencilBuffer = nullptr;
		}

		/* init the pipe line */
		void InitPipe(IceDogPlatform::PlatformWindow pfWindow) override;

		/* call the pipe to render */
		void Render() override;

		/* regist the render data */
		void RegistRenderData(std::shared_ptr<RenderData> rd) override;

	protected:
		/* reset the render target size */
		void Resize(int newWidth, int newHeight) override;
		
	private:
		IDXGISwapChain* r_mainSwapChain;
		ID3D11RenderTargetView* r_backBufferRenderTargetView;
		ID3D11DepthStencilView* r_backBufferDepthStencilView;
		ID3D11Texture2D* r_backBufferDepthStencilBuffer;
		ID3D11InputLayout* r_inputLayout;
		D3D11_VIEWPORT r_viewPort;
		ID3DX11Effect* r_effectFX;
	};
}

