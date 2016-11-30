#pragma once
#include "../../Utils/Common/UtilBasic.h"

#if defined __DIRECTX__

namespace IceDogRendering
{
	static bool BuildFX(std::string url, ID3D11Device* device, ID3DX11Effect*& fx)
	{
		DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		std::ifstream fin(url, std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		int size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		std::vector<char> compiledShader(size);

		fin.read(&compiledShader[0], size);
		fin.close();

		auto temp = D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, device, &fx);
		return !(((HRESULT)(temp)) < 0);
	};
}

#endif