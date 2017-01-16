#pragma once

#include "../Utils/Math/MathDef.h"

namespace IceDogCore
{
	using IceDogUtils::float3;

	class Coordinate
	{
	protected:
		void MarkCoordinateDirty() { c_dirty = true; }
		void MarkCoordinateClean() { c_dirty = false; }

		// move the coordinate follow the forward direction
		virtual void MoveForward(double distance) = 0;
		// get the look at location
		virtual float3 GetLookAtLocation() = 0;
	protected:
		bool c_dirty;
	};
}
