#include "GeometryGenerator.h"
#include "Geometry.h"
#include "../../Utils/Math/MathHelper.h"

using namespace IceDogResources;
using namespace IceDogGeometry;

void GeometryGenerator::CreateSphere(float radius, unsigned int sliceCount, unsigned int stackCount, std::shared_ptr<IceDogRendering::MeshData> rd)
{
	using namespace IceDogRendering;
	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	bottomVertex.color = float4(0.95, 0, 0, 1);
	topVertex.color = float4(0.95, 0, 0, 1);

	std::vector<Vertex> Vertices;
	Vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f*XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (unsigned int i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (unsigned int j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;

			// spherical to artesian
			v.position.x = radius*sinf(phi)*cosf(theta);
			v.position.y = radius*cosf(phi);
			v.position.z = radius*sinf(phi)*sinf(theta);
			v.color = float4(0.95, 0, 0, 1);

			v.normal = v.position.Normilize();

			v.tex0.x = theta / XM_2PI;
			v.tex0.y = phi / XM_PI;

			Vertices.push_back(v);
		}
	}

	Vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	std::vector<unsigned int> Indices;

	for (unsigned int i = 1; i <= sliceCount; ++i)
	{
		Indices.push_back(0);
		Indices.push_back(i + 1);
		Indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	unsigned int baseIndex = 1;
	unsigned int ringVertexCount = sliceCount + 1;
	for (unsigned int i = 0; i < stackCount - 2; ++i)
	{
		for (unsigned int j = 0; j < sliceCount; ++j)
		{
			Indices.push_back(baseIndex + i*ringVertexCount + j);
			Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	unsigned int southPoleIndex = Vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (unsigned int i = 0; i < sliceCount; ++i)
	{
		Indices.push_back(southPoleIndex);
		Indices.push_back(baseIndex + i);
		Indices.push_back(baseIndex + i + 1);
	}

	Vertex* vd = new Vertex[Vertices.size()];
	unsigned int* inds = new unsigned int[Indices.size()];

	for (size_t i = 0; i < Vertices.size(); i++)
		vd[i] = Vertices[i];
	for (size_t i = 0; i < Indices.size(); i++)
		inds[i] = Indices[i];
	rd->SetVertexData(vd, Vertices.size());
	rd->SetIndexData(inds, Indices.size() / 3);
	rd->MarkDataStateDirty();
}

void GeometryGenerator::Subdivide(std::shared_ptr<IceDogRendering::MeshData> rd)
{
	using namespace IceDogRendering;
	// Save a copy of the input geometry.
	std::vector<unsigned int> IndicesCopy;
	for (int i = 0; i < rd->GetTriangleCount() * 3; ++i)
	{
		IndicesCopy.push_back(rd->GetIndexData().get()[i]);
	}
	std::vector<Vertex> VertexsCopy;
	for (int i = 0; i < rd->GetVertexCount(); ++i)
	{
		VertexsCopy.push_back(rd->GetVertexData().get()[i]);
	}



	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	unsigned int numTris = IndicesCopy.size() / 3;
	for (unsigned int i = 0; i < numTris; ++i)
	{
		Vertex v0 = VertexsCopy[IndicesCopy[i * 3 + 0]];
		Vertex v1 = VertexsCopy[IndicesCopy[i * 3 + 1]];
		Vertex v2 = VertexsCopy[IndicesCopy[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		Vertex m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.position = float3(
			0.5f*(v0.position.x + v1.position.x),
			0.5f*(v0.position.y + v1.position.y),
			0.5f*(v0.position.z + v1.position.z));

		m1.position = float3(
			0.5f*(v1.position.x + v2.position.x),
			0.5f*(v1.position.y + v2.position.y),
			0.5f*(v1.position.z + v2.position.z));

		m2.position = float3(
			0.5f*(v0.position.x + v2.position.x),
			0.5f*(v0.position.y + v2.position.y),
			0.5f*(v0.position.z + v2.position.z));

		//
		// Add new geometry.
		//

		VertexsCopy.push_back(v0); // 0
		VertexsCopy.push_back(v1); // 1
		VertexsCopy.push_back(v2); // 2
		VertexsCopy.push_back(m0); // 3
		VertexsCopy.push_back(m1); // 4
		VertexsCopy.push_back(m2); // 5

		IndicesCopy.push_back(i * 6 + 0);
		IndicesCopy.push_back(i * 6 + 3);
		IndicesCopy.push_back(i * 6 + 5);

		IndicesCopy.push_back(i * 6 + 3);
		IndicesCopy.push_back(i * 6 + 4);
		IndicesCopy.push_back(i * 6 + 5);

		IndicesCopy.push_back(i * 6 + 5);
		IndicesCopy.push_back(i * 6 + 4);
		IndicesCopy.push_back(i * 6 + 2);

		IndicesCopy.push_back(i * 6 + 3);
		IndicesCopy.push_back(i * 6 + 1);
		IndicesCopy.push_back(i * 6 + 4);
	}

	unsigned int* newIndexs = new unsigned int[IndicesCopy.size()];
	for (size_t i = 0; i < IndicesCopy.size(); ++i)
	{
		newIndexs[i] = IndicesCopy[i];
	}

	Vertex* newVertexs = new Vertex[VertexsCopy.size()];
	for (size_t i = 0; i < VertexsCopy.size(); i++)
	{
		newVertexs[i] = VertexsCopy[i];
	}
	rd->SetIndexData(newIndexs, IndicesCopy.size() / 3);
	rd->SetVertexData(newVertexs, VertexsCopy.size());
}

void IceDogResources::IceDogGeometry::GeometryGenerator::CreateTeapot(int tess, float size, bool rhcoords, std::shared_ptr<IceDogRendering::MeshData> rd)
{
	DirectX::VertexCollection vc;
	DirectX::IndexCollection ic;
	DirectX::ComputeTeapot(vc, ic, size, tess, rhcoords);
	using IceDogRendering::Vertex;
	Vertex* vxs = new Vertex[vc.size()];
	unsigned int* ixs = new unsigned int[ic.size()];
	for (size_t i = 0; i < vc.size(); ++i)
	{
		vxs[i].color = IceDogUtils::float4(IceDogRendering::Color::Silver);
		vxs[i].normal = IceDogUtils::float3(vc[i].normal.x, vc[i].normal.y, vc[i].normal.z);
		vxs[i].position = IceDogUtils::float3(vc[i].position.x, vc[i].position.y, vc[i].position.z);
		vxs[i].tex0 = IceDogUtils::float2(vc[i].textureCoordinate.x, vc[i].textureCoordinate.y);
	}
	for (size_t i=0;i<ic.size();i++)
	{
		ixs[i] = ic[i];
	}
	rd->SetIndexData(ixs, ic.size() / 3);
	rd->SetVertexData(vxs, vc.size());
}


void GeometryGenerator::CreateGeosphere(float radius, unsigned int numSubdivisions, std::shared_ptr<IceDogRendering::MeshData> rd)
{
	using namespace IceDogRendering;
	// Put a cap on the number of subdivisions.
	numSubdivisions = min(numSubdivisions, 5u);

	// Approximate a sphere by tessellating an icosahedron.

	const float X = 0.525731f;
	const float Z = 0.850651f;

	float3 pos[12] =
	{
		float3(-X, 0.0f, Z),  float3(X, 0.0f, Z),
		float3(-X, 0.0f, -Z), float3(X, 0.0f, -Z),
		float3(0.0f, Z, X),   float3(0.0f, Z, -X),
		float3(0.0f, -Z, X),  float3(0.0f, -Z, -X),
		float3(Z, X, 0.0f),   float3(-Z, X, 0.0f),
		float3(Z, -X, 0.0f),  float3(-Z, -X, 0.0f)
	};

	unsigned long k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	Vertex* Vertices = new Vertex[12];
	unsigned int* Indices = new unsigned int[60];

	for (unsigned int i = 0; i < 12; ++i)
		Vertices[i].position = pos[i];

	for (unsigned int i = 0; i < 60; ++i)
		Indices[i] = k[i];

	rd->SetIndexData(Indices, 60 / 3);
	rd->SetVertexData(Vertices, 12);

	for (unsigned int i = 0; i < numSubdivisions; ++i)
		Subdivide(rd);

	// Project vertices onto sphere and scale.
	for (unsigned int i = 0; i < rd->GetVertexCount(); ++i)
	{
		// Project onto unit sphere.
		float3 n = rd->GetVertexData().get()[i].position.Normilize();

		// Project onto sphere.
		float3 p(radius*n.x, radius*n.y, radius*n.z);

		rd->GetVertexData().get()[i].position = p;
		rd->GetVertexData().get()[i].normal = n;

		// Derive texture coordinates from spherical coordinates.
		float theta = IceDogUtils::MathHelper::AngleFromXY(
			rd->GetVertexData().get()[i].position.x,
			rd->GetVertexData().get()[i].position.z);

		float phi = acosf(rd->GetVertexData().get()[i].position.y / radius);

		rd->GetVertexData().get()[i].tex0.x = theta / XM_2PI;
		rd->GetVertexData().get()[i].tex0.y = phi / XM_PI;
	}
}

void GeometryGenerator::CreateHill(float width, float depth, unsigned int m, unsigned int n, std::shared_ptr<IceDogRendering::MeshData> rd)
{
	auto GetNormal = [](float x, float z)->IceDogRendering::float3
	{
		IceDogRendering::float3 n(
			-0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
			1.0f,
			-0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));
		return n.Normilize();
	};

	auto GetHeight = [](float x, float z)->float
	{
		return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
	};

	CreateGrid(width, depth, m, n, rd);
	auto VertexData = rd->GetVertexData();
	
	for (int i = 0; i <rd->GetVertexCount(); ++i)
	{
		IceDogRendering::float3 p = VertexData.get()[i].position;

		p.y = GetHeight(p.x, p.z);

		VertexData.get()[i].position = p;
		VertexData.get()[i].normal = GetNormal(p.x, p.z);
	}
}

void GeometryGenerator::CreateGrid(float width, float depth, unsigned int m, unsigned int n, std::shared_ptr<IceDogRendering::MeshData> rd)
{
	unsigned int vertexCount = m*n;
	unsigned int faceCount = (m - 1)*(n - 1) * 2;

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	IceDogRendering::Vertex* Vertices = new IceDogRendering::Vertex[vertexCount];
	for (unsigned int i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for (unsigned int j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			Vertices[i*n + j].position = IceDogRendering::float3(x, 0.0f, z);
			Vertices[i*n + j].normal = IceDogRendering::float3(0.0f, 1.0f, 0.0f);
			Vertices[i*n + j].color = IceDogUtils::float4(IceDogRendering::Color::Silver);

			// Stretch texture over grid.
			Vertices[i*n + j].tex0.x = j*du;
			Vertices[i*n + j].tex0.y = i*dv;
		}
	}
	rd->SetVertexData(Vertices, vertexCount);

	//
	// Create the indices.
	//


	unsigned int* Indices = new unsigned int[faceCount * 3];
											// Iterate over each quad and compute indices.
	unsigned int k = 0;
	for (unsigned int i = 0; i < m - 1; ++i)
	{
		for (unsigned int j = 0; j < n - 1; ++j)
		{
			Indices[k] = i*n + j;
			Indices[k + 1] = i*n + j + 1;
			Indices[k + 2] = (i + 1)*n + j;

			Indices[k + 3] = (i + 1)*n + j;
			Indices[k + 4] = i*n + j + 1;
			Indices[k + 5] = (i + 1)*n + j + 1;

			k += 6; // next quad
		}
	}
	rd->SetIndexData(Indices, faceCount);
}

void GeometryGenerator::CreateBox(float width, float height, float depth, std::shared_ptr<IceDogRendering::MeshData> rd)
{
	//
	// Create the vertices.
	//

	IceDogRendering::Vertex* v=new IceDogRendering::Vertex[24];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	// Fill in the front face vertex data.
	v[0] = IceDogRendering::Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = IceDogRendering::Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = IceDogRendering::Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[3] = IceDogRendering::Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = IceDogRendering::Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[5] = IceDogRendering::Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = IceDogRendering::Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[7] = IceDogRendering::Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = IceDogRendering::Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[9] = IceDogRendering::Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[10] = IceDogRendering::Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[11] = IceDogRendering::Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = IceDogRendering::Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[13] = IceDogRendering::Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = IceDogRendering::Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[15] = IceDogRendering::Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = IceDogRendering::Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[17] = IceDogRendering::Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[18] = IceDogRendering::Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[19] = IceDogRendering::Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = IceDogRendering::Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[21] = IceDogRendering::Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[22] = IceDogRendering::Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[23] = IceDogRendering::Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	rd->SetVertexData(v, 24);

	//
	// Create the indices.
	//

	unsigned int* i=new unsigned int[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	rd->SetIndexData(i, 12);
}
