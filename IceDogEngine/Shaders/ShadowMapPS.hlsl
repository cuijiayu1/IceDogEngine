#include "LightDefination.hlsli"

cbuffer cbPerFrameSM
{
	float4x4 dl_proj;
	float4x4 dl_view;
	float shadow_sample_size;
};

cbuffer cbPerFrame
{
	DirectionalLight directionLight;
	SpotLight spotLight;
	PointLight pointLight;
	float3 lightOn;
	float3 eyePos;
	float2 zNearFar;
};

cbuffer cbPerObject
{
	float4x4 m_world;
	float4x4 m_view;
	float4x4 m_viewInv;
	float4x4 m_proj;
	float isolevel;
};

struct LightVSIn
{
	float3 position : POSITION;
};

struct MCVsIn
{
	float3 pos : POSITION;
	float4 val_f : WEIGHT0;
	float4 val_b : WEIGHT1;
};

struct ShadowMapPSOut
{
	float depth : COLOR;
};

struct ShadowMapVSOut
{
	float4 positionH : SV_POSITION;
	float2 depth : DEPTHSHADOW;
};

struct LightVSOut
{
	float4 position : POSITION;
};

struct LightPSOut
{
	float4 finalColor : COLOR;
};

struct LightGSOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture1D<int1> mcEdgeTable;
Texture2D<int1> mcTriTable;
Texture2D directionalShadowMap;
Texture2D gBuffer_normal;
Texture2D gBuffer_baseColor;
Texture2D gBuffer_specularRoughnessMetallic;
Texture2D gBuffer_depth;

SamplerState shadowSample {
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};

const static float Pi = 3.14159265374;

const float3 vertDecal[8] = {
	float3(0,0,0),
	float3(1,0,0),
	float3(1,1,0),
	float3(0,1,0),
	float3(0,0,1),
	float3(1,0,1),
	float3(1,1,1),
	float3(0,1,1),
};

ShadowMapPSOut main(ShadowMapVSOut pin) : SV_Target
{
	ShadowMapPSOut result;
	result.depth = pin.depth.x / pin.depth.y;
	return result;
}