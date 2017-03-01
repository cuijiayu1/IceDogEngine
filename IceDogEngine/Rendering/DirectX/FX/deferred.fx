
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
	float4x4 m_viewInv;
	float4x4 m_proj;
	float4x4 m_worldInverseTranspose;
	Material m_mat;
};

struct VSIn
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 tangentU : TANGENTU;
	float2 diffuseUV : TEXCOORD0;
	float2 normalUV : TEXCOORD1;
};

struct VSOut
{
	float4 positionH : SV_POSITION;
	float4 color : COLOR;
	float2 diffuseUV : TEXCOORD0;
	float3 normalW : TEXCOORD1;
	float2 depth : TEXCOORD2;
};

struct PSOut
{
	float4 normal		: COLOR0;
	float4 baseColor	: COLOR1;
	float4 specular		: COLOR2;
	float4 depth		: COLOR3;
};

VSOut GBufferVS(VSIn vin)
{
	VSOut vout;
	vout.positionH = mul(mul(mul(float4(vin.position, 1.0), m_world), m_view), m_proj);
	vout.diffuseUV = vin.diffuseUV;
	vout.normalW = normalize(mul(float4(vin.normal, 0.0), m_world).xyz);
	vout.color = vin.color;
	vout.depth = vout.positionH.zw;
	return vout;
}

float3 float_to_color(float f)
{
	float3 color;
	f *= 256;
	color.x = floor(f);
	f = (f - color.x) * 256;
	color.y = floor(f);
	color.z = f - color.y;
	color.xy *= 0.00390625; // *= 1.0/256
	return color;
}

float color_to_float(float3 color)
{
	const float3 byte_to_float = float3(1.0, 1.0 / 256, 1.0 / (256 * 256));
	return dot(color, byte_to_float);
}

Texture2D diffuseMap;
SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};

PSOut GBufferPS(VSOut pin) : SV_Target
{
	PSOut result;
	result.normal = float4(normalize(pin.normalW)*0.5f + 0.5f, 0);
	result.baseColor = diffuseMap.Sample(samAnisotropic, pin.diffuseUV);
	result.specular = float4(0.75, 0.75, 0.75, 1);
	float depth = pin.depth.x / pin.depth.y;
	result.depth.rgb = float_to_color(depth);
	result.depth.a = 1;
	return result;
}

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
	// ny = vy/(vz*a)
	float vy = (ny*vz) / a;
	return vy;
}

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
Texture2D gBuffer_baseColor;
Texture2D gBuffer_specular;
Texture2D gBuffer_depth;

const static float Pi = 3.14159265374;

LightPSOut LightPS(LightGSOut vout) : SV_Target
{
	LightPSOut result;
	if (vout.uv.y > 0.75)
	{
		if (vout.uv.x <= 0.25)
		{
			result.finalColor= gBuffer_normal.Sample(samAnisotropic, float2(vout.uv.x * 4, (vout.uv.y - 0.75) * 4));
		}
		else if (vout.uv.x <= 0.5)
		{
			result.finalColor = gBuffer_baseColor.Sample(samAnisotropic, float2(vout.uv.x * 4-1, (vout.uv.y - 0.75) * 4));
		}
		else if (vout.uv.x <= 0.75)
		{
			result.finalColor = gBuffer_specular.Sample(samAnisotropic, float2(vout.uv.x * 4 - 2, (vout.uv.y - 0.75) * 4));
		}
		else
		{
			result.finalColor = gBuffer_depth.Sample(samAnisotropic, float2(vout.uv.x * 4 - 3, (vout.uv.y - 0.75) * 4));
		}
	}
	else
	{
		// cal world position using depth
		float4 depthSample = gBuffer_depth.Sample(samAnisotropic, vout.uv);
		float ndcDepth = clamp(color_to_float(depthSample.xyz), 0.0f, 1.0f);
		float4 wPos;
		wPos.z = NrmDevZToViewZ(ndcDepth);
		wPos.x = NrmDevXToViewX(vout.uv.x*2.0f - 1.0f, wPos.z);
		wPos.y = NrmDevYToViewY(-(vout.uv.y*2.0f - 1.0f), wPos.z);
		wPos.w = 1;
		wPos = mul(wPos, m_viewInv);

		float3 wNormal = gBuffer_normal.Sample(samAnisotropic, vout.uv)*2.0f - 1.0f;
		float3 El = float3(1,1,1);
		float3 l = normalize(-directionLight.direction);
		float Cosi = saturate(dot(wNormal, l));
		float4 Cdiff = gBuffer_baseColor.Sample(samAnisotropic, vout.uv);
		float4 Mdiff = float4(Cdiff.xyz * El * Cosi,Cdiff.w);
		float4 Ldiff = Mdiff / Pi;

		float m = 10;
		float4 Cspec = gBuffer_specular.Sample(samAnisotropic, vout.uv);
		float4 Mspec = float4(Cspec.xyz * El * Cosi, Cspec.w);
		float3 v = normalize(eyePos - wPos.xyz);
		float3 h = normalize(l + v);
		float Cosh = saturate(dot(wNormal, h));
		float4 Lspec = ((m + 8) / (8 * Pi))*pow(Cosh, m)*Mspec;

		result.finalColor = Ldiff + Lspec;
	}
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
