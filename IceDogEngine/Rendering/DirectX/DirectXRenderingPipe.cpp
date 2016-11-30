#include "DirectXRenderingPipe.h"
#include "RenderingDef.h"

using namespace IceDogRendering;

void DirectXRenderingPipe::Resize(int newWidth, int newHeight)
{
	c_backBufferWidth = newWidth;
	c_backBufferHeight = newHeight;

	// for recreate the back buffer rendertarget view
	ReleaseCOM(r_backBufferRenderTargetView);
	ReleaseCOM(r_backBufferDepthStencilView);
	ReleaseCOM(r_backBufferDepthStencilBuffer);

	if (ISFAILED(r_mainSwapChain->ResizeBuffers(1, newWidth, newHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
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

	// create the depth stencil view
	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width = c_backBufferWidth;
	dsd.Height = c_backBufferHeight;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// config when have msaa enable
	if (c_enableMsaa)
	{
		dsd.SampleDesc.Count = 4;
		dsd.SampleDesc.Quality = c_msaaQuility - 1;
	}
	else
	{
		dsd.SampleDesc.Count = 1;
		dsd.SampleDesc.Quality = 0;
	}
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

	// bind the depth /stencil buffer to swap chain
	c_PDRR.r_deviceContext->OMSetRenderTargets(1, &r_backBufferRenderTargetView, r_backBufferDepthStencilView);
	// set the view port
	r_viewPort.Height = c_backBufferHeight;
	r_viewPort.Width = c_backBufferWidth;
	r_viewPort.TopLeftX = 0;
	r_viewPort.TopLeftY = 0;
	r_viewPort.MinDepth = 0;
	r_viewPort.MaxDepth = 1;

	c_PDRR.r_deviceContext->RSSetViewports(1, &r_viewPort);
}

void DirectXRenderingPipe::Render()
{
	assert(c_PDRR.r_deviceContext);
	assert(r_mainSwapChain);

	// clear the back buffer
	c_PDRR.r_deviceContext->ClearRenderTargetView(r_backBufferRenderTargetView,IceDogRendering::Color::Blue);
	// clear depth stencil view
	c_PDRR.r_deviceContext->ClearDepthStencilView(r_backBufferDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	c_PDRR.r_deviceContext->IASetInputLayout(r_inputLayout);
	c_PDRR.r_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// draw call

	D3DX11_TECHNIQUE_DESC techDesc;
	auto tech = r_effectFX->GetTechniqueByName("ColorTech");
	tech->GetDesc(&techDesc);

	UINT stride = sizeof(IceDogRendering::Vertex);
	UINT offset = 0;
	for (auto rd : r_renderDatas)
	{
		auto tempVertexBuffer = rd->GetVertexBuffer();

		c_PDRR.r_deviceContext->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride, &offset);
		c_PDRR.r_deviceContext->IASetIndexBuffer(rd->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		r_effectFX->GetVariableByName("m_world")->AsMatrix()->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&XMFLOAT4X4(rd->GetWorldMatrix().m))));
		r_effectFX->GetVariableByName("m_view")->AsMatrix()->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&XMFLOAT4X4(r_mainPipeView->GetViewMatrix().m))));
		r_effectFX->GetVariableByName("m_proj")->AsMatrix()->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&XMFLOAT4X4(r_mainPipeView->GetProjectionMatrix().m))));

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			tech->GetPassByIndex(p)->Apply(0, c_PDRR.r_deviceContext);
			c_PDRR.r_deviceContext->DrawIndexed(rd->GetTriangleCount() * 3, 0, 0);
		}
	}

	// present
	if (ISFAILED(r_mainSwapChain->Present(0, 0)))
	{
		s_errorlogOutStream << "Present Faild" << std::endl;
	}
	
}

void DirectXRenderingPipe::RegistRenderData(std::shared_ptr<RenderData> rd)
{
	r_renderDatas.push_back(rd);
	auto result = rd->CreateVertexBufferWithIndexBuffer(c_PDRR.r_device);
	if (!result) { s_errorlogOutStream << "Create vertex buffer faild!" << std::endl; }
}

void DirectXRenderingPipe::InitPipe(IceDogPlatform::PlatformWindow pfWindow)
{
	c_platformWindow = pfWindow;

	r_mainPipeView = std::make_shared<PipeView>((float)pfWindow.width / (float)pfWindow.height);

	// check if support 4x msaa
	if (ISFAILED(c_PDRR.r_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &c_msaaQuility)))
	{
		s_errorlogOutStream << "Current device has no 4x msaa support" << std::flush;
	}
	assert(c_msaaQuility > 0);

	// begin construct swap chain
	DXGI_SWAP_CHAIN_DESC swapDesc;
	swapDesc.BufferDesc.Width = c_platformWindow.width;
	swapDesc.BufferDesc.Height = c_platformWindow.height;
	swapDesc.BufferDesc.RefreshRate.Numerator = c_swapChainBufferRefreshRate;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (c_enableMsaa)
	{
		// when enable mass, set the sample count to 4
		swapDesc.SampleDesc.Count = 4;
		swapDesc.SampleDesc.Quality = c_msaaQuility - 1;
	}
	else
	{
		// when no enable just sample count 1
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
	}

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

	// resize the swap chain
	Resize(c_backBufferWidth, c_backBufferHeight);

	if (!IceDogRendering::BuildFX("Rendering/DirectX/FX/basic.fxo",c_PDRR.r_device, r_effectFX))
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