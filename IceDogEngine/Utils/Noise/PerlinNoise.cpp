#include <random>
#include "PerlinNoise.h"

void IceDogAlgorithm::CreateNoiseTable(const int w, const int h, const int z, std::vector<std::vector<std::vector<IceDogUtils::float3>>>& noiseTab)
{
	// alloc for memory
	noiseTab.resize(w);
	for (int i=0;i<w;++i)
	{
		noiseTab[i].resize(h);
	}
	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			noiseTab[i][j].resize(z);
		}
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1, 1);
	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < z; ++k)
			{
				noiseTab[i][j][k] = IceDogUtils::float3(dis(gen), dis(gen), dis(gen)).Normilize();
			}
		}
	}
}

void IceDogAlgorithm::SamplePerlinNoiseData(const std::vector<std::vector<std::vector<IceDogUtils::float3>>>& noiseTab,
											const int w, const int h, const int z, 
											std::vector<std::vector<std::vector<float>>>& noiseData)
{

	// alloc the noise data memory
	noiseData.resize(w);
	for (int i = 0; i < w; ++i)
	{
		noiseData[i].resize(h);
	}
	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			noiseData[i][j].resize(z);
		}
	}

	// begin sample
	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < z; ++k)
			{
				int poi = i /float(w) * (noiseTab.size() - 1.5);
				int poj = j /float(h) * (noiseTab[0].size() - 1.5);
				int pok = k /float(z) * (noiseTab[0][0].size() - 1.5);
				float smpVal = 0;
				IceDogUtils::float3 smpPoint(i / float(w) * (noiseTab.size() - 0.5), j / float(h) * (noiseTab[0].size() - 0.5), k / float(z) * (noiseTab[0][0].size() - 0.5));

				smpVal += IceDogUtils::float3Dot(smpPoint - IceDogUtils::float3(poi, poj, pok), noiseTab[poi][poj][pok]);
				smpVal += IceDogUtils::float3Dot(smpPoint - IceDogUtils::float3(poi + 1, poj, pok), noiseTab[poi + 1][poj][pok]);
				smpVal += IceDogUtils::float3Dot(smpPoint - IceDogUtils::float3(poi + 1, poj, pok + 1), noiseTab[poi + 1][poj][pok + 1]);
				smpVal += IceDogUtils::float3Dot(smpPoint - IceDogUtils::float3(poi, poj, pok + 1), noiseTab[poi][poj][pok + 1]);

				smpVal += IceDogUtils::float3Dot(smpPoint - IceDogUtils::float3(poi, poj + 1, pok), noiseTab[poi][poj + 1][pok]);
				smpVal += IceDogUtils::float3Dot(smpPoint - IceDogUtils::float3(poi + 1, poj + 1, pok), noiseTab[poi + 1][poj + 1][pok]);
				smpVal += IceDogUtils::float3Dot(smpPoint - IceDogUtils::float3(poi + 1, poj + 1, pok + 1), noiseTab[poi + 1][poj + 1][pok + 1]);
				smpVal += IceDogUtils::float3Dot(smpPoint - IceDogUtils::float3(poi, poj + 1, pok + 1), noiseTab[poi][poj + 1][pok + 1]);
				noiseData[i][j][k] = smpVal;
			}
		}
	}
}

