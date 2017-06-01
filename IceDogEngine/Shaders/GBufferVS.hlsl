
cbuffer cbPerFrame : register(b0)
{
	float4x4 m_view;
	float4x4 m_proj;
};

cbuffer cbPerObject : register(b1)
{
	float4x4 m_world;
	float4x4 m_worldInverseTranspose;
};

struct VSIn
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 diffuseUV : TEXCOORD0;
	float2 lightUV : TEXCOORD1;
};

struct VSOut
{
	float4 positionH : SV_POSITION;
	float4 color : COLOR;
	float4 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 modelUV : TEXCOORD0;
	float2 depth : TEXCOORD1;
};

VSOut main(VSIn vin)
{
	VSOut vout;
	vout.positionH = mul(mul(mul(float4(vin.position, 1.0), m_world), m_view), m_proj);
	vout.positionW = mul(float4(vin.position, 1.0), m_world);
	vout.modelUV = vin.diffuseUV;
	vout.normalW = normalize(mul(float4(vin.normal, 0.0), m_worldInverseTranspose).xyz);
	vout.color = vin.color;
	vout.depth = vout.positionH.zw;
	return vout;
}