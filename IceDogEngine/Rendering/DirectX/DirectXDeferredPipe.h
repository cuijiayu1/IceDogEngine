#pragma once

#include "../RenderingPipe.h"
#include "../LightData/DirectionLightData.h"

namespace IceDogRendering
{
	class RenderingManager;

	class DirectXDeferredPipe :public RenderingPipe
	{
	public:
		DirectXDeferredPipe(std::ostream& errOs);

		/* init the pipe line */
		void InitPipe(IceDogPlatform::PlatformWindow pfWindow, ShaderManager* shaderManager) override;

		/* call the pipe to render */
		void Render(std::vector<std::shared_ptr<RenderDataBase>>& renderDatas) override;

		/* call close to release the resource */
		virtual void Close() override;

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
		void CreateRenderTargetTexture2D(ID3D11Texture2D*& texture, ID3D11RenderTargetView*& rt,ID3D11ShaderResourceView*& sr, DXGI_FORMAT format);
		void CreateRenderTargetTexture2D(UINT width, UINT height, ID3D11Texture2D*& texture, ID3D11RenderTargetView*& rt, ID3D11ShaderResourceView*& sr, DXGI_FORMAT format);

		/* resize all buffer */
		void UpdateAllBuffer();

		/* clear all views */
		void ClearAllViews();

		/* render the G-Buffer */
		void RenderGBuffer(std::vector<std::shared_ptr<RenderDataBase>>& renderDatas);

		/* render direct light, need geo for shadow map calculation */
		void RenderDirectLight(std::vector<std::shared_ptr<RenderDataBase>>& renderDatas);

		/* render single direct light */
		void RenderSingleDirectLight();

		/* render single shadow map */
		void RenderSingleShadowMap(std::shared_ptr<LightBase> light, std::vector<std::shared_ptr<RenderDataBase>>& renderDatas);

		/* render environment with direct lighting */
		void RenderEnvWithDirLight();

		/* render the lighting result to quarter size for post process */
		void RenderToQuarter();

		/* update all lights */
		void UpdateAllLights();

		/* BRDF LUT generate Pass */
		void RenderBRDFLut();

		/* Pre Pass, the pass go when init */
		void PrePass();

		/* merge the buffer and output */
		void MergeOutput();

		/* update the input layout */
		void UpdateInputLayout();

		/* the function to build up states for rendering */
		void BuildUpStates();

		/* enable the depth test, to be noticed that depth test is enabled by default, not necessary called unless there is a disable call */
		void EnableDepthTest();

		/* disable the depth test */
		void DisableDepthTest();

		/* enable the specific blend mode for lighting */
		void EnableLightBlend();

		/* disable the specific blend mode for lighting */
		void DisableLightBlend();

		/* create the marching cube look up table */
		void CreateMarchingCubeLookupTable();

		/* create the input layout of the pass */
		void CreateInputLayout(ID3DX11Effect* effect, std::string technique, std::string stage, int descCount, const D3D11_INPUT_ELEMENT_DESC* desc, ID3D11InputLayout*& inputLayout);
		

	private:
		IDXGISwapChain* r_mainSwapChain;
		// g pass
		ID3D11InputLayout* r_meshInputLayout;
		ID3D11InputLayout* r_voxelInputLayout;
		ID3D11InputLayout* r_deferredLightLayout;
		// g pass

		// l pass
		ID3D11InputLayout* r_lvoxelInputLayout;
		ID3D11InputLayout* r_lvertexShadowInputLayout;
		ID3D11InputLayout* r_lvertexLightInputLayout;
		// l pass

		D3D11_VIEWPORT r_viewPort;
		ID3DX11Effect* r_effectFX;
		ID3DX11Effect* r_lightFX;

		ID3D11Buffer* r_singleVertexBuffer;
		ID3D11Buffer* r_singleIndexBuffer;
		IceDogRendering::DeferredLightVertex* r_dflVertex;
		UINT* r_dflIndex;
		D3D11_SUBRESOURCE_DATA r_dflVertexRS;
		D3D11_SUBRESOURCE_DATA r_dflIndexRS;

		//states
		ID3D11BlendState* r_lightBlendEnableState;
		ID3D11BlendState* r_LightBlendDisableState;

		ID3D11DepthStencilState* r_depthTestEnableState;
		ID3D11DepthStencilState* r_depthTestDisableState;

		//views

		// RTV

		ID3D11RenderTargetView* r_backBufferRenderTargetView;
		ID3D11DepthStencilView* r_backBufferDepthStencilView;

		ID3D11RenderTargetView* r_gBufferNormalRTV;
		ID3D11RenderTargetView* r_gBufferBaseColorRTV;
		ID3D11RenderTargetView* r_gBufferSpecularRoughnessMetallicRTV;
		ID3D11RenderTargetView* r_gBufferFinalColorRTV;
		ID3D11RenderTargetView* r_finalColorQuarterRTV;
		ID3D11RenderTargetView* r_lBufferDirectLightRTV;

		ID3D11RenderTargetView* r_brdfLutRTV;

		// SRV

		ID3D11ShaderResourceView* r_gBufferNormalSRV;
		ID3D11ShaderResourceView* r_gBufferBaseColorSRV;
		ID3D11ShaderResourceView* r_gBufferSpecularRoughnessMetallicSRV;
		ID3D11ShaderResourceView* r_gBufferFinalColorSRV;
		ID3D11ShaderResourceView* r_finalColorQuarterSRV;
		ID3D11ShaderResourceView* r_lBufferDirectLightSRV;

		ID3D11ShaderResourceView* r_mcEdgeSRV;
		ID3D11ShaderResourceView* r_mcTriangleSRV;

		ID3D11ShaderResourceView* r_brdfLutSRV;

		//other buffer
		ID3D11Texture2D* r_backBufferDepthStencilBuffer;

		//G-Buffer (Geometry stage out)
		ID3D11Texture2D* r_gBufferNormal;
		ID3D11Texture2D* r_gBufferBaseColor;
		ID3D11Texture2D* r_gBufferSpecularRoughnessMetallic;

		//Light-Buffer 
		ID3D11Texture2D* r_lBufferDirectLightBuffer;

		//Final color buffer (combine direct lighting with ambient lighting)
		ID3D11Texture2D* r_gBufferFinalColor;

		//Post process scene buffer
		ID3D11Texture2D* r_finalColorQuarter;

		//BRDF LUT (BRDF GGX LUT out)
		ID3D11Texture2D* r_brdfLutBuffer;
	};
}