#include "LightDefination.hlsli"

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

[maxvertexcount(16)]
void main(point MCVsIn gIn[1], inout TriangleStream<ShadowMapVSOut> triStream)
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