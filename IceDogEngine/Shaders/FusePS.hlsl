
Texture2D lightColorOut;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};

struct LightPSOut
{
	float4 finalColor : SV_Target;
};

struct LightGSOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

LightPSOut main(LightGSOut vout)
{
	LightPSOut result;
	result.finalColor = lightColorOut.Sample(samAnisotropic, vout.uv);
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