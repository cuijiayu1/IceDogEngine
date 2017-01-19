
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
	float2 tex : TEXCOORD0;
	float2 tex1 : TEXCOORD1;
};

struct VSOut
{
	float4 positionH : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normalW : TEXCOORD1;
	float2 depth : TEXCOORD2;
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
	vout.positionH = mul(mul(mul(float4(vin.position, 1.0), m_world), m_view), m_proj);
	vout.tex = vin.tex;
	vout.normalW = mul(float4(vin.normal, 0.0), m_world).xyz;
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

PSOut GBufferPS(VSOut pin) : SV_Target
{
	PSOut result;
	result.normal = float4(normalize(pin.normalW)*0.5f + 0.5f, 0);
	result.diffuse = pin.color;
	result.specular = float4(0.75, 0.75, 0.75, 1);
	float depth = pin.depth.x / pin.depth.y;
	result.depth.rgb = float_to_color(depth);
	result.depth.a = 1;
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
Texture2D gBuffer_diffuse;
Texture2D gBuffer_specular;
Texture2D gBuffer_depth;

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
			result.finalColor = gBuffer_diffuse.Sample(samAnisotropic, float2(vout.uv.x * 4-1, (vout.uv.y - 0.75) * 4));
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
		float4 depthSample = gBuffer_depth.Sample(samAnisotropic, vout.uv);
		float ndcDepth = clamp(color_to_float(depthSample.xyz), 0.0f, 1.0f);

		float4 vPos;
		vPos.z = NrmDevZToViewZ(ndcDepth);
		vPos.x = NrmDevXToViewX(vout.uv.x*2.0f - 1.0f, vPos.z);
		vPos.y = NrmDevYToViewY(-(vout.uv.y*2.0f - 1.0f), vPos.z);
		//vPos.w = 1;
		//vPos = mul(vPos,(float3x3)m_viewInv);

		float4 vNormal = gBuffer_normal.Sample(samAnisotropic, vout.uv)*2.0f - 1.0f;

		float4 diffuse = gBuffer_diffuse.Sample(samAnisotropic, vout.uv);

		float4 specular = gBuffer_specular.Sample(samAnisotropic, vout.uv);

		float3 toEye = normalize(float3(0.0f, 0.0f, 0.0f) - vPos);

		float3 r = reflect(float3(2,-2,2), normalize(vNormal.xyz));

		float3 gSpecularLight = float3(0.1, 0.1, 0.1);
		float3 gDiffuseLight = float3(0.25, 0.25, 0.25);
		float t = pow(max(dot(r, toEye), 0.0f), specular.w*1.5);

		float s = max(dot(float3(-2, 2, -2), vNormal.xyz), 0.0f);

		float3 specColor =  t*(specular.xyz*gSpecularLight);
		float3 diffuseColor = s*(diffuse.xyz*gDiffuseLight);
		float3 ambientColor = (diffuse.xyz*gDiffuseLight)*0.3f;

		result.finalColor.xyz = specColor + diffuseColor + ambientColor;
		result.finalColor.a = 1.0f;
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
