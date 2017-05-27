#include "ObjMeshLoader.h"
#include "../../Rendering/PDRenderingDef.h"
#include <fstream>
#include <sstream>

bool IceDogUtils::ObjMeshLoader::LoadObj2Mesh(std::string url, std::shared_ptr<IceDogRendering::MeshData> rd)
{
	using IceDogRendering::Vertex;
	using IceDogUtils::float3;
	using IceDogUtils::float2;
	using IceDogUtils::float4;

	std::ifstream inFile;
	std::string line, v = "v", vt = "vt", vn = "vn", f = "f";
	unsigned int lineCount = 0;
	std::vector<float3> vs;
	std::vector<float3> vns;
	std::vector<float2> vts;
	std::vector<std::string> faces;

	inFile.open(url);

	if (!inFile.is_open()) {
		return false;
	}
	while (getline(inFile, line)) // we get a new line to process, here.
	{
		std::istringstream streamLine(line);
		std::string token;

		if (++lineCount == 33) {
			lineCount = lineCount;
		}

		streamLine >> token;

		if (token == v) {
			float3 vert;
			streamLine >> vert.x;
			streamLine >> vert.y;
			streamLine >> vert.z;
			vs.push_back(vert);
		}
		else if (token == vt) {
			float2 coord;
			streamLine >> coord.x;
			streamLine >> coord.y;
			vts.push_back(coord);
		}
		else if (token == vn) {
			float3	norm;
			streamLine >> norm.x;
			streamLine >> norm.y;
			streamLine >> norm.z;
			vns.push_back(norm);
		}
		else if (token == f)
		{
			while (streamLine.good())
			{
				std::string chunk;
				std::string empty = "";  //this empty string is basically to cover the case where
				streamLine >> chunk;
				if (chunk == empty) continue;
				faces.push_back(chunk);
			}
		}
	}
	// create vertex data buffer
	Vertex* vertexs = new Vertex[faces.size()];
	// create the indices data buffer
	unsigned int* indsbf = new unsigned int[faces.size()];

	for (size_t i = 0; i < faces.size(); ++i)
	{
		int vInd, tInd = 0, nInd = 0;
		char dummy;			//there is whitespace after the data.
		std::istringstream streamChunk(faces[i]);

		streamChunk >> vInd;
		vertexs[i].position = vs[vInd - 1];
		vertexs[i].color = float4(0.7, 0.7, 0.7, 1);
		indsbf[i] = i;

		streamChunk >> dummy;
		if (vts.size() != 0) {
			streamChunk >> tInd;
			vertexs[i].tex0 = vts[tInd - 1];
		}

		streamChunk >> dummy;
		if (vns.size() != 0) {
			streamChunk >> nInd;
			vertexs[i].normal = vns[nInd - 1];
		}
	}
	rd->SetIndexData(indsbf, faces.size() / 3);
	rd->SetVertexData(vertexs, faces.size());
	

	inFile.close();

	vs.clear();
	vts.clear();
	vns.clear();

	return true;
}
