
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

struct LightVSIn
{
	float3 position : POSITION;
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

LightVSOut LightVS(LightVSIn vin)
{
	LightVSOut a;
	a.position = float4(0.5, 0.5, 0, 1);
	return a;
}

[maxvertexcount(4)]
void LightGS(point LightVSOut gIn[1],
inout TriangleStream<LightGSOut> triStream)
{
	LightGSOut O0;
	LightGSOut O1;
	LightGSOut O2;
	LightGSOut O3;
	O0.uv = float2(0, 1);   //------->u
	O1.uv = float2(0, 0);	//|
	O2.uv = float2(1, 1);	//|
	O3.uv = float2(1, 0);	//v
	O0.position = float4(-1, -1, 0, 1);		//y
	O1.position = float4(-1, 1, 0, 1);		//|
	O2.position = float4(1, -1, 0, 1);		//|
	O3.position = float4(1, 1, 0, 1);		//----->x
	triStream.Append(O0);
	triStream.Append(O1);
	triStream.Append(O2);
	triStream.Append(O3);
}

Texture2D gBuffer_normal;
Texture2D gBuffer_diffuse;
Texture2D gBuffer_specular;
Texture2D gBuffer_depth;

LightPSOut LightPS(LightGSOut vout) : SV_Target
{
	LightPSOut result;
	result.finalColor = gBuffer_depth.Sample(samAnisotropic, vout.uv);
	return result;
}

technique11 Deferred
{
	pass GBufferStage
	{
		SetVertexShader(CompileShader(vs_5_0, GBufferVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, GBufferPS()));
	}
	pass LightingStage
	{
		SetVertexShader(CompileShader(vs_5_0, LightVS()));
		SetGeometryShader(CompileShader(gs_5_0, LightGS()));
		SetPixelShader(CompileShader(ps_5_0, LightPS()));
	}
}
