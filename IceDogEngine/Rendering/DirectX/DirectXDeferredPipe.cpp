
#include "DirectXDeferredPipe.h"
#include "RenderingDef.h"

namespace IceDogRendering
{

	DirectXDeferredPipe::DirectXDeferredPipe(std::ostream& errOs) : RenderingPipe(errOs)
	{
		c_msaaQuility = 0;
		c_backBufferHeight = 600;
		c_backBufferWidth = 800;
		c_swapChainBufferRefreshRate = 60;
		c_enableMsaa = false;
		r_backBufferDepthStencilView = nullptr;
		r_backBufferRenderTargetView = nullptr;
		r_backBufferDepthStencilBuffer = nullptr;
		r_gBufferNormal = nullptr;
		r_gBufferSpecular = nullptr;
		r_gBufferFinalColor = nullptr;
		r_gBufferDiffuse = nullptr;
		r_gBufferDepth = nullptr;

		r_gBufferNormalRTV = nullptr;
		r_gBufferDiffuseRTV = nullptr;
		r_gBufferSpecularRTV = nullptr;
		r_gBufferDepthRTV = nullptr;
		r_gBufferFinalColorRTV = nullptr;

		r_gBufferDepthSRV = nullptr;
		r_gBufferDiffuseSRV = nullptr;
		r_gBufferFinalColorSRV = nullptr;
		r_gBufferNormalSRV = nullptr;
		r_gBufferSpecularSRV = nullptr;
	}

	void DirectXDeferredPipe::InitPipe(IceDogPlatform::PlatformWindow pfWindow)
	{
		c_platformWindow = pfWindow;

		// begin construct swap chain
		DXGI_SWAP_CHAIN_DESC swapDesc;
		swapDesc.BufferDesc.Width = c_platformWindow.width;
		swapDesc.BufferDesc.Height = c_platformWindow.height;
		swapDesc.BufferDesc.RefreshRate.Numerator = c_swapChainBufferRefreshRate;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;
		swapDesc.OutputWindow = c_platformWindow.c_window;
		swapDesc.Windowed = true;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.Flags = 0;

		IDXGIDevice* dxgiDevice = 0;
		if (ISFAILED(c_PDRR.r_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)))
		{
			s_errorlogOutStream << "Try Get dxgi factory faild" << std::endl;
		}

		IDXGIAdapter* dxgiAdapter = 0;
		if (ISFAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter)))
		{
			s_errorlogOutStream << "Try Get dxgi factory faild" << std::endl;
		}

		IDXGIFactory* dxgiFactory = 0;
		if (ISFAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory)))
		{
			s_errorlogOutStream << "Try Get dxgi factory faild" << std::endl;
		}

		if (ISFAILED(dxgiFactory->CreateSwapChain(c_PDRR.r_device, &swapDesc, &r_mainSwapChain)))
		{
			s_errorlogOutStream << "Create swap chain faild" << std::endl;
		}

		// release the temp com
		ReleaseCOM(dxgiDevice);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiFactory);

		// resize all the buffers
		Resize(c_backBufferWidth, c_backBufferHeight);

		if (!IceDogRendering::BuildFX("Rendering/DirectX/FX/deferred.fxo", c_PDRR.r_device, r_effectFX))
		{
			s_errorlogOutStream << "Create effect faild" << std::endl;
		}
		// create input layout
		auto tech = r_effectFX->GetTechniqueByName("ColorTech");
		D3DX11_PASS_DESC passDesc;
		tech->GetPassByIndex(0)->GetDesc(&passDesc);
		int inputCount = std::end(IceDogRendering::vertexDesc) - std::begin(IceDogRendering::vertexDesc);
		if (ISFAILED(c_PDRR.r_device->CreateInputLayout(IceDogRendering::vertexDesc, inputCount, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &r_inputLayout)))
		{
			s_errorlogOutStream << "Create input layout faild" << std::endl;
		}
	}

	void DirectXDeferredPipe::ClearAllViews()
	{
		// clear the back buffer
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_backBufferRenderTargetView, IceDogRendering::Color::Blue);
		// clear depth stencil view
		c_PDRR.r_deviceContext->ClearDepthStencilView(r_backBufferDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

		// clear other rt view
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_gBufferDepthRTV, IceDogRendering::Color::Black);
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_gBufferDiffuseRTV, IceDogRendering::Color::Black);
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_gBufferFinalColorRTV, IceDogRendering::Color::Black);
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_gBufferNormalRTV, IceDogRendering::Color::Black);
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_gBufferSpecularRTV, IceDogRendering::Color::Black);
	}

	void DirectXDeferredPipe::SetupConstBuffer()
	{
		// set const buffer per frame
		r_effectFX->GetVariableByName("eyePos")->SetRawValue(&r_mainPipeView->GetEyePosition(), 0, sizeof(float3));
		float3 lightOn = float3(0, 0, 0);
		// update each light
		if (r_defaultLG.HasDirectionalLightsSpace() != r_defaultLG.GetPerLightCount())
		{
			r_effectFX->GetVariableByName("directionLight")->SetRawValue(&(r_defaultLG.GetDirectionalLight()[0]), 0, sizeof(DirectionalLight));
			lightOn.x = 1;
		}
		if (r_defaultLG.HasSpotLightsSpace() != r_defaultLG.GetPerLightCount())
		{
			r_effectFX->GetVariableByName("spotLight")->SetRawValue(&(r_defaultLG.GetSpotLight()[0]), 0, sizeof(SpotLight));
			lightOn.y = 1;
		}
		if (r_defaultLG.HasPointLightsSpace() != r_defaultLG.GetPerLightCount())
		{
			r_effectFX->GetVariableByName("pointLight")->SetRawValue(&(r_defaultLG.GetPointLight()[0]), 0, sizeof(PointLight));
			lightOn.z = 1;
		}
		// update light on state
		r_effectFX->GetVariableByName("lightOn")->SetRawValue(&lightOn, 0, sizeof(float3));
	}

	void DirectXDeferredPipe::RenderGBuffer(std::vector<std::shared_ptr<RenderData>>& renderDatas)
	{
		//ID3D11RenderTargetView* renderTargets[] = { r_gBufferDepthRTV,r_gBufferDiffuseRTV,r_gBufferNormalRTV,r_gBufferSpecularRTV };

		c_PDRR.r_deviceContext->OMSetRenderTargets(1, &r_backBufferRenderTargetView, r_backBufferDepthStencilView);

		UINT stride = sizeof(IceDogRendering::Vertex);
		UINT offset = 0;

		D3DX11_TECHNIQUE_DESC techDesc;
		auto tech = r_effectFX->GetTechniqueByName("ColorTech");
		auto pass = tech->GetPassByName("gBuffer");
		pass->Apply(0, c_PDRR.r_deviceContext);

		for (auto rd : renderDatas)
		{
			if (rd->GetIndexBuffer() == nullptr || rd->GetVertexBuffer() == nullptr) { continue; }
			auto tempVertexBuffer = rd->GetVertexBuffer();

			c_PDRR.r_deviceContext->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride, &offset);
			c_PDRR.r_deviceContext->IASetIndexBuffer(rd->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			r_effectFX->GetVariableByName("m_world")->AsMatrix()->SetMatrix(rd->GetWorldMatrix().m);
			r_effectFX->GetVariableByName("m_view")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewMatrix().m);
			r_effectFX->GetVariableByName("m_proj")->AsMatrix()->SetMatrix(r_mainPipeView->GetProjectionMatrix().m);
			r_effectFX->GetVariableByName("m_worldInverseTranspose")->AsMatrix()->SetMatrix(rd->GetWorldInverseTransposeMatrix().m);
			r_effectFX->GetVariableByName("m_mat")->SetRawValue(&rd->GetMaterial(), 0, sizeof(Material));

			c_PDRR.r_deviceContext->DrawIndexed(rd->GetTriangleCount() * 3, 0, 0);
		}

		r_mainSwapChain->Present(0, 0);

		static bool fuck = true;
		if (fuck)
		{
			DirectX::ScratchImage srcImg;
			CaptureTexture(c_PDRR.r_device, c_PDRR.r_deviceContext, r_gBufferFinalColor, srcImg);
			if (ISFAILED(SaveToTGAFile(*srcImg.GetImage(0, 0, 0), L"wocao.tga")))
			{
				std::cout << "FUCK!" << std::endl;
			}
		}
		fuck = false;
	}

	void DirectXDeferredPipe::RenderLightBuffer()
	{

	}

	void DirectXDeferredPipe::MergeOutput()
	{

	}

	void DirectXDeferredPipe::Render(std::vector<std::shared_ptr<RenderData>>& renderDatas)
	{
		assert(c_PDRR.r_deviceContext);
		assert(r_mainSwapChain);

		// check if have a pipeView
		if (r_mainPipeView == nullptr) { return; }

		ClearAllViews();

		c_PDRR.r_deviceContext->IASetInputLayout(r_inputLayout);
		c_PDRR.r_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3DX11_TECHNIQUE_DESC techDesc;
		auto tech = r_effectFX->GetTechniqueByName("ColorTech");
		tech->GetDesc(&techDesc);

		SetupConstBuffer();
		RenderGBuffer(renderDatas);
		RenderLightBuffer();
		MergeOutput();
		return;

		
		

		UINT stride = sizeof(IceDogRendering::Vertex);
		UINT offset = 0;
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			tech->GetPassByIndex(p)->Apply(0, c_PDRR.r_deviceContext);
			for (auto rd : renderDatas)
			{
				if (rd->GetIndexBuffer() == nullptr || rd->GetVertexBuffer() == nullptr) { continue; }
				auto tempVertexBuffer = rd->GetVertexBuffer();

				c_PDRR.r_deviceContext->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride, &offset);
				c_PDRR.r_deviceContext->IASetIndexBuffer(rd->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

				r_effectFX->GetVariableByName("m_world")->AsMatrix()->SetMatrix(rd->GetWorldMatrix().m);
				r_effectFX->GetVariableByName("m_view")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewMatrix().m);
				r_effectFX->GetVariableByName("m_proj")->AsMatrix()->SetMatrix(r_mainPipeView->GetProjectionMatrix().m);
				r_effectFX->GetVariableByName("m_worldInverseTranspose")->AsMatrix()->SetMatrix(rd->GetWorldInverseTransposeMatrix().m);
				r_effectFX->GetVariableByName("m_mat")->SetRawValue(&rd->GetMaterial(), 0, sizeof(Material));

				c_PDRR.r_deviceContext->DrawIndexed(rd->GetTriangleCount() * 3, 0, 0);
			}
		}

		// present
		if (ISFAILED(r_mainSwapChain->Present(0, 0)))
		{
			s_errorlogOutStream << "Present Faild" << std::endl;
		}

	}

	void DirectXDeferredPipe::ReleaseAllBuffer()
	{
		// for recreate the back buffer rendertarget view
		ReleaseCOM(r_backBufferRenderTargetView);
		ReleaseCOM(r_backBufferDepthStencilView);
		ReleaseCOM(r_backBufferDepthStencilBuffer);
		ReleaseCOM(r_gBufferDepth);
		ReleaseCOM(r_gBufferDiffuse);
		ReleaseCOM(r_gBufferFinalColor);
		ReleaseCOM(r_gBufferNormal);
		ReleaseCOM(r_gBufferSpecular);

		ReleaseCOM(r_gBufferNormalRTV);
		ReleaseCOM(r_gBufferDiffuseRTV);
		ReleaseCOM(r_gBufferSpecularRTV);
		ReleaseCOM(r_gBufferDepthRTV);
		ReleaseCOM(r_gBufferFinalColorRTV);

		ReleaseCOM(r_gBufferDepthSRV);
		ReleaseCOM(r_gBufferDiffuseSRV);
		ReleaseCOM(r_gBufferFinalColorSRV);
		ReleaseCOM(r_gBufferNormalSRV);
		ReleaseCOM(r_gBufferSpecularSRV);
	}

	void DirectXDeferredPipe::CreateRenderTargetTexture2D(ID3D11Texture2D*& texture, ID3D11RenderTargetView*& rt, ID3D11ShaderResourceView*& sr)
	{
		D3D11_TEXTURE2D_DESC dsd;
		dsd.Width = c_backBufferWidth;
		dsd.Height = c_backBufferHeight;
		dsd.MipLevels = 1;
		dsd.ArraySize = 1;
		dsd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dsd.SampleDesc.Count = 1;
		dsd.SampleDesc.Quality = 0;
		dsd.Usage = D3D11_USAGE_DEFAULT;
		dsd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		dsd.CPUAccessFlags = 0;
		dsd.MiscFlags = 0;

		// create the texture 2d
		if (ISFAILED(c_PDRR.r_device->CreateTexture2D(&dsd, 0, &texture)))
		{
			s_errorlogOutStream << "Create render target texture faild" << std::endl;
			return;
		}

		// create the render target view
		if (ISFAILED(c_PDRR.r_device->CreateRenderTargetView(texture, 0, &rt)))
		{
			s_errorlogOutStream << "Create render target view faild" << std::flush;
			return;
		}

		// create the shader resource view
		if (ISFAILED(c_PDRR.r_device->CreateShaderResourceView(texture, 0, &sr)))
		{
			s_errorlogOutStream << "Create shader resource view faild" << std::flush;
			return;
		}
	}

	void DirectXDeferredPipe::ResizeSwapChain()
	{
		if (ISFAILED(r_mainSwapChain->ResizeBuffers(1, c_backBufferWidth, c_backBufferHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
		{
			s_errorlogOutStream << "Resize swap chain buffer faild" << std::flush;
			return;
		}
		// get the back buffer
		ID3D11Texture2D* tempBackBufferTexture;
		if (ISFAILED(r_mainSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&tempBackBufferTexture))))
		{
			s_errorlogOutStream << "Get swap chain back buffer faild" << std::flush;
			return;
		}
		// recreate the back buffer render target view
		if (ISFAILED(c_PDRR.r_device->CreateRenderTargetView(tempBackBufferTexture, 0, &r_backBufferRenderTargetView)))
		{
			s_errorlogOutStream << "Create render target view from back buffer texture faild" << std::flush;
			return;
		}
		ReleaseCOM(tempBackBufferTexture);
	}

	void DirectXDeferredPipe::UpdateAllBuffer()
	{
		ReleaseAllBuffer();
		// create the depth stencil view
		D3D11_TEXTURE2D_DESC dsd;
		dsd.Width = c_backBufferWidth;
		dsd.Height = c_backBufferHeight;
		dsd.MipLevels = 1;
		dsd.ArraySize = 1;
		dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsd.SampleDesc.Count = 1;
		dsd.SampleDesc.Quality = 0;
		dsd.Usage = D3D11_USAGE_DEFAULT;
		dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsd.CPUAccessFlags = 0;
		dsd.MiscFlags = 0;

		// create the depth stencil buffer
		if (ISFAILED(c_PDRR.r_device->CreateTexture2D(&dsd, 0, &r_backBufferDepthStencilBuffer)))
		{
			s_errorlogOutStream << "Create depth stencil buffer faild" << std::endl;
			return;
		}
		// create the depth stencil render target view
		if (ISFAILED(c_PDRR.r_device->CreateDepthStencilView(r_backBufferDepthStencilBuffer, 0, &r_backBufferDepthStencilView)))
		{
			s_errorlogOutStream << "Create depth stencil render target view faild" << std::endl;
			return;
		}

		CreateRenderTargetTexture2D(r_gBufferDepth, r_gBufferDepthRTV, r_gBufferDepthSRV);
		CreateRenderTargetTexture2D(r_gBufferDiffuse, r_gBufferDiffuseRTV, r_gBufferDiffuseSRV);
		CreateRenderTargetTexture2D(r_gBufferFinalColor, r_gBufferFinalColorRTV, r_gBufferFinalColorSRV);
		CreateRenderTargetTexture2D(r_gBufferNormal, r_gBufferNormalRTV, r_gBufferNormalSRV);
		CreateRenderTargetTexture2D(r_gBufferSpecular, r_gBufferSpecularRTV, r_gBufferSpecularSRV);
	}

	void DirectXDeferredPipe::Resize(int newWidth, int newHeight)
	{
		c_backBufferWidth = newWidth;
		c_backBufferHeight = newHeight;

		UpdateAllBuffer();

		ResizeSwapChain();

		// set the view port
		r_viewPort.Height = c_backBufferHeight;
		r_viewPort.Width = c_backBufferWidth;
		r_viewPort.TopLeftX = 0;
		r_viewPort.TopLeftY = 0;
		r_viewPort.MinDepth = 0;
		r_viewPort.MaxDepth = 1;

		c_PDRR.r_deviceContext->RSSetViewports(1, &r_viewPort);
	}
}