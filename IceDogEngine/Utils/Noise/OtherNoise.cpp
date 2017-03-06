#include "OtherNoise.h"

IceDogUtils::float4 IceDogAlgorithm::NoiseSampleInPlace(IceDogUtils::float4 p)
{
	return IceDogUtils::float4(
		16.0*p.y*p.z +
		4.0*2.0*p.x,
		16.0*p.x*p.z +
		4.0*2.0*p.y,
		16.0*p.x*p.y +
		4.0*2.0*p.z,
		16.0*p.x*p.y*p.z +
		4.0*p.x*p.x +
		4.0*p.y*p.y +
		4.0*p.z*p.z - 1.0
	);
}
