
RasterizerState WireframeRS
{
	FillMode = Wireframe;
	CullMode = Back;
	FrontCounterClockwise = false;
};

cbuffer cbPerObject
{
	float4x4 m_world;
	float4x4 m_view;
	float4x4 m_proj;
};

struct VertexIn
{
	float3 v_position : POSITION;
	float4 v_color : COLOR;
	float3 v_normal : NORMAL;
	float3 v_tangentU : TANGENTU;
	float2 v_tex0 : TEXCOORD0;
	float2 v_tex1 : TEXCOORD1;
};

struct VertexOut
{
	float4 v_positionH : SV_POSITION;
	float4 v_color : COLOR;
	float3 v_normal : NORMAL;
	float3 v_tangentU : TANGENTU;
	float2 v_tex0 : TEXCOORD0;
	float2 v_tex1 : TEXCOORD1;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.v_positionH = mul(mul(mul(float4(vin.v_position, 1.0), m_world), m_view), m_proj);
	vout.v_color = vin.v_color;
	vout.v_normal = vin.v_normal;
	vout.v_tangentU = vin.v_tangentU;
	vout.v_tex0 = vin.v_tex0;
	vout.v_tex1 = vin.v_tex1;
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.v_color;
}

technique11 ColorTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetRasterizerState(WireframeRS);
	}
}