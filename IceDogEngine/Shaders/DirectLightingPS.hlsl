#include "LightDefination.hlsli"
#include "Samplers.hlsli"
#include "MathBasic.hlsli"

cbuffer cbPerFrame : register(b0)
{
	DirectionalLight directionLight;
	float4x4 dl_proj;
	float4x4 dl_view;
	float4x4 m_viewInv;
	float4x4 m_proj;
	float3 eyePos;
	float shadow_sample_size;
	float2 zNearFar;
	float2 pad;
};

cbuffer cbPerObject : register(b1)
{
	float3 lightOn;
};

static const float2 poissonDisk[16] = {
	float2(-0.94201624, -0.39906216),
	float2(0.94558609, -0.76890725),
	float2(-0.094184101, -0.92938870),
	float2(0.34495938, 0.29387760),
	float2(-0.91588581, 0.45771432),
	float2(-0.81544232, -0.87912464),
	float2(-0.38277543, 0.27676845),
	float2(0.97484398, 0.75648379),
	float2(0.44323325, -0.97511554),
	float2(0.53742981, -0.47373420),
	float2(-0.26496911, -0.41893023),
	float2(0.79197514, 0.19090188),
	float2(-0.24188840, 0.99706507),
	float2(-0.81409955, 0.91437590),
	float2(0.19984126, 0.78641367),
	float2(0.14383161, -0.14100790)
};

Texture2D<float> directionalShadowMap;

Texture2D<float4> gBuffer_normal;
Texture2D<float4> gBuffer_baseColor;
Texture2D<float4> gBuffer_specularRoughnessMetallic;

struct LightPSOut
{
	float4 finalColor : COLOR;
};

struct LightGSOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

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

float PCFFilter(float2 smpUV, float cmpDepth, float smpDx, float filterRadius)
{
	float sum = 0.0;
	for (int i = 0; i < 16; ++i)
	{
		float2 offset = poissonDisk[i] * filterRadius * cmpDepth *smpDx;
		float depth = directionalShadowMap.Sample(shadowSample, smpUV + offset);
		sum += depth > cmpDepth;
	}
	return sum / 16.0;
}

float PCSS(float2 smpUV, float cmpDepth, float smpDx, float lightSize)
{
	// block search
	float blocker = 0.0;
	int count = 0;
	float viewLength = (zNearFar.y - zNearFar.x);
	for (int i = 0; i < 16; ++i)
	{
		//poissonDisk[i] * smpDx to correct uv step
		//cmpDepth is [0-1], multiply viewLength to get step count
		//lightSize is [0-1] to control the search width
		float2 offset = poissonDisk[i] * smpDx * cmpDepth * viewLength * lightSize;
		float depth = directionalShadowMap.Sample(shadowSample, smpUV + offset);
		if (depth < cmpDepth)
		{
			blocker += depth;
			count++;
		}
	}
	if (count == 0)
		return 1;
	blocker /= count;
	//(cmpDepth - blocker) / blocker is (0-1), multiply viewLength to get correct width
	//again lightSize is used to control the rad.
	float rad = (cmpDepth - blocker) * viewLength * lightSize / blocker;

	return PCFFilter(smpUV, cmpDepth, smpDx, 4 * (rad + 1));
}

float NormalLinear(float2 smpUV, float cmpDepth, float smpDx)
{
	float r_0 = directionalShadowMap.Sample(shadowSample, smpUV).x >= cmpDepth;
	float r_1 = directionalShadowMap.Sample(shadowSample, smpUV + float2(smpDx, 0)).x >= cmpDepth;
	float r_2 = directionalShadowMap.Sample(shadowSample, smpUV + float2(0, smpDx)).x >= cmpDepth;
	float r_3 = directionalShadowMap.Sample(shadowSample, smpUV + float2(smpDx, smpDx)).x >= cmpDepth;

	float2 texelPos = shadow_sample_size*smpUV;
	float2 t = frac(texelPos);

	return lerp(lerp(r_0, r_1, t.x), lerp(r_2, r_3, t.x), t.y);
}

float ShadowLightFract(float4 wPos)
{
	float fract = 0;
	float4 tempLoc = mul(mul(wPos, dl_view), dl_proj);
	// convert to NDC
	tempLoc = tempLoc / tempLoc.w;
	if (abs(tempLoc.x)>1 || abs(tempLoc.y) >1 || tempLoc.z>1 || tempLoc.z<0)
	{
		fract = 1;
		return fract;
	}
	// convert to UV
	float2 ShadowTexC = 0.5 * tempLoc.xy + float2(0.5, 0.5);
	ShadowTexC.y = 1.0f - ShadowTexC.y;
	float cmpDepth = -0.0005 + tempLoc.z;
	float smpDx = 1 / shadow_sample_size;

	return PCSS(ShadowTexC, cmpDepth, smpDx, 1);
	//return PCFFilter(ShadowTexC, cmpDepth, smpDx, 8);
	//return NormalLinear(ShadowTexC, cmpDepth, smpDx);
}

LightPSOut main(LightGSOut vout) : SV_Target
{
	LightPSOut result;
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
	float3 El = directionLight.diffuse*directionLight.intensity;

	float3 DiffuseColor = BaseColor * (1 - Metallic);
	float3 SpecularColor = lerp(0.08 * Specular.xxx, BaseColor, Metallic.xxx);

	float3 wNormal = gBuffer_normal.Sample(samAnisotropic, vout.uv).xyz*2.0f - 1.0f;
	wNormal = normalize(wNormal);
	float3 l = normalize(-directionLight.direction);
	float3 v = normalize(eyePos - wPos.xyz);
	float3 h = normalize(l + v);
	float Cosi = saturate(dot(wNormal, l));

	float3 brdf_diff = Diffuse(DiffuseColor, Roughness, saturate(dot(wNormal, v)), saturate(dot(wNormal, l)), saturate(dot(h, v)));
	float3 brdf_spec = (D(Roughness, saturate(dot(wNormal, h)))*F(saturate(dot(v, h)), SpecularColor)*G(Roughness, wNormal, l, v)) / (4 * dot(wNormal, l)*dot(wNormal, v));

	float3 Ldir_diff_spec = (all(ndcDepth))*(brdf_diff + brdf_spec) * El*Cosi*ShadowLightFract(wPos);

	result.finalColor = float4(Ldir_diff_spec, 1);
	return result;
}