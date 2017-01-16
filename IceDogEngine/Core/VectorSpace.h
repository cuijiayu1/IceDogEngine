#pragma once

#include "BasicSpace.h"

namespace IceDogCore
{
	// LH
	class VectorSpace:public BasicSpace
	{
	public:
		VectorSpace():c_forward(0,0,1),
						c_up(0,1,0),
						c_right(1,0,0),
						c_location(0,0,0),
						c_scale(1,1,1)
		{}
		// move forward in current direction
		void MoveForward(double distance) override 
		{
			c_location = c_forward*distance + c_location;
			MarkCoordinateDirty();
		}

		// move right in current direction
		void MoveRight(double distance) override
		{
			c_location = c_right*distance + c_location;
			MarkCoordinateDirty();
		}

		// ret the look at location
		float3 GetLookAtLocation() override
		{
			return c_location + c_forward;
		}

		// ret the location
		float3 GetLocation() override
		{
			return c_location;
		}

		/* get the rotation */
		float3 GetRotation() override
		{
			return c_rotation;
		}

		/* set the location */
		void SetLocation(const float3& location) override
		{
			c_location = location;
			MarkCoordinateDirty();
		}

		/* set the rotation */
		void SetRotation(const float3& rotate) override
		{
			c_rotation = rotate;
			UpdateForwardRightUp();
			MarkCoordinateDirty();
		}

		/* set the scale */
		void SetScale(const float3& scale) override
		{
			c_scale = scale;
			MarkCoordinateDirty();
		}

		// get the scale
		float3 GetScale() override { return c_scale; }

		/* get/set forward vector */
		float3 GetForward() override { return c_forward; }

		// get right vector
		float3 GetRight() override { return c_right; }

		// get up vector
		float3 GetUp() override { return c_up; }

	private:
		void UpdateForwardRightUp() 
		{
			float4x4& rotationMatrix = float4x4::GetRotationMatrix(c_rotation);
			// update forward
			float4 result = float4(0, 0, 1, 0)*rotationMatrix;
			c_forward = float3(result.x, result.y, result.z);
			// update up
			result = float4(0, 1, 0, 0)*rotationMatrix;
			c_up = float3(result.x, result.y, result.z);
			// update right
			result = float4(1, 0, 0, 0)*rotationMatrix;
			c_right = float3(result.x, result.y, result.z);
		}

	private:
		float3 c_forward; /* where to face */
		float3 c_right; /* where is right */
		float3 c_up; /* where is up */
		float3 c_location; /* where am i */
		float3 c_scale; /* what size */
		float3 c_rotation; /* to world rotation */
	};
}
