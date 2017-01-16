#pragma once
#include "../Common/UtilBasic.h"

namespace IceDogUtils
{
	struct float2
	{
		float x;
		float y;

		float2(float _x, float _y) :x(_x), y(_y) {}
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
		float3 Normilize()
		{
			float length = sqrtf(x*x + y*y + z*z);
			return float3(x / length, y / length, z / length);
		}
		float3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
		float3() :x(0), y(0), z(0) {}
		explicit float3(const float* data) :x(data[0]), y(data[1]), z(data[2]) {}
		float3& operator= (const float3& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
	};
	inline float3 operator*(const float3& pos, double dist) { return float3(pos.x*dist, pos.y*dist, pos.z*dist); }
	inline float3 operator*(double dist, const float3& pos) { return float3(pos.x*dist, pos.y*dist, pos.z*dist); }
	inline float3 operator+(const float3& f1, const float3& f2) { return float3(f1.x + f2.x, f1.y + f2.y, f1.z + f2.z); }

	struct float4
	{
		float x;
		float y;
		float z;
		float w;

		float4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {}
		float4(const float3& f3, float _w) :x(f3.x), y(f3.y), z(f3.z), w(_w) {}
		float4() :x(0), y(0), z(0), w(0) {}
		explicit float4(const float* data) :x(data[0]), y(data[1]), z(data[2]), w(data[3]) {}
		float4& operator= (const float4& Float4) { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
	};

	template<typename T, int vecSize>
	class Vector
	{
	public:
		T _data[vecSize];
	public:
		T operator[](size_t place)
		{
			assert(place >= 0 && place < vecSize);
			return _data[place];
		}
		Vector() {};
		Vector(const T(&inidata)[vecSize])
		{
			for (size_t i = 0; i < vecSize; i++)
			{
				_data[i] = inidata[i];
			}
		}

		Vector Normilize()
		{
			T sum = 0;
			for (size_t i = 0; i < vecSize; i++)
			{
				sum += _data[i] * _data[i];
			}
			T length = sqrtl(sum);
			T tempArray[vecSize];
			for (size_t i = 0; i < vecSize; i++)
			{
				tempArray[i] = _data[i] / length;
			}
			return Vector(tempArray);
		}

		static Vector Zero()
		{
			T temp[vecSize] = { 0 };
			return Vector(temp);
		}
		// get the vector size
		constexpr size_t Length()
		{
			return vecSize;
		}

		Vector& operator=(const Vector& source)
		{
			for (size_t i = 0; i < vecSize; i++)
			{
				_data[i] = source._data[i];
			}
			return *this;
		}
	};
}