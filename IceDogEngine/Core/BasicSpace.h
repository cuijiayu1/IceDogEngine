#pragma once

#include "../Utils/Math/MathDef.h"
#include "../Rendering/PDRenderingDef.h"

namespace IceDogCore
{
	using IceDogUtils::float3;
	using IceDogUtils::float4;
	using IceDogRendering::float4x4;

	class BasicSpace
	{
	protected:
		void MarkCoordinateDirty() { c_dirty = true; }

	public:
		// check coordinate is dirty or not
		bool IsCoordinateDirty() { return c_dirty; }
		// make the data clean
		void MarkCoordinateClean() { c_dirty = false; }
		// move the coordinate follow the forward direction
		virtual void MoveForward(double distance) = 0;
		// move right in current direction
		virtual void MoveRight(double distance) = 0;
		// get the look at location
		virtual float3 GetLookAtLocation() = 0;
		// get the location of the space
		virtual float3 GetLocation() = 0;
		// set the location of the space
		virtual void SetLocation(const float3& location) = 0;
		// get the scale of the space
		virtual float3 GetScale() = 0;
		// set the scale of the space
		virtual void SetScale(const float3& scale) = 0;
		// set the rotation of the space
		virtual void SetRotation(const float3& rotate) = 0;
		// get the rotation of the space
		virtual float3 GetRotation() = 0;
		// get the rotation in rads
		virtual float3 GetRotationRad() = 0;
		// get forward vector
		virtual float3 GetForward() = 0;
		// get right vector
		virtual float3 GetRight() = 0;
		// get up vector
		virtual float3 GetUp() = 0;
	protected:
		bool c_dirty;
   	};
}
