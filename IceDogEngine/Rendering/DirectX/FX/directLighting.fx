struct DirectionalLight
{
	float4 direction;
	float3 diffuse;
	float intensity;
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Shadow Map Pass Begin															//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  [ 7 6 ]
//[ 3 2 ]
//	[ 4 5 ]
//[ 0 1 ]
//

float3 vertexInterp(float isolevel, float3 p1, float valp1, float3 p2, float valp2)
{
	return lerp(p1, p2, (isolevel - valp1) / (valp2 - valp1));
}

//Get edge table value
int edgeTableValue(int i) {
	return mcEdgeTable[i].r;
}

//Get triangle table value
int triTableValue(int i, int j) {
	return mcTriTable[int2(j, i)].r;
}

MCVsIn MCVS(MCVsIn vin)
{
	return vin;
}

ShadowMapVSOut ShadowMapVS(LightVSIn vin)
{
	ShadowMapVSOut vout;
	vout.positionH = mul(mul(mul(float4(vin.position, 1.0), m_world), dl_view), dl_proj);
	vout.depth = vout.positionH.zw;
	return vout;
}

ShadowMapPSOut ShadowMapPS(ShadowMapVSOut pin) : SV_Target{
	ShadowMapPSOut result;
	result.depth = pin.depth.x / pin.depth.y;
	return result;
}

[maxvertexcount(16)]
void ShadowMapGS(point MCVsIn gIn[1], inout TriangleStream<ShadowMapVSOut> triStream)
{
	//  [ 7 6 ] (back)
	//[ 3 2 ] (front)
	//	[ 4 5 ]
	//[ 0 1 ]
	//
	MCVsIn vet = gIn[0];
	int cubeindex = 0;
	//Determine the index into the edge table which
	//tells us which vertices are inside of the surface
	if (vet.val_f.x < isolevel) cubeindex = cubeindex | 1;
	if (vet.val_f.y < isolevel) cubeindex = cubeindex | 2;
	if (vet.val_f.z < isolevel) cubeindex = cubeindex | 4;
	if (vet.val_f.w < isolevel) cubeindex = cubeindex | 8;
	if (vet.val_b.x < isolevel) cubeindex = cubeindex | 16;
	if (vet.val_b.y < isolevel) cubeindex = cubeindex | 32;
	if (vet.val_b.z < isolevel) cubeindex = cubeindex | 64;
	if (vet.val_b.w < isolevel) cubeindex = cubeindex | 128;

	//Cube is entirely in/out of the surface
	if (edgeTableValue(cubeindex) == 0)
		return;

	float3 vertlist[12];
	//Find the vertices where the surface intersects the cube
	if ((edgeTableValue(cubeindex) & 1) != 0)
		vertlist[0] = vertexInterp(isolevel, vet.pos + vertDecal[0], vet.val_f.x, vet.pos + vertDecal[1], vet.val_f.y);
	if ((edgeTableValue(cubeindex) & 2) != 0)
		vertlist[1] = vertexInterp(isolevel, vet.pos + vertDecal[1], vet.val_f.y, vet.pos + vertDecal[2], vet.val_f.z);
	if ((edgeTableValue(cubeindex) & 4) != 0)
		vertlist[2] = vertexInterp(isolevel, vet.pos + vertDecal[2], vet.val_f.z, vet.pos + vertDecal[3], vet.val_f.w);
	if ((edgeTableValue(cubeindex) & 8) != 0)
		vertlist[3] = vertexInterp(isolevel, vet.pos + vertDecal[3], vet.val_f.w, vet.pos + vertDecal[0], vet.val_f.x);
	if ((edgeTableValue(cubeindex) & 16) != 0)
		vertlist[4] = vertexInterp(isolevel, vet.pos + vertDecal[4], vet.val_b.x, vet.pos + vertDecal[5], vet.val_b.y);
	if ((edgeTableValue(cubeindex) & 32) != 0)
		vertlist[5] = vertexInterp(isolevel, vet.pos + vertDecal[5], vet.val_b.y, vet.pos + vertDecal[6], vet.val_b.z);
	if ((edgeTableValue(cubeindex) & 64) != 0)
		vertlist[6] = vertexInterp(isolevel, vet.pos + vertDecal[6], vet.val_b.z, vet.pos + vertDecal[7], vet.val_b.w);
	if ((edgeTableValue(cubeindex) & 128) != 0)
		vertlist[7] = vertexInterp(isolevel, vet.pos + vertDecal[7], vet.val_b.w, vet.pos + vertDecal[4], vet.val_b.x);
	if ((edgeTableValue(cubeindex) & 256) != 0)
		vertlist[8] = vertexInterp(isolevel, vet.pos + vertDecal[0], vet.val_f.x, vet.pos + vertDecal[4], vet.val_b.x);
	if ((edgeTableValue(cubeindex) & 512) != 0)
		vertlist[9] = vertexInterp(isolevel, vet.pos + vertDecal[1], vet.val_f.y, vet.pos + vertDecal[5], vet.val_b.y);
	if ((edgeTableValue(cubeindex) & 1024) != 0)
		vertlist[10] = vertexInterp(isolevel, vet.pos + vertDecal[2], vet.val_f.z, vet.pos + vertDecal[6], vet.val_b.z);
	if ((edgeTableValue(cubeindex) & 2048) != 0)
		vertlist[11] = vertexInterp(isolevel, vet.pos + vertDecal[3], vet.val_f.w, vet.pos + vertDecal[7], vet.val_b.w);

	for (int i = 0; i<16; i += 3) {
		if (triTableValue(cubeindex, i) == -1) { break; }
		// vertex 0 
		ShadowMapVSOut v0;
		v0.positionH = mul(mul(mul(float4(vertlist[triTableValue(cubeindex, i)], 1), m_world), dl_view), dl_proj);
		v0.depth = v0.positionH.zw;

		// vertex 1
		ShadowMapVSOut v1;
		v1.positionH = mul(mul(mul(float4(vertlist[triTableValue(cubeindex, i + 1)], 1), m_world), dl_view), dl_proj);
		v1.depth = v1.positionH.zw;

		// vertex 2
		ShadowMapVSOut v2;
		v2.positionH = mul(mul(mul(float4(vertlist[triTableValue(cubeindex, i + 2)], 1), m_world), dl_view), dl_proj);
		v2.depth = v2.positionH.zw;

		//triStream.Append(v1);
		//triStream.Append(v0);
		triStream.Append(v0);
		triStream.Append(v1);
		triStream.Append(v2);
		triStream.RestartStrip();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Shadow Map Pass End																//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Lighting Pass Start																//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	O0.position = float4(-1, -1, 1, 1);		//y
	O1.position = float4(-1, 1, 1, 1);		//|
	O2.position = float4(1, -1, 1, 1);		//|
	O3.position = float4(1, 1, 1, 1);		//----->x
	triStream.Append(O0);
	triStream.Append(O1);
	triStream.Append(O2);
	triStream.Append(O3);
}

float Pow5(float x)
{
	float x2 = x*x;
	return x2 * x2 * x;
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float3 F(float VoH, float3 SpecularColor)
{
	float Fc = Pow5(1 - VoH);
	return saturate(50.0 * SpecularColor.g) * Fc + (1 - Fc) * SpecularColor;
	//return f0 + (1 - f0)*pow(2, (-5.55473*VoH - 6.98316)*VoH);
}

float D(float Roughness, float NoH)
{
	float a = Roughness * Roughness;
	float a2 = a * a;
	float d = (NoH * a2 - NoH) * NoH + 1;	// 2 mad
	return a2 / (Pi*d*d);					// 4 mul, 1 rcp
}

float G1(float NoV, float roughness)
{
	float k = pow(roughness, 2) / 2.0;
	return NoV / (NoV*(1 - k) + k);
}

float G(float roughness, float3 n, float3 l, float3 v)
{
	return saturate(G1(saturate(dot(n, l)), roughness)*G1(saturate(dot(n, v)), roughness));
}

//////////////////////////////////////
//		Diffuse brdf begin			//
//////////////////////////////////////

float3 Diffuse_Burley(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	float FD90 = 0.5 + 2 * VoH * VoH * Roughness;
	float FdV = 1 + (FD90 - 1) * Pow5(1 - NoV);
	float FdL = 1 + (FD90 - 1) * Pow5(1 - NoL);
	return DiffuseColor * ((1 / Pi) * FdV * FdL);
}

float3 Diffuse_Lambert(float3 DiffuseColor)
{
	return DiffuseColor * (1 / Pi);
}

float3 Diffuse(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	//return Diffuse_Lambert(DiffuseColor);
	return Diffuse_Burley(DiffuseColor, Roughness, NoV, NoL, VoH);
}

//////////////////////////////////////
//		Diffuse brdf end			//
//////////////////////////////////////

const float2 poissonDisk[16] = {
	float2(-0.94201624, -0.39906216),
	float2(0.94558609, -0.76890725),
	float2(-0.094184101, -0.92938870),
	float2(0.34495938, 0.29387760),
	float2(-0.91588581, 0.45771432),
	float2(-0.81544232, -0.87912464),
	float2(-0.38277543, 0.27676845),
	float2(0.97484398, 0.75648379),
	float2(0.44323325, -0.97511554),
	float2(0.53742981, -0.47373420),
	float2(-0.26496911, -0.41893023),
	float2(0.79197514, 0.19090188),
	float2(-0.24188840, 0.99706507),
	float2(-0.81409955, 0.91437590),
	float2(0.19984126, 0.78641367),
	float2(0.14383161, -0.14100790)
};

float PCFFilter(float2 smpUV, float cmpDepth, float smpDx, float filterRadius)
{
	float sum = 0.0;
	for (int i = 0; i < 16; ++i)
	{
		float2 offset = poissonDisk[i] * filterRadius * cmpDepth *smpDx;
		float depth = directionalShadowMap.Sample(shadowSample, smpUV + offset);
		sum += depth > cmpDepth;
	}
	return sum / 16.0;
}

float PCSS(float2 smpUV, float cmpDepth, float smpDx, float lightSize)
{
	// block search
	float blocker = 0.0;
	int count = 0;
	float viewLength = (zNearFar.y - zNearFar.x);
	for (int i = 0; i < 16; ++i)
	{
		//poissonDisk[i] * smpDx to correct uv step
		//cmpDepth is [0-1], multiply viewLength to get step count
		//lightSize is [0-1] to control the search width
		float2 offset = poissonDisk[i] * smpDx * cmpDepth * viewLength * lightSize;
		float depth = directionalShadowMap.Sample(shadowSample, smpUV + offset);
		if (depth < cmpDepth)
		{
			blocker += depth;
			count++;
		}
	}
	if (count == 0)
		return 1;
	blocker /= count;
	//(cmpDepth - blocker) / blocker is (0-1), multiply viewLength to get correct width
	//again lightSize is used to control the rad.
	float rad = (cmpDepth - blocker) * viewLength * lightSize / blocker;
	
	return PCFFilter(smpUV, cmpDepth, smpDx, 4*(rad + 1));
}

float NormalLinear(float2 smpUV, float cmpDepth, float smpDx)
{
	float r_0 = directionalShadowMap.Sample(shadowSample, smpUV).x >= cmpDepth;
	float r_1 = directionalShadowMap.Sample(shadowSample, smpUV + float2(smpDx, 0)).x >= cmpDepth;
	float r_2 = directionalShadowMap.Sample(shadowSample, smpUV + float2(0, smpDx)).x >= cmpDepth;
	float r_3 = directionalShadowMap.Sample(shadowSample, smpUV + float2(smpDx, smpDx)).x >= cmpDepth;

	float2 texelPos = shadow_sample_size*smpUV;
	float2 t = frac(texelPos);

	return lerp(lerp(r_0, r_1, t.x), lerp(r_2, r_3, t.x), t.y);
}

float ShadowLightFract(float4 wPos)
{
	float fract = 0;
	float4 tempLoc = mul(mul(wPos, dl_view), dl_proj);
	// convert to NDC
	tempLoc = tempLoc / tempLoc.w;
	if (abs(tempLoc.x)>1 || abs(tempLoc.y) >1|| tempLoc.z>1 || tempLoc.z<0)
	{
		fract = 1;
		return fract;
	}
	// convert to UV
	float2 ShadowTexC = 0.5 * tempLoc.xy + float2(0.5, 0.5);
	ShadowTexC.y = 1.0f - ShadowTexC.y;
	float cmpDepth = -0.0005 + tempLoc.z;
	float smpDx = 1/ shadow_sample_size;
	
	return PCSS(ShadowTexC, cmpDepth, smpDx, 0.4);
	//return PCFFilter(ShadowTexC, cmpDepth, smpDx, 8);
	//return NormalLinear(ShadowTexC, cmpDepth, smpDx);
}

LightPSOut LightPS(LightGSOut vout) : SV_Target{
	LightPSOut result;
	// cal world position using depth
	float ndcDepth = gBuffer_specularRoughnessMetallic.Sample(samAnisotropic, vout.uv).w;
	float4 wPos;
	wPos.z = NrmDevZToViewZ(ndcDepth);
	wPos.x = NrmDevXToViewX(vout.uv.x*2.0f - 1.0f, wPos.z);
	wPos.y = NrmDevYToViewY(-(vout.uv.y*2.0f - 1.0f), wPos.z);
	wPos.w = 1;
	wPos = mul(wPos, m_viewInv);

	float3 BaseColor = gBuffer_baseColor.Sample(samAnisotropic, vout.uv).xyz;
	float3 SpecularRoughnessMetallic = gBuffer_specularRoughnessMetallic.Sample(samAnisotropic, vout.uv).xyz;
	float Specular = SpecularRoughnessMetallic.x;
	float Roughness = SpecularRoughnessMetallic.y;
	float Metallic = SpecularRoughnessMetallic.z;
	float3 El = directionLight.diffuse*directionLight.intensity;

	float3 DiffuseColor = BaseColor * (1 - Metallic);
	float3 SpecularColor = lerp(0.08 * Specular.xxx, BaseColor, Metallic.xxx);

	float3 wNormal = gBuffer_normal.Sample(samAnisotropic, vout.uv)*2.0f - 1.0f;
	wNormal = normalize(wNormal);
	float3 l = normalize(-directionLight.direction);
	float3 v = normalize(eyePos - wPos.xyz);
	float3 h = normalize(l + v);
	float Cosi = saturate(dot(wNormal, l));

	float3 brdf_diff = Diffuse(DiffuseColor, Roughness, saturate(dot(wNormal, v)), saturate(dot(wNormal, l)), saturate(dot(h, v)));
	float3 brdf_spec = (D(Roughness, saturate(dot(wNormal, h)))*F(saturate(dot(v, h)), SpecularColor)*G(Roughness, wNormal, l, v))/ (4 * dot(wNormal, l)*dot(wNormal, v));
		
	float3 Ldir_diff_spec = (all(ndcDepth))*(brdf_diff + brdf_spec) * El*Cosi*ShadowLightFract(wPos);

	result.finalColor = float4(Ldir_diff_spec, 1);

	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Lighting Pass End																//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


technique11 Lighting
{
	pass VoxelShadowStage
	{
		SetVertexShader(CompileShader(vs_5_0, MCVS()));
		SetGeometryShader(CompileShader(gs_5_0, ShadowMapGS()));
		SetPixelShader(CompileShader(ps_5_0, ShadowMapPS()));
	}
	pass ShadowStage
	{
		SetVertexShader(CompileShader(vs_5_0, ShadowMapVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, ShadowMapPS()));
	}
	pass DirectLightingStage
	{
		SetVertexShader(CompileShader(vs_5_0, LightVS()));
		SetGeometryShader(CompileShader(gs_5_0, LightGS()));
		SetPixelShader(CompileShader(ps_5_0, LightPS()));
	}
}
