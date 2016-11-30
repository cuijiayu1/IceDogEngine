#pragma once
#include "../Utils/Common/UtilBasic.h"

namespace IceDogRendering
{
	struct float2
	{
		float x;
		float y;

		float2(float _x, float _y):x(_x),y(_y) {}
		float2() :x(0), y(0) {}
		explicit float2(const float* data) :x(data[0]), y(data[1]) {}
		float2& operator= (const float2& Float2) { x = Float2.x; y = Float2.y; return *this; }
	};

	struct float3
	{
		float x;
		float y;
		float z;

		static float3 Zeros() { return float3(0, 0, 0); }
		float3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
		float3() :x(0), y(0),z(0) {}
		explicit float3(const float* data) :x(data[0]), y(data[1]), z(data[2]) {}
		float3& operator= (const float3& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
	};

	struct float4
	{
		float x;
		float y;
		float z;
		float w;

		float4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {}
		float4() :x(0), y(0), z(0),w(0) {}
		explicit float4(const float* data) :x(data[0]), y(data[1]), z(data[2]),w(data[3]) {}
		float4& operator= (const float4& Float4) { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
	};

	struct float4x4
	{
		float m[16];

		float4x4() {}

#if defined __DIRECTX__
		float4x4(XMFLOAT4X4 xmf)
		{
			m[0] = xmf._11;m[1] = xmf._12;m[2] = xmf._13;m[3] = xmf._14;
			m[4] = xmf._21;m[5] = xmf._22;m[6] = xmf._23;m[7] = xmf._24;
			m[8] = xmf._31;m[9] = xmf._32;m[10] = xmf._33;m[11] = xmf._34;
			m[12] = xmf._41;m[13] = xmf._42;m[14] = xmf._43;m[15] = xmf._44;
		}
#endif
		
		float4x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
			m[4] = m10; m[5] = m11; m[6] = m12; m[7] = m13;
			m[8] = m20; m[9] = m21; m[10] = m22; m[11] = m23;
			m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
		}

		static float4x4 Identity() 
		{
			return float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		}
	};

	struct Vertex
	{
		float3 position;
		float4 color;
		float4 normal;
		float2 tex0;
		float2 tex1;
	};

	namespace Color
	{
		const float White[]{ 1.0f, 1.0f, 1.0f, 1.0f };
		const float Black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		const float Red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		const float Green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
		const float Blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		const float Yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		const float Cyan[] = { 0.0f, 1.0f, 1.0f, 1.0f };
		const float Magenta[] = { 1.0f, 0.0f, 1.0f, 1.0f };

		const float Silver[] = { 0.75f, 0.75f, 0.75f, 1.0f };
		const float LightSteelBlue[] = { 0.69f, 0.77f, 0.87f, 1.0f };
	}
}

