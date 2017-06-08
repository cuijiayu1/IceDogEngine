#include "Samplers.hlsli"

cbuffer cbPerFrame : register(b0)
{
	float3 eyePos;
};

cbuffer cbPerObject : register(b1)
{
	float4 parallaxCfg;
	float4 DifNorParEmi;
};

Texture2D parallaxMap;
Texture2D normalMap;
Texture2D diffuseMap;
Texture2D SepRogMetEmi;

struct VSOut
{
	float4 positionH : SV_POSITION;
	float4 color : COLOR;
	float4 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 modelUV : TEXCOORD0;
	float2 depth : TEXCOORD1;
};

struct PSOut
{
	float4 normal		: COLOR0;
	float4 baseColor	: COLOR1;
	float4 specularRoughnessMetallic		: COLOR2;
};

float2 parallaxMapping_low(float3 tant_eyeVec, float2 uv)
{
	float3 eyeVec = tant_eyeVec;
	float h = parallaxMap.Sample(samAnisotropic, uv).x;
	return eyeVec.xy / eyeVec.z *h * parallaxCfg.x;
}

float2 parallaxMapping_high(float3 tant_eyeVec, float2 uv)
{
	float3 eyeVec = tant_eyeVec;
	float _ParallaxScale = parallaxCfg.x;

	// subdiv layers
	float numLayers = parallaxCfg.y;

	// step high in one layer
	float layerHeight = 1.0 / numLayers;
	// max high
	float currentLayerHeight = 1.0;
	// max delta
	float2 P = eyeVec.xy * _ParallaxScale;
	// delta step length
	float2 deltaTexCoords = P / numLayers;

	// step by step until find the cloestest
	float2 currentTexCoords = uv;
	float currentDepthMapValue = parallaxMap.Sample(samAnisotropic, currentTexCoords).x;
	int times = 0;
	while (currentLayerHeight > currentDepthMapValue&&times<41)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = parallaxMap.Sample(samAnisotropic, currentTexCoords).x;
		currentLayerHeight -= layerHeight;
		times = times + 1;
	}

	// cal h1, h2
	float2 prevTexCoords = currentTexCoords + deltaTexCoords;
	float afterHeight = currentDepthMapValue - currentLayerHeight;
	float beforeHeight = currentLayerHeight + layerHeight - parallaxMap.Sample(samAnisotropic, prevTexCoords).x;
	// get weight by h1 h2, interplate bet two point
	float weight = afterHeight / (afterHeight + beforeHeight);
	float2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	return finalTexCoords - uv;
}

float3x3 compute_tangent_frame(float3 N, float3 p, float2 uv)
{
	// get edge vectors of pixel triangle
	float3 dp1 = ddx(p);
	float3 dp2 = ddy(p);
	float2 duv1 = ddx(uv);
	float2 duv2 = ddy(uv);

	// solve the linear system
	float3 dp2perp = cross(dp2, N);
	float3 dp1perp = cross(N, dp1);
	float3 T = (dp2perp * duv1.x + dp1perp * duv2.x);
	float3 B = (dp2perp * duv1.y + dp1perp * duv2.y);

	// construct a scale-invariant frame 
	float invmax = rsqrt(max(dot(T, T), dot(B, B)));
	return float3x3(T*invmax, B*invmax, N);
}

PSOut main(VSOut pin) : SV_Target
{
	PSOut result;

	float3x3 tangentMat;
	if (DifNorParEmi.z == 1 || DifNorParEmi.y == 1)
	{
		tangentMat = compute_tangent_frame(normalize(pin.normalW), pin.positionW, pin.modelUV);
	}

	// if use parallax mapping just modify the uv
	if (DifNorParEmi.z == 1)
	{
		float3 eyeVec = normalize(mul(normalize(eyePos - pin.positionW), transpose(tangentMat)));
		if (abs(1 - parallaxCfg.z) < 0.01)
			pin.modelUV = pin.modelUV + parallaxMapping_high(eyeVec, pin.modelUV);
		else
			pin.modelUV = pin.modelUV + parallaxMapping_low(eyeVec, pin.modelUV);
	}

	if (DifNorParEmi.y == 1)
	{
		float3 normal_map = normalMap.Sample(samAnisotropic, pin.modelUV);
		normal_map = normalize(2 * (normal_map - 0.5));
		result.normal = float4(normalize(mul(normal_map, tangentMat))*0.5f + 0.5f, 0);
	}
	else
	{
		result.normal = float4(normalize(pin.normalW)*0.5f + 0.5f, 0);
	}

	if (DifNorParEmi.x == 1)
	{
		result.baseColor = diffuseMap.Sample(samAnisotropic, pin.modelUV);
	}
	else
	{
		result.baseColor = pin.color;
	}
	//SepRogMetEmi
	if (DifNorParEmi.w == 1)
	{
		float4 srme = SepRogMetEmi.Sample(samAnisotropic, pin.modelUV);
		result.specularRoughnessMetallic.x = 0.5;
		result.specularRoughnessMetallic.y = srme.y;
		result.specularRoughnessMetallic.z = srme.z;
	}
	else
	{
		result.specularRoughnessMetallic = float4(0.5, 0.09, 0.8, 1);
	}

	float depth = pin.depth.x / pin.depth.y;
	result.specularRoughnessMetallic.a = depth;
	// if use parallax mapping depth also need to be adjust
	//if (DifNorParEmi.z == 1)
	//{
	//	depth += parallaxCfg.x*parallaxMap.Sample(samAnisotropic, pin.modelUV).x;
	//}
	return result;
}