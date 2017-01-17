
RasterizerState WireframeRS
{
	FillMode = Wireframe;
	CullMode = Back;
	FrontCounterClockwise = false;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 reflect;
};

struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float pad;
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float range;
	float3 att;
	float pad;
};

struct SpotLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float range;
	float3 direction;
	float spot;
	float3 att;
	float pad;
};

cbuffer cbPerFrame
{
	DirectionalLight directionLight;
	SpotLight spotLight;
	PointLight pointLight;
	float3 lightOn;
	float3 eyePos;
};

cbuffer cbPerObject
{
	float4x4 m_world;
	float4x4 m_view;
	float4x4 m_proj;
	float4x4 m_worldInverseTranspose;
	Material m_mat;
};

struct VSIn
{
	float3 v_position : POSITION;
	float4 v_color : COLOR;
	float3 v_normal : NORMAL;
	float3 v_tangentU : TANGENTU;
	float2 v_tex0 : TEXCOORD0;
	float2 v_tex1 : TEXCOORD1;
};

struct VSOut
{
	float4 v_positionH : SV_POSITION;
	float3 v_positionW : POSITIONW;
	float4 v_color : COLOR;
	float3 v_normalW : NORMAL;
	float3 v_tangentU : TANGENTU;
	float2 v_tex0 : TEXCOORD0;
	float2 v_tex1 : TEXCOORD1;
};

struct PSOut
{
	float4 normal		: COLOR0;
	float4 diffuse		: COLOR1;
	float4 specular		: COLOR2;
	float4 depth		: COLOR3;
};

VSOut GBufferVS(VSIn vin)
{
	VSOut vout;
	vout.v_positionH = mul(mul(mul(float4(vin.v_position, 1.0), m_world), m_view), m_proj);
	vout.v_positionW = mul(float4(vin.v_position, 1.0f), m_world).xyz;
	vout.v_color = vin.v_color;
	vout.v_normalW = mul(vin.v_normal,(float3x3)m_worldInverseTranspose);
	vout.v_tangentU = vin.v_tangentU;
	vout.v_tex0 = vin.v_tex0;
	vout.v_tex1 = vin.v_tex1;
	return vout;
}

PSOut GBufferPS(VSOut pin) : SV_Target
{
	PSOut result;
	float col = pin.v_positionH.z / pin.v_positionH.w;
	result.normal = float4(pin.v_normalW,0);
	result.diffuse = pin.v_color;
	result.specular = float4(0.75, 0.75, 0.75, 0.75);
	result.depth = float4(col, col, col, col);
	return result;
}

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};

Texture2D gBuffer_normal;
Texture2D gBuffer_diffuse;
Texture2D gBuffer_specular;
Texture2D gBuffer_depth;

struct LightVSIn
{
	float3 v_position : POSITION;
	float2 v_uv : TEXCOORD;
};

struct LightVSOut
{
	float4 v_positionH : SV_POSITION;
	float3 v_positionW : POSITIONW;
	float2 v_uv : TEXCOORD;
};

struct LightPSOut
{
	float4 finalColor : COLOR;
};

LightVSOut LightVS(LightVSIn vin)
{
	LightVSOut a;
	a.v_positionH = mul(mul(mul(float4(vin.v_position, 1), m_world), m_view), m_proj);
	a.v_positionW = mul(float4(vin.v_position, 1), m_world);
	a.v_uv = vin.v_uv;
	return a;
}

LightPSOut LightPS(LightVSOut vout) : SV_Target
{
	LightPSOut result;
	result.finalColor = gBuffer_diffuse.Sample(samAnisotropic, vout.v_uv);
	return result;
}

technique11 ColorTech
{
	pass gBuffer
	{
		SetVertexShader(CompileShader(vs_5_0, GBufferVS()));
		SetPixelShader(CompileShader(ps_5_0, GBufferPS()));
	}

	pass lightPass
	{
		SetVertexShader(CompileShader(vs_5_0, LightVS()));
		SetPixelShader(CompileShader(ps_5_0, LightPS()));
	}
}