cbuffer cbPerFrame : register(b0)
{
	float4x4 dl_proj;
	float4x4 dl_view;
};

cbuffer cvPerObject : register(b1)
{
	float4x4 m_world;
};

struct LightVSIn
{
	float3 position : POSITION;
};

struct ShadowMapVSOut
{
	float4 positionH : SV_POSITION;
	float2 depth : DEPTHSHADOW;
};

ShadowMapVSOut main(LightVSIn vin)
{
	ShadowMapVSOut vout;
	vout.positionH = mul(mul(mul(float4(vin.position, 1.0), m_world), dl_view), dl_proj);
	vout.depth = vout.positionH.zw;
	return vout;
}