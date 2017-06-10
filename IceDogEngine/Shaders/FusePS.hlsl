#include "Samplers.hlsli"

Texture2D lightColorOut;

Texture2D hehe : register(t10);

struct LightPSOut
{
	float4 finalColor : SV_Target;
};

struct LightGSOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 ACESToneMapping(float4 color, float exp)
{
	const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;
	color *= exp;
	return (color * (A * color + B)) / (color * (C * color + D) + E);
}

LightPSOut main(LightGSOut vout)
{
	LightPSOut result;
	//result.finalColor = lightColorOut.Sample(samAnisotropic, vout.uv);
	//result.finalColor = ACESToneMapping(hehe.Sample(samAnisotropic, vout.uv)*6, 0.5);
	result.finalColor = ACESToneMapping(lightColorOut.Sample(samAnisotropic, vout.uv), 4);
	/*if (result.finalColor.r > 3 || result.finalColor.g > 3 || result.finalColor.b > 3)
	{
		result.finalColor = float4(0, 1, 0, 1);
	}*/
	/*if (vout.uv.y <= 0.75)
	{
	if (result.finalColor.r > 1 || result.finalColor.g > 1 || result.finalColor.b > 1)
	{
	result.finalColor = float4(1, 0, 0, 1);
	}
	}
	else
	{
	result.finalColor = result.finalColor;
	}
	*/
	return result;
}