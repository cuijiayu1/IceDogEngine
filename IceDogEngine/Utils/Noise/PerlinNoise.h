#pragma once

#include "../Math/MathDef.h"

namespace IceDogAlgorithm
{
	/* create a perlin noise look up table */
	void CreateNoiseTable(const int w, const int h, const int z, std::vector<std::vector<std::vector<IceDogUtils::float3>>>& noiseTab);

	/* sample perlin noise data from a perlin noise look up table */
	void SamplePerlinNoiseData(const std::vector<std::vector<std::vector<IceDogUtils::float3>>>& noiseTab,
							   const int w, const int h, const int z,
							   std::vector<std::vector<std::vector<float>>>& noiseData);
}