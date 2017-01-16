#pragma once
#include "../Utils/Common/UtilBasic.h"
#include "../Utils/Math/MathDef.h"

namespace IceDogRendering
{
	using IceDogUtils::float2;
	using IceDogUtils::float3;
	using IceDogUtils::float4;
	using IceDogUtils::Vector;

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

		static float4x4 GetRotationMatrix(const float3& rotate)
		{
#if defined __DIRECTX__
			DirectX::XMFLOAT4X4 temp4x4;
			DirectX::XMStoreFloat4x4(&temp4x4,DirectX::XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z));
			return float4x4(temp4x4);
#endif
			return Identity();
		}

		static float4x4 FromSRT(const float3& scale, const float3& rotate, const float3& transform)
		{
#if defined __DIRECTX__
			DirectX::XMFLOAT4X4 retMat;
			auto mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
			auto mRotate = DirectX::XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
			auto mTransf = DirectX::XMMatrixTranslation(transform.x, transform.y, transform.z);
			DirectX::XMStoreFloat4x4(&retMat, mScale*mRotate*mTransf);
			return float4x4(retMat);
#endif
			return Identity();
		}
	};
	inline float4 operator*(const float4& v1,const float4x4& mat)
	{
		return float4(v1.x*mat.m[0] + v1.y*mat.m[4] + v1.z*mat.m[8] + v1.w*mat.m[12],
			v1.x*mat.m[1] + v1.y*mat.m[5] + v1.z*mat.m[9] + v1.w*mat.m[13],
			v1.x*mat.m[2] + v1.y*mat.m[6] + v1.z*mat.m[10] + v1.w*mat.m[14],
			v1.x*mat.m[3] + v1.y*mat.m[7] + v1.z*mat.m[11] + v1.w*mat.m[15]);
	}

	struct Vertex
	{
		float3 position;
		float4 color;
		float3 normal;
		float3 tangentU;
		float2 tex0;
		float2 tex1;

		Vertex() {}

		Vertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: position(px, py, pz), normal(nx, ny, nz),
			tangentU(tx, ty, tz), tex0(u, v),color(0.2,0.2,0.2,1) {}
	};

	struct DirectionalLight
	{
		DirectionalLight()
		{
			ambient = float4(0.2, 0.2, 0.2, 1);
			diffuse = float4(0.5, 0.5, 0.5, 1);
			specular = float4(0.5, 0.5, 0.5, 1);
			direction = float3(0.57735f, -0.57735f, 0.57735f);
		}

		float4 ambient;
		float4 diffuse;
		float4 specular;
		float3 direction;
		float pad;
	};

	struct PointLight
	{
		PointLight()
		{
			ambient = float4(0.3f, 0.3f, 0.3f, 1);
			diffuse = float4(0.7f, 0.7f, 0.7f, 1);
			specular = float4(0.7f, 0.7f, 0.7f, 1);
			att = float3(0, 0.4, 0);
			range = 25;
			position = float3(0, 0, 0);
		}

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
		SpotLight()
		{
			ambient = float4(0.3, 0.3, 0.3, 1);
			diffuse = float4(0.7, 0.7, 0.7, 1);
			specular = float4(0.6, 0.6, 0.6, 1);
			att = float3(0, 0.1, 0);
			spot = 96.0f;
			range = 25;
			position = float3(1, 1, 1);
			direction = float3(-1, -1, -1);
		}
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

	struct Material
	{
		float4 ambient;
		float4 diffuse;
		float4 specular;
		float4 reflect;
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

