#pragma once
#include "../Common/UtilBasic.h"
#include "../../Rendering/PDRenderingDef.h"

#define Pi 3.141592653f

namespace IceDogUtils
{
	class MathHelper
	{
	public:
		/* cal the angle given x,y */
		static float AngleFromXY(float x, float y);

		/* cal the (A^-1)T */
		static IceDogRendering::float4x4 InverseTranspose(IceDogRendering::float4x4 M);
	};

}
