#pragma once

#include "../RenderingPipe.h"

namespace IceDogRendering
{
	class DirectXDeferredPipe :public RenderingPipe
	{
	public:
		DirectXDeferredPipe(std::ostream& errOs);

		/* init the pipe line */
		void InitPipe(IceDogPlatform::PlatformWindow pfWindow) override;

		/* call the pipe to render */
		void Render(std::vector<std::shared_ptr<RenderData>>& renderDatas) override;

	protected:
		/* reset the render target size */
		void Resize(int newWidth, int newHeight) override;

	private:
		/* setup the const buffer for cal */
		void SetupConstBuffer();

		/* resize the swap chain */
		void ResizeSwapChain();

		/* release the render matter resource */
		void ReleaseAllBuffer();

		/* create g buffer texture */
		void CreateRenderTargetTexture2D(ID3D11Texture2D*& texture, ID3D11RenderTargetView*& rt,ID3D11ShaderResourceView*& sr);

		/* resize all buffer */
		void UpdateAllBuffer();

		/* clear all views */
		void ClearAllViews();

		/* render the G-Buffer */
		void RenderGBuffer(std::vector<std::shared_ptr<RenderData>>& renderDatas);

		/* render the light buffer */
		void RenderLightBuffer();

		/* merge the buffer and output */
		void MergeOutput();
		

	private:
		IDXGISwapChain* r_mainSwapChain;
		ID3D11InputLayout* r_inputLayout;
		D3D11_VIEWPORT r_viewPort;
		ID3DX11Effect* r_effectFX;

		//views
		ID3D11RenderTargetView* r_backBufferRenderTargetView;
		ID3D11DepthStencilView* r_backBufferDepthStencilView;

		ID3D11RenderTargetView* r_gBufferNormalRTV;
		ID3D11RenderTargetView* r_gBufferDiffuseRTV;
		ID3D11RenderTargetView* r_gBufferSpecularRTV;
		ID3D11RenderTargetView* r_gBufferDepthRTV;
		ID3D11RenderTargetView* r_gBufferFinalColorRTV;

		ID3D11ShaderResourceView* r_gBufferNormalSRV;
		ID3D11ShaderResourceView* r_gBufferDiffuseSRV;
		ID3D11ShaderResourceView* r_gBufferSpecularSRV;
		ID3D11ShaderResourceView* r_gBufferDepthSRV;
		ID3D11ShaderResourceView* r_gBufferFinalColorSRV;

		//other buffer
		ID3D11Texture2D* r_backBufferDepthStencilBuffer;

		//G-Buffer (Geometry stage out)
		ID3D11Texture2D* r_gBufferNormal;
		ID3D11Texture2D* r_gBufferDiffuse;
		ID3D11Texture2D* r_gBufferSpecular;
		ID3D11Texture2D* r_gBufferDepth;

		//Final color buffer (light stage out)
		ID3D11Texture2D* r_gBufferFinalColor;
	};
}