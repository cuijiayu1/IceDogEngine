#include "Samplers.hlsli"
#include "MathBasic.hlsli"

cbuffer cbPerFrame : register(b0)
{
	float4x4 m_viewInv;
	float4x4 m_proj;
	float3 eyePos;
};

TextureCube cubeMap;
Texture2D brdfLut;
Texture2D gBuffer_normal;
Texture2D gBuffer_baseColor;
Texture2D gBuffer_specularRoughnessMetallic;
Texture2D lBuffer_direct;

struct LightPSOut
{
	float4 finalColor : SV_Target;
};

struct LightGSOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

float3 CalRf(float3 Rf0, float thetai)
{
	return Rf0 + (1.0 - Rf0)*pow((1.0 - max(cos(thetai), 0.0)), 5);
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float3 F(float VoH, float3 SpecularColor)
{
	float Fc = Pow5(1 - VoH);
	return saturate(50.0 * SpecularColor.g) * Fc + (1 - Fc) * SpecularColor;
	//return f0 + (1 - f0)*pow(2, (-5.55473*VoH - 6.98316)*VoH);
}

float D(float Roughness, float NoH)
{
	float a = Roughness * Roughness;
	float a2 = a * a;
	float d = (NoH * a2 - NoH) * NoH + 1;	// 2 mad
	return a2 / (Pi*d*d);					// 4 mul, 1 rcp
}

float G1(float NoV, float roughness)
{
	float k = pow(roughness, 2) / 2.0;
	return NoV / (NoV*(1 - k) + k);
}

float G(float roughness, float3 n, float3 l, float3 v)
{
	return saturate(G1(saturate(dot(n, l)), roughness)*G1(saturate(dot(n, v)), roughness));
}

//////////////////////////////////////
//		Diffuse brdf begin			//
//////////////////////////////////////

float3 Diffuse_Burley(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	float FD90 = 0.5 + 2 * VoH * VoH * Roughness;
	float FdV = 1 + (FD90 - 1) * Pow5(1 - NoV);
	float FdL = 1 + (FD90 - 1) * Pow5(1 - NoL);
	return DiffuseColor * ((1 / Pi) * FdV * FdL);
}

float3 Diffuse_Lambert(float3 DiffuseColor)
{
	return DiffuseColor * (1 / Pi);
}

float3 Diffuse(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	//return Diffuse_Lambert(DiffuseColor);
	return Diffuse_Burley(DiffuseColor, Roughness, NoV, NoL, VoH);
}

//////////////////////////////////////
//		Diffuse brdf end			//
//////////////////////////////////////

// shlick aproxx
float GGX(float NdotV, float a)
{
	float k = (a * a) / 2;
	return NdotV / (NdotV * (1.0f - k) + k);
}

float G_Smith(float a, float nDotV, float nDotL)
{
	return GGX(nDotL, a) * GGX(nDotV, a);
}

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

float3 ImportanceSampleGGX(float2 Xi, float Roughness, float3 N) {
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

float4 CosineSampleHemisphere(float2 E)
{
	float Phi = 2 * Pi * E.x;
	float CosTheta = sqrt(E.y);
	float SinTheta = sqrt(1 - CosTheta * CosTheta);

	float3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;

	float PDF = CosTheta / Pi;

	return float4(H, PDF);
}


float4 cubeMapInst(float4 color)
{
	return color;
}

float3 PrefilterEnvMap(float Roughness, float3 R) {
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
			PrefilteredColor += cubeMapInst(cubeMap.SampleLevel(linearSample, L, 0)).rgb * NoL;
			TotalWeight += NoL;
		}
	}
	return PrefilteredColor / TotalWeight;
}

float3 DiffuseIBL(float3 c_diff, float3 N)
{
	float3 prefiltered_clr = 0;

	const uint NUM_SAMPLES = 1024;
	for (uint i = 0; i < NUM_SAMPLES; ++i)
	{
		float2 xi = hammersley(i, NUM_SAMPLES);
		float3 L = ImportanceSampleLambert(xi, N);
		//float3 L = CosineSampleHemisphere(xi);
		float NoL = saturate(dot(N, L));
		prefiltered_clr += cubeMapInst(cubeMap.SampleLevel(linearSample, L, 0)).rgb;
	}

	return c_diff * prefiltered_clr / NUM_SAMPLES;
}

float3 hackEnvMap(float Roughness, float3 R)
{
	//return PrefilterEnvMap(Roughness, R);
	return cubeMapInst(cubeMap.Sample(linearSample, R, Roughness*10)).xyz;
}

float3 ApproximateSpecularIBL(float3 SpecularColor, float Roughness, float3 N, float3 V) {
	float NoV = saturate(dot(N, V));
	float3 R = 2 * dot(V, N) * N - V;
	float3 PrefilteredColor = hackEnvMap(Roughness, R);
	float2 EnvBRDF = brdfLut.Sample(pointSample, float2(Roughness, NoV)).xy;
	return PrefilteredColor * (SpecularColor * EnvBRDF.x + saturate(50.0 * SpecularColor.g)*EnvBRDF.y);
}

float3 ApproximateDiffuseIBL(float3 DiffuseColor, float3 N) {
	//return DiffuseIBL(DiffuseColor, N);
	float4 difenvE = cubeMapInst(cubeMap.Sample(linearSample, N, 10)); // the brdf: direction lighting diff term 
	return DiffuseColor*difenvE;
}


float NrmDevZToViewZ(float nz)
{
	float a = m_proj[2].z;
	float b = m_proj[3].z;
	float vz = b / (nz - a);
	return vz;
}

float NrmDevXToViewX(float nx, float vz)
{
	float a = m_proj[0].x;
	float vx = (nx*vz) / a;;
	return vx;
}

float NrmDevYToViewY(float ny, float vz)
{
	float a = m_proj[1].y;
	float vy = (ny*vz) / a;
	return vy;
}


LightPSOut main(LightGSOut vout)
{
	LightPSOut result;
	if (vout.uv.y > 0.75)
	{
		if (vout.uv.x <= 0.25)
		{
			result.finalColor = gBuffer_normal.Sample(samAnisotropic, float2(vout.uv.x * 4, (vout.uv.y - 0.75) * 4));
		}
		else if (vout.uv.x <= 0.5)
		{
			result.finalColor = gBuffer_baseColor.Sample(samAnisotropic, float2(vout.uv.x * 4 - 1, (vout.uv.y - 0.75) * 4));
		}
		else if (vout.uv.x <= 0.75)
		{
			result.finalColor = gBuffer_specularRoughnessMetallic.Sample(samAnisotropic, float2(vout.uv.x * 4 - 2, (vout.uv.y - 0.75) * 4));
		}
		else
		{
			result.finalColor = gBuffer_specularRoughnessMetallic.Sample(samAnisotropic, float2(vout.uv.x * 4 - 3, (vout.uv.y - 0.75) * 4)).w;
		}
	}
	else
	{
		// cal world position using depth
		float ndcDepth = gBuffer_specularRoughnessMetallic.Sample(samAnisotropic, vout.uv).w;
		float4 wPos;
		wPos.z = NrmDevZToViewZ(ndcDepth);
		wPos.x = NrmDevXToViewX(vout.uv.x*2.0f - 1.0f, wPos.z);
		wPos.y = NrmDevYToViewY(-(vout.uv.y*2.0f - 1.0f), wPos.z);
		wPos.w = 1;
		wPos = mul(wPos, m_viewInv);

		float3 BaseColor = gBuffer_baseColor.Sample(samAnisotropic, vout.uv).xyz;
		float3 SpecularRoughnessMetallic = gBuffer_specularRoughnessMetallic.Sample(samAnisotropic, vout.uv).xyz;
		float Specular = SpecularRoughnessMetallic.x;
		float Roughness = SpecularRoughnessMetallic.y;
		float Metallic = SpecularRoughnessMetallic.z;

		float3 DiffuseColor = BaseColor * (1 - Metallic);
		float3 SpecularColor = lerp(0.08 * Specular.xxx, BaseColor, Metallic.xxx);

		float3 wNormal = gBuffer_normal.Sample(samAnisotropic, vout.uv)*2.0f - 1.0f;
		wNormal = normalize(wNormal);
		float3 v = normalize(eyePos - wPos.xyz);


		float3 Lenv = (1 - all(ndcDepth))*cubeMapInst(cubeMap.Sample(linearSample, -v, 0)).xyz;
		float3 Lenv_spec = (all(ndcDepth))*ApproximateSpecularIBL(SpecularColor, Roughness, wNormal, v);
		float3 Lenv_diff = (all(ndcDepth))*ApproximateDiffuseIBL(DiffuseColor, wNormal);

		float3 combine = lBuffer_direct.Sample(samAnisotropic, vout.uv) + Lenv_spec + Lenv_diff + Lenv;
		
		result.finalColor = float4(combine, 1);
	}
	return result;
}