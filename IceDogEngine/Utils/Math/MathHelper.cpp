#include "MathHelper.h"

using namespace IceDogUtils;

float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f*Pi; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + Pi; // in [0, 2*pi).

	return theta;
}

IceDogRendering::float4x4 MathHelper::InverseTranspose(IceDogRendering::float4x4 M)
{
#if defined __DIRECTX__
	XMMATRIX A= XMLoadFloat4x4(&(XMFLOAT4X4(M.m)));
	A.r[3] = XMVectorSet(0, 0, 0, 1);
	XMVECTOR det = XMMatrixDeterminant(A);
	XMMATRIX result= XMMatrixTranspose(XMMatrixInverse(&det, A));
	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, result);
	return IceDogRendering::float4x4(temp);
#endif
}