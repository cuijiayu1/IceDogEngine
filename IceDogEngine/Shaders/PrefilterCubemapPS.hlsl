#include "MathBasic.hlsli"
#include "Samplers.hlsli"

cbuffer cbPerFrame : register(b0)
{
	float3 roughnessScaleFlag;
	float pad;
};

TextureCube cubeMap;

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
float radicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10f; // / 0x100000000
}

float2 hammersley(uint i, uint N)
{
	return float2(float(i) / float(N), radicalInverse_VdC(i));
}

float3 ImportanceSampleLambert(float2 xi)
{
	const float Pi = 3.1415926f;

	float phi = 2 * Pi * xi.x;
	float cos_theta = sqrt(1 - xi.y);
	float sin_theta = sqrt(1 - cos_theta * cos_theta);
	return float3(sin_theta * cos(phi), sin_theta * sin(phi), cos_theta);
}

float3 ImportanceSampleLambert(float2 xi, float3 normal)
{
	float3 h = ImportanceSampleLambert(xi);

	float3 up_vec = abs(normal.z) < 0.999f ? float3(0, 0, 1) : float3(1, 0, 0);
	float3 tangent = normalize(cross(up_vec, normal));
	float3 binormal = cross(normal, tangent);
	return tangent * h.x + binormal * h.y + normal * h.z;
}

float4 DiffuseIBL(float3 N)
{
	float3 prefiltered_clr = 0;

	const uint NUM_SAMPLES = 1024;
	for (uint i = 0; i < NUM_SAMPLES; ++i)
	{
		float2 xi = hammersley(i, NUM_SAMPLES);
		float3 L = ImportanceSampleLambert(xi, N);
		//float3 L = CosineSampleHemisphere(xi);
		float NoL = saturate(dot(N, L));
		prefiltered_clr += cubeMap.SampleLevel(linearSample, L, 0).rgb;
	}

	return float4(prefiltered_clr / NUM_SAMPLES, 1);
}

float3 ImportanceSampleGGX(float2 Xi, float Roughness, float3 N) 
{
	float a = Roughness * Roughness;
	float Phi = 2 * Pi * Xi.x;
	float CosTheta = sqrt((1 - Xi.y) / (1 + (a*a - 1) * Xi.y));
	float SinTheta = sqrt(1 - CosTheta * CosTheta);
	float3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;
	float3 UpVector = abs(N.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
	float3 TangentX = normalize(cross(UpVector, N));
	float3 TangentY = cross(N, TangentX); // Tangent to world space 
	return TangentX * H.x + TangentY * H.y + N * H.z;
}

float4 PrefilterEnvMap(float Roughness, float3 R) {
	float3 N = R;
	float3 V = R;
	float3 PrefilteredColor = 0;
	const uint NumSamples = 1024;
	float TotalWeight = 0;
	for (uint i = 0; i < NumSamples; i++) {
		float2 Xi = hammersley(i, NumSamples);
		float3 H = ImportanceSampleGGX(Xi, Roughness, N);
		float3 L = 2 * dot(V, H) * H - V;
		float NoL = saturate(dot(N, L));
		if (NoL > 0) {
			PrefilteredColor += cubeMap.SampleLevel(linearSample, L, 0).rgb * NoL;
			TotalWeight += NoL;
		}
	}
	return float4(PrefilteredColor / TotalWeight, 1);
}

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
	return normalize(float3(-uv.x, -0.5, uv.y));
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
	vt.uv = vt.uv*roughnessScaleFlag.y;
	if (roughnessScaleFlag.z > 3)
	{
		if (vt.RTindex == 0)
			return cubeMap.SampleLevel(linearSample, POS_X(vt.uv), 0);
		if (vt.RTindex == 1)
			return cubeMap.SampleLevel(linearSample, NEG_X(vt.uv), 0);
		if (vt.RTindex == 2)
			return cubeMap.SampleLevel(linearSample, POS_Y(vt.uv), 0);
		if (vt.RTindex == 3)
			return cubeMap.SampleLevel(linearSample, NEG_Y(vt.uv), 0);
		if (vt.RTindex == 4)
			return cubeMap.SampleLevel(linearSample, POS_Z(vt.uv), 0);

		return cubeMap.SampleLevel(linearSample, NEG_Z(vt.uv), 0);
	}
	if (roughnessScaleFlag.z < -3)
	{
		if (vt.RTindex == 0)
			return DiffuseIBL(POS_X(vt.uv));
		if (vt.RTindex == 1)
			return DiffuseIBL(NEG_X(vt.uv));
		if (vt.RTindex == 2)
			return DiffuseIBL(POS_Y(vt.uv));
		if (vt.RTindex == 3)
			return DiffuseIBL(NEG_Y(vt.uv));
		if (vt.RTindex == 4)
			return DiffuseIBL(POS_Z(vt.uv));

		return DiffuseIBL(NEG_Z(vt.uv));
	}
	else
	{
		if (vt.RTindex == 0)
			return PrefilterEnvMap(roughnessScaleFlag.x, POS_X(vt.uv));
		if (vt.RTindex == 1)
			return PrefilterEnvMap(roughnessScaleFlag.x, NEG_X(vt.uv));
		if (vt.RTindex == 2)
			return PrefilterEnvMap(roughnessScaleFlag.x, POS_Y(vt.uv));
		if (vt.RTindex == 3)
			return PrefilterEnvMap(roughnessScaleFlag.x, NEG_Y(vt.uv));
		if (vt.RTindex == 4)
			return PrefilterEnvMap(roughnessScaleFlag.x, POS_Z(vt.uv));

		return PrefilterEnvMap(roughnessScaleFlag.x, NEG_Z(vt.uv));
	}
}