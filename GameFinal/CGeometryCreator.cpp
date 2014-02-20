#include "stdafx.h"
#include "CGeometryCreator.h"


void CGeometryCreator::createCubeData(
	f32 width,
	f32 height,
	f32 depth,
	SGeometryData& geoData)
{
	geoData.Vertices.resize(24);
	geoData.Indices.resize(36);

	f32 w2 = 0.5f*width;
	f32 h2 = 0.5f*height;
	f32 d2 = 0.5f*depth;

	SGeometryVertex* v = &geoData.Vertices[0];

	// Fill in the front face vertex data.
	v[0] = SGeometryVertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = SGeometryVertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = SGeometryVertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = SGeometryVertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = SGeometryVertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = SGeometryVertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = SGeometryVertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = SGeometryVertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = SGeometryVertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = SGeometryVertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = SGeometryVertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = SGeometryVertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = SGeometryVertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = SGeometryVertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = SGeometryVertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = SGeometryVertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = SGeometryVertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = SGeometryVertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = SGeometryVertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = SGeometryVertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = SGeometryVertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = SGeometryVertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = SGeometryVertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = SGeometryVertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	u16* i = &geoData.Indices[0];

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
}

void CGeometryCreator::createPlaneData(
	f32 width,
	f32 depth,
	u32 xsegments,
	u32 ysegments,
	f32 uTiles,
	f32 vTiles,
	SGeometryData& geoData)
{
	UINT vertexCount = (xsegments + 1) * (ysegments + 1);
	UINT faceCount = xsegments * ysegments * 2;

	f32 halfWidth = 0.5f * width;
	f32 halfDepth = 0.5f * depth;

	f32 dx = width / xsegments;
	f32 dz = depth / ysegments;

	f32 du = uTiles / xsegments;
	f32 dv = vTiles / ysegments;

	geoData.Vertices.resize(vertexCount);
	for (u32 i = 0; i <= ysegments; ++i)
	{
		f32 z = halfDepth - i * dz;
		for (u32 j = 0; j <= xsegments; ++j)
		{
			f32 x = -halfWidth + j * dx;
			u32 index = i * (xsegments + 1) + j;
			geoData.Vertices[index].Position = XMFLOAT3(x, 0.0f, z);
			geoData.Vertices[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			geoData.Vertices[index].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);
			geoData.Vertices[index].TexC.x = j * du;
			geoData.Vertices[index].TexC.y = i * dv;
		}
	}

	geoData.Indices.resize(faceCount * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < ysegments; ++i)
	{
		for (UINT j = 0; j < xsegments; ++j)
		{
			geoData.Indices[k] = i * (xsegments + 1) + j;
			geoData.Indices[k + 1] = i * (xsegments + 1) + j + 1;
			geoData.Indices[k + 2] = (i + 1) * (xsegments + 1) + j;

			geoData.Indices[k + 3] = (i + 1) * (xsegments + 1) + j;
			geoData.Indices[k + 4] = i * (xsegments + 1) + j + 1;
			geoData.Indices[k + 5] = (i + 1) * (xsegments + 1) + j + 1;

			k += 6; // next quad
		}
	}
}