#include "Samplers.hlsli"

Texture2D lightColorOut;

struct LightPSOut
{
	float4 finalColor : COLOR;
};

struct LightGSOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

LightPSOut main(LightGSOut vout) : SV_Target
{
	LightPSOut result;
	result.finalColor = lightColorOut.Sample(samAnisotropic, vout.uv * 4);
	return result;
}