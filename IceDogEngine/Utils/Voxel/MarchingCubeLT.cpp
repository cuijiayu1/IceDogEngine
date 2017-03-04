#include "MarchingCubeLT.h"


int IceDogAlgorithm::PolygoniseArray(std::vector<std::vector<std::vector<float>>>& arrayDatas, const double isolevel, std::shared_ptr<IceDogRendering::RenderData> rd)
{
	int w = arrayDatas.size();
	int h = arrayDatas[0].size();
	int d = arrayDatas[0][0].size();
	std::vector<IceDogRendering::Vertex> vts;
	int cubeindex;

	for (int i = 0; i < w - 1; ++i)
	{
		for (int j = 0; j < h - 1; ++j)
		{
			for (int k = 0; k < d - 1; ++k)
			{
				IceDogUtils::float3 vertlist[12];

				cubeindex = 0;
				if (arrayDatas[i][j + 1][k + 1] < isolevel) cubeindex |= 1;
				if (arrayDatas[i + 1][j + 1][k + 1] < isolevel) cubeindex |= 2;
				if (arrayDatas[i + 1][j + 1][k] < isolevel) cubeindex |= 4;
				if (arrayDatas[i][j + 1][k] < isolevel) cubeindex |= 8;
				if (arrayDatas[i][j][k + 1] < isolevel) cubeindex |= 16;
				if (arrayDatas[i + 1][j][k + 1] < isolevel) cubeindex |= 32;
				if (arrayDatas[i + 1][j][k] < isolevel) cubeindex |= 64;
				if (arrayDatas[i][j][k] < isolevel) cubeindex |= 128;

				/* Cube is entirely in/out of the surface */
				if (edgeTable[cubeindex] == 0)
					continue;

				/* Find the vertices where the surface intersects the cube */
				if (edgeTable[cubeindex] & 1)
					vertlist[0] =
					VertexInterp(isolevel, float3(i,j+1,k+1), float3(i+1, j + 1, k + 1), arrayDatas[i][j + 1][k + 1], arrayDatas[i + 1][j + 1][k + 1]);
				if (edgeTable[cubeindex] & 2)
					vertlist[1] =
					VertexInterp(isolevel, float3(i+1, j + 1, k + 1), float3(i + 1, j + 1, k), arrayDatas[i + 1][j + 1][k + 1], arrayDatas[i + 1][j + 1][k]);
				if (edgeTable[cubeindex] & 4)
					vertlist[2] =
					VertexInterp(isolevel, float3(i + 1, j + 1, k), float3(i, j + 1, k), arrayDatas[i + 1][j + 1][k], arrayDatas[i][j + 1][k]);
				if (edgeTable[cubeindex] & 8)
					vertlist[3] =
					VertexInterp(isolevel, float3(i, j + 1, k), float3(i,j+1,k+1), arrayDatas[i][j + 1][k], arrayDatas[i][j + 1][k + 1]);
				if (edgeTable[cubeindex] & 16)
					vertlist[4] =
					VertexInterp(isolevel, float3(i, j, k+1), float3(i+1, j, k + 1), arrayDatas[i][j][k + 1], arrayDatas[i + 1][j][k + 1]);
				if (edgeTable[cubeindex] & 32)
					vertlist[5] =
					VertexInterp(isolevel, float3(i+1, j, k + 1), float3(i + 1, j, k), arrayDatas[i + 1][j][k + 1], arrayDatas[i + 1][j][k]);
				if (edgeTable[cubeindex] & 64)
					vertlist[6] =
					VertexInterp(isolevel, float3(i + 1, j, k), float3(i, j, k), arrayDatas[i + 1][j][k], arrayDatas[i][j][k]);
				if (edgeTable[cubeindex] & 128)
					vertlist[7] =
					VertexInterp(isolevel, float3(i, j, k), float3(i, j, k+1), arrayDatas[i][j][k], arrayDatas[i][j][k + 1]);
				if (edgeTable[cubeindex] & 256)
					vertlist[8] =
					VertexInterp(isolevel, float3(i,j+1,k+1), float3(i, j, k+1), arrayDatas[i][j + 1][k + 1], arrayDatas[i][j][k + 1]);
				if (edgeTable[cubeindex] & 512)
					vertlist[9] =
					VertexInterp(isolevel, float3(i+1, j + 1, k + 1), float3(i+1, j, k + 1), arrayDatas[i + 1][j + 1][k + 1], arrayDatas[i + 1][j][k + 1]);
				if (edgeTable[cubeindex] & 1024)
					vertlist[10] =
					VertexInterp(isolevel, float3(i + 1, j + 1, k), float3(i + 1, j, k), arrayDatas[i + 1][j + 1][k], arrayDatas[i + 1][j][k]);
				if (edgeTable[cubeindex] & 2048)
					vertlist[11] =
					VertexInterp(isolevel, float3(i, j + 1, k), float3(i, j, k), arrayDatas[i][j + 1][k], arrayDatas[i][j][k]);

				/* Create the triangle */
				for (int ntr = 0; triTable[cubeindex][ntr] != -1; ntr += 3) {
					IceDogRendering::Vertex tri[3];
					tri[0].position = vertlist[triTable[cubeindex][ntr]];
					tri[1].position = vertlist[triTable[cubeindex][ntr + 1]];
					tri[2].position = vertlist[triTable[cubeindex][ntr + 2]];
					float3 normal = IceDogUtils::float3Cross(tri[2].position - tri[0].position, tri[1].position - tri[0].position).Normilize();
					tri[0].normal = normal;
					tri[1].normal = normal;
					tri[2].normal = normal;
					vts.push_back(tri[1]);
					vts.push_back(tri[0]);
					vts.push_back(tri[2]);
				}
			}
		}
	}
	IceDogRendering::Vertex* vtsOut = new IceDogRendering::Vertex[vts.size()];
	for (int i = 0; i < vts.size(); ++i)
	{
		vtsOut[i] = vts[i];
	}
	rd->SetVertexData(vtsOut, vts.size());
	unsigned int* idsOut = new unsigned int[vts.size()];
	for (int i = 0; i < vts.size(); ++i)
	{
		idsOut[i] = i;
	}
	rd->SetIndexData(idsOut, vts.size() / 3);

	return vts.size()/3;
}

IceDogUtils::float3 IceDogAlgorithm::VertexInterp(double isolevel, float3 p1, float3 p2, double valp1, double valp2)
{
	double mu;
	float3 p;

	if (abs(isolevel - valp1) < 0.00001)
		return(p1);
	if (abs(isolevel - valp2) < 0.00001)
		return(p2);
	if (abs(valp1 - valp2) < 0.00001)
		return(p1);
	mu = (isolevel - valp1) / (valp2 - valp1);
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);

	return(p);
}