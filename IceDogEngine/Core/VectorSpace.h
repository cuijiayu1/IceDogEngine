#pragma once

#include "Coordinate.h"

namespace IceDogCore
{
	// LH
	class VectorSpace:public Coordinate
	{
	public:
		VectorSpace():c_forward(0,0,1),
						c_up(0,1,0),
						c_right(1,0,0),
						c_location(0,0,0)
		{}
		// move forward in current direction
		void MoveForward(double distance) override 
		{
			c_location = c_forward*distance + c_location;
			MarkCoordinateDirty();
		}

		// ret the look at location
		float3 GetLookAtLocation() override
		{
			return c_location + c_forward;
		}
		
		// get/set forward vector
		float3 GetForward() { return c_forward; }

		// get right vector
		float3 GetRight() { return c_right; }

		// get up vector
		float3 GetUp() { return c_up; }

	private:
		void UpdateForwardRightUp() {}

	private:
		float3 c_forward; /* where to face */
		float3 c_right; /* where is right */
		float3 c_up; /* where is up */
		float3 c_location; /* where am i */
	};
}
