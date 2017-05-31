#include "DirectXShaderInstance.h"

using namespace IceDogRendering;

DirectXShaderInstance::DirectXShaderInstance(std::string url, ShaderType st):ShaderInstance(url, st)
{
	r_deviceShaderPtr = nullptr;
	r_constantBuffer[0] = nullptr;
	r_constantBuffer[1] = nullptr;
}

DirectXShaderInstance::~DirectXShaderInstance()
{
	ReleaseCOM(r_deviceShaderPtr);
	ReleaseCOM(r_constantBuffer[0]);
	ReleaseCOM(r_constantBuffer[1]);
}

void DirectXShaderInstance::Init(PlatformDependenceRenderResource pdrr, std::string x86_compiler, std::string x64_compiler)
{
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);

	r_pdrr = pdrr;
	// compile the shader
	// construct the compile command
	std::cout << "Compile Shader:" << c_url << std::endl;
	std::string compile_cmd;
	std::string compile_mode = "";
	std::string compile_type = "";
	std::string compile_tag = c_url;
	compile_tag = compile_tag.replace(compile_tag.find(".hlsl"), std::string(".hlsl").size(), ".cso");
#ifdef _WIN32
	compile_cmd = x86_compiler + " ";
#endif
#ifdef _WIN64
	compile_cmd = x64_compiler + " ";
#endif 
#ifdef _DEBUG
	compile_mode = "/Od /Zi ";
#endif
	compile_mode += "/Zpr ";
	std::cout << "Using Compiler:" << compile_cmd << std::endl;
	
	switch (c_shaderType)
	{
	case IceDogRendering::ShaderType::PS:
		compile_type = "ps_5_0 ";
		break;
	case IceDogRendering::ShaderType::VS:
		compile_type = "vs_5_0 ";
		break;
	case IceDogRendering::ShaderType::CS:
		compile_type = "cs_5_0 ";
		break;
	case IceDogRendering::ShaderType::GS:
		compile_type = "gs_5_0 ";
		break;
	}

	compile_cmd += compile_mode;
	compile_cmd += "/T ";
	compile_cmd += compile_type;
	compile_cmd += "/Fo ";
	compile_cmd += compile_tag + " ";
	compile_cmd += c_url;

	// compile the shader
	if (system(compile_cmd.c_str()))
	{
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "Compile Failed!" << std::endl;
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
		return;
	}
	std::cout << std::endl;

	// read the shader
	r_shaderCode = IceDogUtils::ReadAllBytes(compile_tag.c_str());

	bool failed_flag = false;
	// create the shader
	switch (c_shaderType)
	{
	case IceDogRendering::ShaderType::PS:
		ID3D11PixelShader* ps_ptr;
		if (ISFAILED(pdrr.r_device->CreatePixelShader(r_shaderCode.data(), r_shaderCode.size(), NULL, &ps_ptr)))
		{
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			std::cout << "Create Pixel Shader Failed!" << std::endl;
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
			failed_flag = true;
		}
		r_deviceShaderPtr = ps_ptr;
		break;
	case IceDogRendering::ShaderType::VS:
		ID3D11VertexShader* vs_ptr;
		if (ISFAILED(pdrr.r_device->CreateVertexShader(r_shaderCode.data(), r_shaderCode.size(), NULL, &vs_ptr)))
		{
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			std::cout << "Create Vertex Shader Failed!" << std::endl;
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
			failed_flag = true;
		}
		r_deviceShaderPtr = vs_ptr;
		break;
	case IceDogRendering::ShaderType::CS:
		ID3D11ComputeShader* cs_ptr;
		if (ISFAILED(pdrr.r_device->CreateComputeShader(r_shaderCode.data(), r_shaderCode.size(), NULL, &cs_ptr)))
		{
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			std::cout << "Create Compute Shader Failed!" << std::endl;
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
			failed_flag = true;
		}
		r_deviceShaderPtr = cs_ptr;
		break;
	case IceDogRendering::ShaderType::GS:
		ID3D11GeometryShader* gs_ptr;
		if (ISFAILED(pdrr.r_device->CreateGeometryShader(r_shaderCode.data(), r_shaderCode.size(), NULL, &gs_ptr)))
		{
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			std::cout << "Create Geometry Shader Failed!" << std::endl;
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
			failed_flag = true;
		}
		r_deviceShaderPtr = gs_ptr;
		break;
	}
	if (failed_flag)
	{
		throw "Create Shader Failed!";
	}
}

void IceDogRendering::DirectXShaderInstance::Close()
{
	ReleaseCOM(r_deviceShaderPtr);
	ReleaseCOM(r_constantBuffer[0]);
	ReleaseCOM(r_constantBuffer[1]);
	ShaderInstance::Close();
}

void* IceDogRendering::DirectXShaderInstance::GetRawShaderPtr()
{
	return r_deviceShaderPtr;
}

void IceDogRendering::DirectXShaderInstance::UpdateApplyBuffer(int cm)
{
	if (r_constantBuffer[cm])
		UpdateCB(cm);
	else
		CreateBuffer(cm);
	switch (c_shaderType)
	{
	case IceDogRendering::ShaderType::PS:
		r_pdrr.r_deviceContext->PSSetConstantBuffers(cm, 1, &r_constantBuffer[cm]);
		break;
	case IceDogRendering::ShaderType::VS:
		r_pdrr.r_deviceContext->VSSetConstantBuffers(cm, 1, &r_constantBuffer[cm]);
		break;
	case IceDogRendering::ShaderType::CS:
		r_pdrr.r_deviceContext->CSSetConstantBuffers(cm, 1, &r_constantBuffer[cm]);
		break;
	case IceDogRendering::ShaderType::GS:
		r_pdrr.r_deviceContext->GSSetConstantBuffers(cm, 1, &r_constantBuffer[cm]);
		break;
	}
}

void IceDogRendering::DirectXShaderInstance::ApplyShader()
{
	switch (c_shaderType)
	{
	case IceDogRendering::ShaderType::PS:
		r_pdrr.r_deviceContext->PSSetShader((ID3D11PixelShader*)r_deviceShaderPtr, 0, 0);
		break;
	case IceDogRendering::ShaderType::VS:
		r_pdrr.r_deviceContext->VSSetShader((ID3D11VertexShader*)r_deviceShaderPtr, 0, 0);
		break;
	case IceDogRendering::ShaderType::CS:
		r_pdrr.r_deviceContext->CSSetShader((ID3D11ComputeShader*)r_deviceShaderPtr, 0, 0);
		break;
	case IceDogRendering::ShaderType::GS:
		r_pdrr.r_deviceContext->GSSetShader((ID3D11GeometryShader*)r_deviceShaderPtr, 0, 0);
		break;
	}
}

void IceDogRendering::DirectXShaderInstance::UnloadShader()
{
	switch (c_shaderType)
	{
	case IceDogRendering::ShaderType::PS:
		r_pdrr.r_deviceContext->PSSetShader(nullptr, 0, 0);
		break;
	case IceDogRendering::ShaderType::VS:
		r_pdrr.r_deviceContext->VSSetShader(nullptr, 0, 0);
		break;
	case IceDogRendering::ShaderType::CS:
		r_pdrr.r_deviceContext->CSSetShader(nullptr, 0, 0);
		break;
	case IceDogRendering::ShaderType::GS:
		r_pdrr.r_deviceContext->GSSetShader(nullptr, 0, 0);
		break;
	}
}

void IceDogRendering::DirectXShaderInstance::SetViriable(const std::string& alias, void* ptr, const int cm)
{
	r_aliasPtrMap[cm][alias] = ptr;
}

bool IceDogRendering::DirectXShaderInstance::HasConstantBuffer()
{
	return r_orderAlias[0].size() + r_orderAlias[1].size();
}

void* IceDogRendering::DirectXShaderInstance::GetBufferRawPtr(int cm)
{
	return r_constantBuffer[cm];
}

void IceDogRendering::DirectXShaderInstance::CreateBuffer(int cm)
{
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);

	size_t bufferSize = c_bufferSize[cm];

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	void* datas= GetDataCope(cm);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = datas;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	
	if (ISFAILED(r_pdrr.r_device->CreateBuffer(&desc, &InitData, &r_constantBuffer[cm])))
	{
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "Create Constant Buffer Failed!" << std::endl;
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
	}

	delete[] datas;
}

void IceDogRendering::DirectXShaderInstance::UpdateCB(int cm)
{
	void* datas = GetDataCope(cm);

	r_pdrr.r_deviceContext->UpdateSubresource(r_constantBuffer[cm], 0, 0, datas, 0, 0);

	delete[] datas;
}

void* IceDogRendering::DirectXShaderInstance::GetDataCope(int cm)
{
	byte* datas = reinterpret_cast<byte*>(malloc(c_bufferSize[cm]));
	byte* tempPtr = datas;

	size_t array_size = r_orderAlias[cm].size();
	size_t size_inc = r_orderSize[cm][0];
	for (size_t i=0;i<array_size;++i)
	{
		size_inc = r_orderSize[cm][i];
		memcpy(tempPtr, r_aliasPtrMap[cm][r_orderAlias[cm][i]], size_inc);
		tempPtr += size_inc;
	}
	return datas;
}
