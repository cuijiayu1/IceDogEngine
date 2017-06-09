#include "MathBasic.hlsli"
#include "Samplers.hlsli"

Texture2D hdrsource;

struct PrefilterCubemapOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	uint RTindex : SV_RenderTargetArrayIndex;
};

/*
0         TEXTURE_CUBE_MAP_POSITIVE_X
1         TEXTURE_CUBE_MAP_NEGATIVE_X
2         TEXTURE_CUBE_MAP_POSITIVE_Y
3         TEXTURE_CUBE_MAP_NEGATIVE_Y
4         TEXTURE_CUBE_MAP_POSITIVE_Z
5         TEXTURE_CUBE_MAP_NEGATIVE_Z
*/

float3 POS_X(float2 uv)
{
	uv.xy = uv.yx;
	uv = uv - 0.5;
	return normalize(float3(0.5, -uv.x, -uv.y));
}

float3 NEG_X(float2 uv)
{
	uv = uv.yx;
	uv = uv - 0.5;
	return normalize(float3(-0.5, -uv.x, uv.y));
}

float3 POS_Y(float2 uv)
{
	uv = uv - 0.5;
	return normalize(float3(uv.x, 0.5, uv.y));
}

float3 NEG_Y(float2 uv)
{
	uv = uv - 0.5;
	return normalize(float3(uv.x, -0.5, -uv.y));
}

float3 POS_Z(float2 uv)
{
	uv = uv - 0.5;
	return normalize(float3(uv.x, -uv.y, 0.5));
}

float3 NEG_Z(float2 uv)
{
	uv = uv - 0.5;
	return normalize(float3(-uv.x, -uv.y, -0.5));
}

float4 main(PrefilterCubemapOut vt) : SV_Target
{
	static const float3 up=float3(0, 1, 0);
	static const float3 right=float3(1, 0, 0);
	static const float3 forward=float3(0, 0, 1);

	float3 sphereNormal = NEG_Z(vt.uv);
	if (vt.RTindex == 0)
		sphereNormal = POS_X(vt.uv);
	if (vt.RTindex == 1)
		sphereNormal = NEG_X(vt.uv);
	if (vt.RTindex == 2)
		sphereNormal = POS_Y(vt.uv);
	if (vt.RTindex == 3)
		sphereNormal = NEG_Y(vt.uv);
	if (vt.RTindex == 4)
		sphereNormal = POS_Z(vt.uv);

	float v_dig = (1 - dot(up, sphereNormal)) / 2;
	float u_dig = 0;

	sphereNormal.y = 0;
	sphereNormal = normalize(sphereNormal);
	float dotRight = dot(sphereNormal, right);
	float dotFor = dot(sphereNormal, forward);
	if (dotFor >= 0)
		u_dig = acos(dotRight);
	else
		u_dig = 2 * Pi - acos(dotRight);
	u_dig = u_dig / (2 * Pi);
	u_dig = clamp(u_dig, 0, 1);
	return hdrsource.Sample(samAnisotropic, float2(u_dig, v_dig));
}