
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
	float4 DifNorParEmi;
	float4 parallaxCfg;
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

struct PSOut
{
	float4 normal		: COLOR0;
	float4 baseColor	: COLOR1;
	float4 specular		: COLOR2;
	float4 depth		: COLOR3;
};

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

VSOut GBufferVS(VSIn vin)
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

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};

Texture2D diffuseMap;
Texture2D normalMap;
Texture2D parallaxMap;

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

PSOut GBufferPS(VSOut pin) : SV_Target
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
	
	if (DifNorParEmi.y==1)
	{
		float3 normal_map = normalMap.Sample(samAnisotropic, pin.modelUV);
		normal_map = normalize(2 * (normal_map - 0.5));
		result.normal = float4(normalize(mul(normal_map, tangentMat))*0.5f + 0.5f, 0);
	}
	else
	{
		result.normal = float4(normalize(pin.normalW)*0.5f + 0.5f, 0);
	}

	if (DifNorParEmi.x==1)
	{
		result.baseColor = diffuseMap.Sample(samAnisotropic, pin.modelUV);
	}
	else
	{
		result.baseColor = float4(0.75, 0.75, 0.75, 1);
	}
	result.specular = float4(0.1, 0.1, 0.1, 1);
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

		result.finalColor = 4*(Ldiff + Lspec);
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
