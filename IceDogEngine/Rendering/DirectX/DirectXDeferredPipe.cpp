
#include "../../Utils/Voxel/MarchingCubeLT.h"
#include "DirectXDeferredPipe.h"
#include "RenderingDef.h"

namespace IceDogRendering
{
	DirectXDeferredPipe::DirectXDeferredPipe(std::ostream& errOs) : RenderingPipe(errOs)
	{
		r_voxelInputLayout = 0;
		r_meshInputLayout = 0;
		r_deferredLightLayout = 0;

		c_msaaQuility = 0;
		c_backBufferHeight = 1024;
		c_backBufferWidth = 1024;
		c_swapChainBufferRefreshRate = 60;
		c_enableMsaa = false;
		r_backBufferDepthStencilView = nullptr;
		r_backBufferRenderTargetView = nullptr;
		r_backBufferDepthStencilBuffer = nullptr;
		r_gBufferNormal = nullptr;
		r_gBufferSpecularRoughnessMetallic = nullptr;
		r_gBufferFinalColor = nullptr;
		r_gBufferBaseColor = nullptr;

		r_gBufferNormalRTV = nullptr;
		r_gBufferBaseColorRTV = nullptr;
		r_gBufferSpecularRoughnessMetallicRTV = nullptr;
		r_gBufferFinalColorRTV = nullptr;

		r_gBufferBaseColorSRV = nullptr;
		r_gBufferFinalColorSRV = nullptr;
		r_gBufferNormalSRV = nullptr;
		r_gBufferSpecularRoughnessMetallicSRV = nullptr;
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
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R8G8B8A8_UNORM;
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
			s_errorlogOutStream << "Try Get dxgi factory failed" << std::endl;
		}

		IDXGIAdapter* dxgiAdapter = 0;
		if (ISFAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter)))
		{
			s_errorlogOutStream << "Try Get dxgi factory failed" << std::endl;
		}

		IDXGIFactory* dxgiFactory = 0;
		if (ISFAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory)))
		{
			s_errorlogOutStream << "Try Get dxgi factory failed" << std::endl;
		}

		if (ISFAILED(dxgiFactory->CreateSwapChain(c_PDRR.r_device, &swapDesc, &r_mainSwapChain)))
		{
			s_errorlogOutStream << "Create swap chain failed" << std::endl;
		}

		// release the temp com
		ReleaseCOM(dxgiDevice);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiFactory);

		// resize all the buffers
		Resize(c_backBufferWidth, c_backBufferHeight);

		if (!IceDogRendering::BuildFX("Rendering/DirectX/FX/deferred.fxo", c_PDRR.r_device, r_effectFX))
		{
			s_errorlogOutStream << "Create effect failed" << std::endl;
		}

		UpdateInputLayout();

		// init single vertex buffer
		r_dflVertex = new IceDogRendering::DeferredLightVertex();
		r_dflIndex = new UINT();
		*r_dflIndex = 0;

		D3D11_BUFFER_DESC bdes;
		bdes.Usage = D3D11_USAGE_IMMUTABLE;
		bdes.ByteWidth = sizeof(IceDogRendering::DeferredLightVertex);
		bdes.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bdes.CPUAccessFlags = 0;
		bdes.MiscFlags = 0;
		bdes.StructureByteStride = 0;
		r_dflVertexRS.pSysMem = r_dflVertex;
		if (ISFAILED(c_PDRR.r_device->CreateBuffer(&bdes, &r_dflVertexRS, &r_singleVertexBuffer)))
		{
			s_errorlogOutStream << "Create Light vertex buffer failed" << std::flush;
		}

		D3D11_BUFFER_DESC ides;
		ides.Usage = D3D11_USAGE_IMMUTABLE;
		ides.ByteWidth = sizeof(UINT);
		ides.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ides.CPUAccessFlags = 0;
		ides.MiscFlags = 0;
		ides.StructureByteStride = 0;
		r_dflIndexRS.pSysMem = r_dflIndex;
		if (ISFAILED(c_PDRR.r_device->CreateBuffer(&ides, &r_dflIndexRS, &r_singleIndexBuffer)))
		{
			s_errorlogOutStream << "Create Light index buffer failed" << std::flush;
		}

		// test code begin
		r_cubeMapSource.LoadFromFile(L"Source/Textures/cube.dds", c_PDRR);
		// test code end

		// build up the state for rendering
		BuildUpStates();
		CreateMarchingCubeLookupTable();
		PrePass();
	}

	void DirectXDeferredPipe::UpdateInputLayout()
	{
		ReleaseCOM(r_meshInputLayout);
		ReleaseCOM(r_voxelInputLayout);
		ReleaseCOM(r_deferredLightLayout);

		// create input layout
		auto tech = r_effectFX->GetTechniqueByName("Deferred");
		D3DX11_PASS_DESC meshpassDesc;
		tech->GetPassByName("GBufferStage")->GetDesc(&meshpassDesc);
		int inputCount = std::end(IceDogRendering::vertexDesc) - std::begin(IceDogRendering::vertexDesc);
		if (ISFAILED(c_PDRR.r_device->CreateInputLayout(IceDogRendering::vertexDesc, inputCount, meshpassDesc.pIAInputSignature, meshpassDesc.IAInputSignatureSize, &r_meshInputLayout)))
		{
			s_errorlogOutStream << "Create input layout failed" << std::endl;
		}

		D3DX11_PASS_DESC voxelpassDesc;
		tech->GetPassByName("VoxelStage")->GetDesc(&voxelpassDesc);
		inputCount = std::end(IceDogRendering::voxelVertexDesc) - std::begin(IceDogRendering::voxelVertexDesc);
		if (ISFAILED(c_PDRR.r_device->CreateInputLayout(IceDogRendering::voxelVertexDesc, inputCount, voxelpassDesc.pIAInputSignature, voxelpassDesc.IAInputSignatureSize, &r_voxelInputLayout)))
		{
			s_errorlogOutStream << "Create input layout failed" << std::endl;
		}

		D3DX11_PASS_DESC lpassDesc;
		tech->GetPassByName("LightingStage")->GetDesc(&lpassDesc);
		inputCount = std::end(IceDogRendering::deferredLightVertexDesc) - std::begin(IceDogRendering::deferredLightVertexDesc);
		if (ISFAILED(c_PDRR.r_device->CreateInputLayout(IceDogRendering::deferredLightVertexDesc, inputCount, lpassDesc.pIAInputSignature, lpassDesc.IAInputSignatureSize, &r_deferredLightLayout)))
		{
			s_errorlogOutStream << "Create input layout failed" << std::endl;
		}
	}

	void DirectXDeferredPipe::BuildUpStates()
	{
		// create the lighting blend enable state 
		D3D11_BLEND_DESC lightBlendDesc = { 0 };
		lightBlendDesc.AlphaToCoverageEnable = FALSE;
		lightBlendDesc.IndependentBlendEnable = FALSE;
		lightBlendDesc.RenderTarget[0].BlendEnable = TRUE;
		lightBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		lightBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		lightBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		lightBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		lightBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		lightBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		lightBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		c_PDRR.r_device->CreateBlendState(&lightBlendDesc, &r_lightBlendEnableState);

		// create the lighting blend disable state
		lightBlendDesc.RenderTarget[0].BlendEnable = FALSE;
		c_PDRR.r_device->CreateBlendState(&lightBlendDesc, &r_LightBlendDisableState);

		// create the depth test enable state
		D3D11_DEPTH_STENCIL_DESC depthDesc = { 0 };
		depthDesc.DepthEnable = TRUE;
		c_PDRR.r_device->CreateDepthStencilState(&depthDesc, &r_depthTestEnableState);

		// create the depth test disable state
		depthDesc.DepthEnable = FALSE;
		c_PDRR.r_device->CreateDepthStencilState(&depthDesc, &r_depthTestDisableState);
	}

	void DirectXDeferredPipe::EnableDepthTest()
	{
		c_PDRR.r_deviceContext->OMSetDepthStencilState(r_depthTestEnableState, 0);
	}

	void DirectXDeferredPipe::DisableDepthTest()
	{
		c_PDRR.r_deviceContext->OMSetDepthStencilState(r_depthTestDisableState, 0);
	}

	void DirectXDeferredPipe::EnableLightBlend()
	{
		const float blendFac[] = { 0,0,0,0 };
		c_PDRR.r_deviceContext->OMSetBlendState(r_lightBlendEnableState, blendFac, 0xffffffff);
	}

	void DirectXDeferredPipe::DisableLightBlend()
	{
		const float blendFac[] = { 0,0,0,0 };
		c_PDRR.r_deviceContext->OMSetBlendState(r_LightBlendDisableState, blendFac, 0xffffffff);
	}

	void DirectXDeferredPipe::CreateMarchingCubeLookupTable()
	{
		D3D11_TEXTURE1D_DESC tx_1dDesc;
		tx_1dDesc.Width = 256;
		tx_1dDesc.MipLevels = 1;
		tx_1dDesc.ArraySize = 1;
		tx_1dDesc.Format = DXGI_FORMAT_R16_SINT;
		tx_1dDesc.Usage = D3D11_USAGE_DEFAULT;
		tx_1dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		tx_1dDesc.CPUAccessFlags = 0;
		tx_1dDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA tx_1dSR;
		tx_1dSR.pSysMem = IceDogAlgorithm::edgeTable;

		ID3D11Texture1D* tx_1d;

		if (ISFAILED(c_PDRR.r_device->CreateTexture1D(&tx_1dDesc, &tx_1dSR, &tx_1d)))
		{
			s_errorlogOutStream << "Create Marching Cube Edge Table Failed!" << std::endl;
		}

		D3D11_TEXTURE2D_DESC tx_2dDesc;
		tx_2dDesc.Width = 16;
		tx_2dDesc.Height = 256;
		tx_2dDesc.MipLevels = 1;
		tx_2dDesc.ArraySize = 1;
		tx_2dDesc.Format = DXGI_FORMAT_R8_SINT;
		tx_2dDesc.SampleDesc.Count = 1;
		tx_2dDesc.SampleDesc.Quality = 0;
		tx_2dDesc.Usage = D3D11_USAGE_DEFAULT;
		tx_2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		tx_2dDesc.CPUAccessFlags = 0;
		tx_2dDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA tx_2dSR;
		tx_2dSR.pSysMem = IceDogAlgorithm::triTable;
		tx_2dSR.SysMemPitch = sizeof(INT8) * 16;
		ID3D11Texture2D* tx_2d;

		if (ISFAILED(c_PDRR.r_device->CreateTexture2D(&tx_2dDesc, &tx_2dSR, &tx_2d)))
		{
			s_errorlogOutStream << "Create Marching Cube Triangle Table Failed!" << std::endl;
		}

		if (ISFAILED(c_PDRR.r_device->CreateShaderResourceView(tx_1d, 0, &r_mcEdgeSRV)))
		{
			s_errorlogOutStream << "Create Marching Cube Edge Table SRV Failed!" << std::endl;
		}
		if (ISFAILED(c_PDRR.r_device->CreateShaderResourceView(tx_2d, 0, &r_mcTriangleSRV)))
		{
			s_errorlogOutStream << "Create Marching Cube Triangle Table SRV Failed!" << std::endl;
		}

		ReleaseCOM(tx_1d);
		ReleaseCOM(tx_2d);
	}

	void DirectXDeferredPipe::UpdateLightData()
	{
		// just test code

		if (r_defaultLG.GetDirectionalLight()[0]->IsDirty())
		{
			r_defaultLG.GetDirectionalLight()[0]->UpdateShadowTexture(c_PDRR);
		}
	}

	void DirectXDeferredPipe::ClearAllViews()
	{
		// clear the back buffer
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_backBufferRenderTargetView, IceDogRendering::Color::Black);
		// clear depth stencil view
		c_PDRR.r_deviceContext->ClearDepthStencilView(r_backBufferDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

		// clear other rt view
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_gBufferBaseColorRTV, IceDogRendering::Color::Black);
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_gBufferFinalColorRTV, IceDogRendering::Color::Black);
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_gBufferNormalRTV, IceDogRendering::Color::Black);
		c_PDRR.r_deviceContext->ClearRenderTargetView(r_gBufferSpecularRoughnessMetallicRTV, IceDogRendering::Color::NONECOLOR);
	}

	void DirectXDeferredPipe::SetupConstBuffer()
	{
		// set const buffer per frame
		r_effectFX->GetVariableByName("eyePos")->SetRawValue(&r_mainPipeView->GetEyePosition(), 0, sizeof(float3));
		r_effectFX->GetVariableByName("zNearFar")->SetRawValue(&r_mainPipeView->GetNearFarPlane(), 0, sizeof(float2));
		float3 lightOn = float3(0, 0, 0);
		// update each light
		if (r_defaultLG.HasDirectionalLightsSpace() != r_defaultLG.GetPerLightCount())
		{	
			r_effectFX->GetVariableByName("directionLight")->SetRawValue(r_defaultLG.GetDirectionalLight()[0].get()->GetLightDef().get(), 0, sizeof(DirectionalLight));
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

	void DirectXDeferredPipe::RenderShadowMap(std::vector<std::shared_ptr<RenderDataBase>>& renderDatas)
	{
		float shadow_map_size = r_defaultLG.GetDirectionalLight()[0]->GetShadowMapSize();
		c_PDRR.r_deviceContext->RSSetViewports(1, (D3D11_VIEWPORT*)&(r_defaultLG.GetDirectionalLight()[0]->GetViewport()));
		r_effectFX->GetVariableByName("dl_proj")->AsMatrix()->SetMatrix((r_defaultLG.GetDirectionalLight()[0]->GetProjectionMatrix()).m);
		r_effectFX->GetVariableByName("dl_view")->AsMatrix()->SetMatrix((r_defaultLG.GetDirectionalLight()[0]->GetViewMatrix()).m);
		r_effectFX->GetVariableByName("shadow_sample_size")->SetRawValue(&shadow_map_size, 0, sizeof(float));

		// get the pass first
		auto tech = r_effectFX->GetTechniqueByName("Deferred");
		auto pass_mesh = tech->GetPassByName("ShadowStage");
		auto pass_voxel = tech->GetPassByName("VoxelShadowStage");

		ID3D11RenderTargetView* renderTargets[] = { r_defaultLG.GetDirectionalLight()[0]->GetShadowMapRTV().GetRenderTargetView() };

		c_PDRR.r_deviceContext->OMSetRenderTargets(0, 0, 0);
		c_PDRR.r_deviceContext->OMSetRenderTargets(1, renderTargets, r_defaultLG.GetDirectionalLight()[0]->GetDepthStencilView().GetDepthStencilView());

		UINT stride_mesh = sizeof(IceDogRendering::Vertex);
		UINT stride_voxel = sizeof(IceDogRendering::VoxelVertex);
		UINT offset = 0;

		r_effectFX->GetVariableByName("mcEdgeTable")->AsShaderResource()->SetResource(r_mcEdgeSRV);
		r_effectFX->GetVariableByName("mcTriTable")->AsShaderResource()->SetResource(r_mcTriangleSRV);
		// render voxel
		c_PDRR.r_deviceContext->IASetInputLayout(r_voxelInputLayout);
		c_PDRR.r_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		float isoLevel = 0.5;
		for (auto renderData : renderDatas)
		{
			std::shared_ptr<VoxelData> rd = std::dynamic_pointer_cast<VoxelData>(renderData);
			if (!rd) { continue; }
			if (rd->GetVertexBuffer() == nullptr) { continue; }
			auto tempVertexBuffer = rd->GetVertexBuffer();
			isoLevel = rd->GetIsoLevel();

			c_PDRR.r_deviceContext->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride_voxel, &offset);

			r_effectFX->GetVariableByName("m_world")->AsMatrix()->SetMatrix(rd->GetWorldMatrix().m);
			r_effectFX->GetVariableByName("m_view")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewMatrix().m);
			r_effectFX->GetVariableByName("m_proj")->AsMatrix()->SetMatrix(r_mainPipeView->GetProjectionMatrix().m);
			r_effectFX->GetVariableByName("m_viewInv")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewInverse().m);
			r_effectFX->GetVariableByName("m_worldInverseTranspose")->AsMatrix()->SetMatrix(rd->GetWorldInverseTransposeMatrix().m);
			r_effectFX->GetVariableByName("isolevel")->SetRawValue(&isoLevel, 0, sizeof(float));
			// apply the material
			r_effectFX->GetVariableByName("DifNorParEmi")->SetRawValue(&float4(0, 0, 0, 0), 0, sizeof(IceDogRendering::float4));

			pass_voxel->Apply(0, c_PDRR.r_deviceContext);
			c_PDRR.r_deviceContext->Draw(rd->GetVertexCount(), 0);
		}

		// render mesh
		c_PDRR.r_deviceContext->IASetInputLayout(r_meshInputLayout);
		c_PDRR.r_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		for (auto renderData : renderDatas)
		{
			std::shared_ptr<MeshData> rd = std::dynamic_pointer_cast<MeshData>(renderData);
			if (!rd) { continue; }
			if (rd->GetIndexBuffer() == nullptr || rd->GetVertexBuffer() == nullptr) { continue; }
			auto tempVertexBuffer = rd->GetVertexBuffer();

			c_PDRR.r_deviceContext->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride_mesh, &offset);
			c_PDRR.r_deviceContext->IASetIndexBuffer(rd->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			r_effectFX->GetVariableByName("m_world")->AsMatrix()->SetMatrix(rd->GetWorldMatrix().m);
			r_effectFX->GetVariableByName("m_view")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewMatrix().m);
			r_effectFX->GetVariableByName("m_proj")->AsMatrix()->SetMatrix(r_mainPipeView->GetProjectionMatrix().m);
			r_effectFX->GetVariableByName("m_viewInv")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewInverse().m);
			r_effectFX->GetVariableByName("m_worldInverseTranspose")->AsMatrix()->SetMatrix(rd->GetWorldInverseTransposeMatrix().m);
			// apply the material
			r_effectFX->GetVariableByName("DifNorParEmi")->SetRawValue(&float4(0, 0, 0, 0), 0, sizeof(IceDogRendering::float4));

			pass_mesh->Apply(0, c_PDRR.r_deviceContext);

			c_PDRR.r_deviceContext->DrawIndexed(rd->GetTriangleCount() * 3, 0, 0);
		}
		// bind the shadow map texture to shader resource
		c_PDRR.r_deviceContext->OMSetRenderTargets(0, 0, 0);
		r_effectFX->GetVariableByName("directionalShadowMap")->AsShaderResource()->SetResource( r_defaultLG.GetDirectionalLight()[0]->GetShadowMapSRV().GetResourceView());

		// reset the view port
		c_PDRR.r_deviceContext->RSSetViewports(1, &r_viewPort);
	}

	void DirectXDeferredPipe::RenderGBuffer(std::vector<std::shared_ptr<RenderDataBase>>& renderDatas)
	{
		auto tech = r_effectFX->GetTechniqueByName("Deferred");
		auto pass_mesh = tech->GetPassByName("GBufferStage");
		auto pass_voxel = tech->GetPassByName("VoxelStage");

		ID3D11RenderTargetView* renderTargets[] = { r_gBufferNormalRTV,r_gBufferBaseColorRTV,r_gBufferSpecularRoughnessMetallicRTV };

		c_PDRR.r_deviceContext->OMSetRenderTargets(0, 0, 0);
		c_PDRR.r_deviceContext->OMSetRenderTargets(3, renderTargets, r_backBufferDepthStencilView);

		UINT stride_mesh = sizeof(IceDogRendering::Vertex);
		UINT stride_voxel = sizeof(IceDogRendering::VoxelVertex);
		UINT offset = 0;

		r_effectFX->GetVariableByName("mcEdgeTable")->AsShaderResource()->SetResource(r_mcEdgeSRV);
		r_effectFX->GetVariableByName("mcTriTable")->AsShaderResource()->SetResource(r_mcTriangleSRV);
		// render voxel
		c_PDRR.r_deviceContext->IASetInputLayout(r_voxelInputLayout);
		c_PDRR.r_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		float isoLevel = 0.5;
		for (auto renderData : renderDatas)
		{
			std::shared_ptr<VoxelData> rd = std::dynamic_pointer_cast<VoxelData>(renderData);
			if (!rd) { continue; }
			if (rd->GetVertexBuffer() == nullptr) { continue; }
			auto tempVertexBuffer = rd->GetVertexBuffer();
			isoLevel = rd->GetIsoLevel();

			c_PDRR.r_deviceContext->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride_voxel, &offset);

			r_effectFX->GetVariableByName("m_world")->AsMatrix()->SetMatrix(rd->GetWorldMatrix().m);
			r_effectFX->GetVariableByName("m_view")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewMatrix().m);
			r_effectFX->GetVariableByName("m_proj")->AsMatrix()->SetMatrix(r_mainPipeView->GetProjectionMatrix().m);
			r_effectFX->GetVariableByName("m_viewInv")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewInverse().m);
			r_effectFX->GetVariableByName("m_worldInverseTranspose")->AsMatrix()->SetMatrix(rd->GetWorldInverseTransposeMatrix().m);
			r_effectFX->GetVariableByName("isolevel")->SetRawValue(&isoLevel, 0, sizeof(float));
			// apply the material
			if (rd->GetMaterialData())
			{
				r_effectFX->GetVariableByName("DifNorParEmi")->SetRawValue(&rd->GetTextureEnableDesc(), 0, sizeof(IceDogRendering::float4));
				r_effectFX->GetVariableByName("diffuseMap")->AsShaderResource()->SetResource(rd->GetMaterialData()->GetDiffuseSRV());
				r_effectFX->GetVariableByName("normalMap")->AsShaderResource()->SetResource(rd->GetMaterialData()->GetNormalSRV());
				r_effectFX->GetVariableByName("parallaxMap")->AsShaderResource()->SetResource(rd->GetMaterialData()->GetParallaxSRV());
				r_effectFX->GetVariableByName("parallaxCfg")->SetRawValue(&rd->GetMaterialData()->GetParallaxCfg(), 0, sizeof(IceDogUtils::float4));
			}
			else
			{
				r_effectFX->GetVariableByName("DifNorParEmi")->SetRawValue(&float4(0, 0, 0, 0), 0, sizeof(IceDogRendering::float4));
			}

			pass_voxel->Apply(0, c_PDRR.r_deviceContext);
			c_PDRR.r_deviceContext->Draw(rd->GetVertexCount(), 0);
		}

		// render mesh
		c_PDRR.r_deviceContext->IASetInputLayout(r_meshInputLayout);
		c_PDRR.r_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		for (auto renderData : renderDatas)
		{
			std::shared_ptr<MeshData> rd = std::dynamic_pointer_cast<MeshData>(renderData);
			if (!rd) { continue; }
			if (rd->GetIndexBuffer() == nullptr || rd->GetVertexBuffer() == nullptr) { continue; }
			auto tempVertexBuffer = rd->GetVertexBuffer();

			c_PDRR.r_deviceContext->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride_mesh, &offset);
			c_PDRR.r_deviceContext->IASetIndexBuffer(rd->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			r_effectFX->GetVariableByName("m_world")->AsMatrix()->SetMatrix(rd->GetWorldMatrix().m);
			r_effectFX->GetVariableByName("m_view")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewMatrix().m);
			r_effectFX->GetVariableByName("m_proj")->AsMatrix()->SetMatrix(r_mainPipeView->GetProjectionMatrix().m);
			r_effectFX->GetVariableByName("m_viewInv")->AsMatrix()->SetMatrix(r_mainPipeView->GetViewInverse().m);
			r_effectFX->GetVariableByName("m_worldInverseTranspose")->AsMatrix()->SetMatrix(rd->GetWorldInverseTransposeMatrix().m);
			// apply the material
			if (rd->GetMaterialData())
			{
				r_effectFX->GetVariableByName("DifNorParEmi")->SetRawValue(&rd->GetTextureEnableDesc(), 0, sizeof(IceDogRendering::float4));
				r_effectFX->GetVariableByName("diffuseMap")->AsShaderResource()->SetResource(rd->GetMaterialData()->GetDiffuseSRV());
				r_effectFX->GetVariableByName("normalMap")->AsShaderResource()->SetResource(rd->GetMaterialData()->GetNormalSRV());
				r_effectFX->GetVariableByName("parallaxMap")->AsShaderResource()->SetResource(rd->GetMaterialData()->GetParallaxSRV());
				r_effectFX->GetVariableByName("parallaxCfg")->SetRawValue(&rd->GetMaterialData()->GetParallaxCfg(), 0, sizeof(IceDogUtils::float4));
			}
			else
			{
				r_effectFX->GetVariableByName("DifNorParEmi")->SetRawValue(&float4(0,0,0,0), 0, sizeof(IceDogRendering::float4));
			}
			
			pass_mesh->Apply(0, c_PDRR.r_deviceContext);

			c_PDRR.r_deviceContext->DrawIndexed(rd->GetTriangleCount() * 3, 0, 0);
		}
	}

	void DirectXDeferredPipe::RenderLightBuffer()
	{
		// get tech
		auto tech = r_effectFX->GetTechniqueByName("Deferred");
		auto pass = tech->GetPassByName("LightingStage");

		// clear depth stencil view
		c_PDRR.r_deviceContext->ClearDepthStencilView(r_backBufferDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

		UINT stride = sizeof(IceDogRendering::DeferredLightVertex);
		UINT offset = 0;
		// set input layout
		c_PDRR.r_deviceContext->IASetInputLayout(r_deferredLightLayout);
		c_PDRR.r_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		c_PDRR.r_deviceContext->OMSetRenderTargets(1, &r_backBufferRenderTargetView, r_backBufferDepthStencilView);
		c_PDRR.r_deviceContext->IASetVertexBuffers(0, 1, &r_singleVertexBuffer, &stride, &offset);
		c_PDRR.r_deviceContext->IASetIndexBuffer(r_singleIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// set g buffer as shader resource			
		r_effectFX->GetVariableByName("gBuffer_normal")->AsShaderResource()->SetResource(r_gBufferNormalSRV);
		r_effectFX->GetVariableByName("gBuffer_baseColor")->AsShaderResource()->SetResource(r_gBufferBaseColorSRV);
		r_effectFX->GetVariableByName("gBuffer_specularRoughnessMetallic")->AsShaderResource()->SetResource(r_gBufferSpecularRoughnessMetallicSRV);

		// set BRDF LUT
		r_effectFX->GetVariableByName("brdfLut")->AsShaderResource()->SetResource(r_brdfLutSRV);

		pass->Apply(0, c_PDRR.r_deviceContext);
		
		EnableLightBlend();
		DisableDepthTest();
		// loop for light drawing
		c_PDRR.r_deviceContext->DrawIndexed(1, 0, 0);
		EnableDepthTest();
		DisableLightBlend();

		// unbind g buffer shader resource
		r_effectFX->GetVariableByName("gBuffer_normal")->AsShaderResource()->SetResource(NULL);
		r_effectFX->GetVariableByName("gBuffer_baseColor")->AsShaderResource()->SetResource(NULL);
		r_effectFX->GetVariableByName("gBuffer_specularRoughnessMetallic")->AsShaderResource()->SetResource(NULL);
		r_effectFX->GetVariableByName("directionalShadowMap")->AsShaderResource()->SetResource(NULL);

		pass->Apply(0, c_PDRR.r_deviceContext);
	}

	void DirectXDeferredPipe::RenderBRDFLut()
	{
		// get tech
		auto tech = r_effectFX->GetTechniqueByName("Deferred");
		auto pass = tech->GetPassByName("BRDFLutStage");

		UINT stride = sizeof(IceDogRendering::DeferredLightVertex);
		UINT offset = 0;
		// set input layout
		c_PDRR.r_deviceContext->IASetInputLayout(r_deferredLightLayout);
		c_PDRR.r_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		c_PDRR.r_deviceContext->OMSetRenderTargets(1, &r_brdfLutRTV, r_backBufferDepthStencilView);
		c_PDRR.r_deviceContext->IASetVertexBuffers(0, 1, &r_singleVertexBuffer, &stride, &offset);
		c_PDRR.r_deviceContext->IASetIndexBuffer(r_singleIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		pass->Apply(0, c_PDRR.r_deviceContext);

		DisableDepthTest();
		// loop for light drawing
		c_PDRR.r_deviceContext->DrawIndexed(1, 0, 0);
		EnableDepthTest();

		pass->Apply(0, c_PDRR.r_deviceContext);
	}

	void DirectXDeferredPipe::PrePass()
	{
		CreateRenderTargetTexture2D(r_brdfLutBuffer, r_brdfLutRTV, r_brdfLutSRV, DXGI_FORMAT_R16G16B16A16_FLOAT);
		RenderBRDFLut();
	}

	void DirectXDeferredPipe::MergeOutput()
	{
		r_mainSwapChain->Present(0, 0);
	}

	void DirectXDeferredPipe::Render(std::vector<std::shared_ptr<RenderDataBase>>& renderDatas)
	{
		assert(c_PDRR.r_deviceContext);
		assert(r_mainSwapChain);

		// check if have a pipeView
		if (r_mainPipeView == nullptr) { return; }

		// render shadow map first
		UpdateLightData();
		RenderShadowMap(renderDatas);
		ClearAllViews();
		SetupConstBuffer();
		if (!r_cubeMapSource.IsDirty())
			r_effectFX->GetVariableByName("cubeMap")->AsShaderResource()->SetResource(r_cubeMapSource.GetCubeMapSRV().GetResourceView());
		RenderGBuffer(renderDatas);
		RenderLightBuffer();
		MergeOutput();
		r_defaultLG.GetDirectionalLight()[0]->CleanBuffer(c_PDRR);
		return;
	}

	void DirectXDeferredPipe::ReleaseAllBuffer()
	{
		// for recreate the back buffer render target view
		ReleaseCOM(r_backBufferRenderTargetView);
		ReleaseCOM(r_backBufferDepthStencilView);
		ReleaseCOM(r_backBufferDepthStencilBuffer);
		ReleaseCOM(r_gBufferBaseColor);
		ReleaseCOM(r_gBufferFinalColor);
		ReleaseCOM(r_gBufferNormal);
		ReleaseCOM(r_gBufferSpecularRoughnessMetallic);

		ReleaseCOM(r_gBufferNormalRTV);
		ReleaseCOM(r_gBufferBaseColorRTV);
		ReleaseCOM(r_gBufferSpecularRoughnessMetallicRTV);
		ReleaseCOM(r_gBufferFinalColorRTV);

		ReleaseCOM(r_gBufferBaseColorSRV);
		ReleaseCOM(r_gBufferFinalColorSRV);
		ReleaseCOM(r_gBufferNormalSRV);
		ReleaseCOM(r_gBufferSpecularRoughnessMetallicSRV);
	}

	void DirectXDeferredPipe::CreateRenderTargetTexture2D(ID3D11Texture2D*& texture, ID3D11RenderTargetView*& rt, ID3D11ShaderResourceView*& sr, DXGI_FORMAT format)
	{
		CreateRenderTargetTexture2D(c_backBufferWidth, c_backBufferHeight, texture, rt, sr, format);
	}

	void DirectXDeferredPipe::CreateRenderTargetTexture2D(UINT width, UINT height, ID3D11Texture2D*& texture, ID3D11RenderTargetView*& rt, ID3D11ShaderResourceView*& sr, DXGI_FORMAT format)
	{
		D3D11_TEXTURE2D_DESC dsd;
		dsd.Width = width;
		dsd.Height = height;
		dsd.MipLevels = 1;
		dsd.ArraySize = 1;
		dsd.Format = format;
		dsd.SampleDesc.Count = 1;
		dsd.SampleDesc.Quality = 0;
		dsd.Usage = D3D11_USAGE_DEFAULT;
		dsd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		dsd.CPUAccessFlags = 0;
		dsd.MiscFlags = 0;

		// create the texture 2d
		if (ISFAILED(c_PDRR.r_device->CreateTexture2D(&dsd, 0, &texture)))
		{
			s_errorlogOutStream << "Create render target texture failed" << std::endl;
			return;
		}

		// create the render target view
		if (ISFAILED(c_PDRR.r_device->CreateRenderTargetView(texture, 0, &rt)))
		{
			s_errorlogOutStream << "Create render target view failed" << std::flush;
			return;
		}

		// create the shader resource view
		if (ISFAILED(c_PDRR.r_device->CreateShaderResourceView(texture, 0, &sr)))
		{
			s_errorlogOutStream << "Create shader resource view failed" << std::flush;
			return;
		}
	}

	void DirectXDeferredPipe::ResizeSwapChain()
	{
		if (ISFAILED(r_mainSwapChain->ResizeBuffers(1, c_backBufferWidth, c_backBufferHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
		{
			s_errorlogOutStream << "Resize swap chain buffer failed" << std::flush;
			return;
		}
		// get the back buffer
		ID3D11Texture2D* tempBackBufferTexture;
		if (ISFAILED(r_mainSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&tempBackBufferTexture))))
		{
			s_errorlogOutStream << "Get swap chain back buffer failed" << std::flush;
			return;
		}
		// recreate the back buffer render target view
		if (ISFAILED(c_PDRR.r_device->CreateRenderTargetView(tempBackBufferTexture, 0, &r_backBufferRenderTargetView)))
		{
			s_errorlogOutStream << "Create render target view from back buffer texture failed" << std::flush;
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
			s_errorlogOutStream << "Create depth stencil buffer failed" << std::endl;
			return;
		}
		// create the depth stencil render target view
		if (ISFAILED(c_PDRR.r_device->CreateDepthStencilView(r_backBufferDepthStencilBuffer, 0, &r_backBufferDepthStencilView)))
		{
			s_errorlogOutStream << "Create depth stencil render target view failed" << std::endl;
			return;
		}

		CreateRenderTargetTexture2D(r_gBufferBaseColor, r_gBufferBaseColorRTV, r_gBufferBaseColorSRV, DXGI_FORMAT_R16G16B16A16_UNORM);
		CreateRenderTargetTexture2D(r_gBufferFinalColor, r_gBufferFinalColorRTV, r_gBufferFinalColorSRV, DXGI_FORMAT_R16G16B16A16_UNORM);
		CreateRenderTargetTexture2D(r_gBufferNormal, r_gBufferNormalRTV, r_gBufferNormalSRV, DXGI_FORMAT_R16G16B16A16_UNORM);
		CreateRenderTargetTexture2D(r_gBufferSpecularRoughnessMetallic, r_gBufferSpecularRoughnessMetallicRTV, r_gBufferSpecularRoughnessMetallicSRV,DXGI_FORMAT_R32G32B32A32_FLOAT);
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