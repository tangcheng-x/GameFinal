#ifndef __IGEOMETRY_CREATOR_H_INCLUDE
#define __IGEOMETRY_CREATOR_H_INCLUDE

#include "gfTypes.h"
#include "gfUtil.h"
#include "IMesh.h"
#include "IReferenceCounted.h"


struct SGeometryVertex
{
	SGeometryVertex(){}
	SGeometryVertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
		: Position(p), Normal(n), TangentU(t), TexC(uv){}
	SGeometryVertex(
		f32 px, f32 py, f32 pz,
		f32 nx, f32 ny, f32 nz,
		f32 tx, f32 ty, f32 tz,
		f32 u,  f32 v)
		: Position(px, py, pz), Normal(nx, ny, nz),
		TangentU(tx, ty, tz), TexC(u, v){}

	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT3 TangentU;
	XMFLOAT2 TexC;
};

struct SGeometryData
{
	std::vector<SGeometryVertex>	Vertices;
	std::vector<u16>				Indices;
};

class IGeometryCreator : public IReferenceCounted
{
public:

	u32 getVertexStride() const
	{
		return sizeof(SGeometryVertex);
	}

	virtual void createCubeData(
		f32 width, 
		f32 height,
		f32 depth,
		SGeometryData& geoData) = 0;

	virtual void createPlaneData(
		f32 width, 
		f32 depth, 
		u32 xsegments,
		u32 ysegments, 
		f32 uTiles, 
		f32 vTiles, 
		SGeometryData& geoData) = 0;

private:

};


#endif
