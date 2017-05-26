
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

void ComputeDirectionalLight(Material mat, DirectionalLight dl,float3 normal,float3 toEye,out float4 ambient,out float4 diffuse,out float4 specular)
{
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	specular = float4(0, 0, 0, 0);
	float3 lightVec = -dl.direction;
	ambient = mat.ambient*dl.ambient;
	float diffuseFactor = dot(lightVec, normal);

	[flatten] if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		diffuse = diffuseFactor*mat.diffuse*dl.diffuse;
		specular = specFactor*mat.specular*dl.specular;
	}
}

void ComputePointLight(Material mat, PointLight pl, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	float3 lightVec = pl.position - pos;
	float d = length(lightVec);
	if (d > pl.range)
		return;
	lightVec /= d;
	ambient = mat.ambient*pl.ambient;
	float diffuseFactor = dot(lightVec, normal);
	[flatten] if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		diffuse = diffuseFactor*mat.diffuse*pl.diffuse;
		spec = specFactor*mat.specular*pl.specular;
	}
	float att = 1.0f / dot(pl.att, float3(1.0f, d, d*d));
	diffuse *= att;
	spec *= att;
}

void ComputeSpotLight(Material mat,SpotLight sl,float3 pos,float3 normal,float3 toEye,out float4 ambient,out float4 diffuse,out float4 spec)
{
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);
	float3 lightVec = sl.position - pos;
	float d = length(lightVec);
	if (d > sl.range)
		return;
	lightVec /= d;
	ambient = mat.ambient*sl.ambient;
	float diffuseFactor = dot(lightVec, normal);
	[flatten] if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		spec = specFactor*mat.specular*sl.specular;
	}
	float spot = pow(max(dot(-lightVec, sl.direction), 0.0f), sl.spot);
	float att = spot / dot(sl.att, float3(1.0f, d, d*d));
	ambient *= spot;
	diffuse *= att;
	spec *= att;
}

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
	float3 v_positionW : POSITIONW;
	float4 v_color : COLOR;
	float3 v_normalW : NORMAL;
	float3 v_tangentU : TANGENTU;
	float2 v_tex0 : TEXCOORD0;
	float2 v_tex1 : TEXCOORD1;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.v_positionH = mul(mul(mul(float4(vin.v_position, 1.0), m_world), m_view), m_proj);
	vout.v_positionW = mul(float4(vin.v_position, 1.0f), m_world).xyz;
	vout.v_color = vin.v_color;
	vout.v_normalW = mul(vin.v_normal,(float3x3)m_worldInverseTranspose);
	vout.v_tangentU = vin.v_tangentU;
	vout.v_tex0 = vin.v_tex0;
	vout.v_tex1 = vin.v_tex1;
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	pin.v_normalW = normalize(pin.v_normalW);
	float3 toEye = normalize(eyePos - pin.v_positionW);
	float4 ambient = float4(0, 0, 0, 0);
	float4 diffuse = float4(0, 0, 0, 0);
	float4 spec = float4(0, 0, 0, 0);
	float4 A, D, S;
	if (lightOn.x > 0.5)
	{
		ComputeDirectionalLight(m_mat, directionLight, pin.v_normalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	if (lightOn.y > 0.5)
	{
		ComputeSpotLight(m_mat, spotLight, pin.v_positionW, pin.v_normalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	if (lightOn.z > 0.5)
	{
		ComputePointLight(m_mat, pointLight, pin.v_positionW, pin.v_normalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	return ambient+diffuse+spec;
}

technique11 ColorTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
		//SetRasterizerState(WireframeRS);
	}
}