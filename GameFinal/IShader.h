#ifndef __ISHADER_H__
#define __ISHADER_H__

#include "gfTypes.h"
#include "gfUtil.h"
#include "IShaderContext.h"
#include <string>
#include <iostream>

class IShaderContext;

enum E_SHADER_TYPE
{
	EST_UNDEFINED_SHADER = -1,
	EST_VERTEX_SHADER = 0,
	EST_PIXEL_SHADER,
	EST_GEOMETRY_SHADER,
	EST_HULL_SHADER,
	EST_DOMAIN_SHADER,
	EST_SHADER_COUNT
};

class IShader : public IReferenceCounted
{
public:
	IShader(u32 sortCode, std::string name)
		: mSortCode(sortCode)
		, mName(name)
	{

	}

	const std::string& getName() const
	{
		return mName;
	}

	virtual E_SHADER_TYPE getType() const = 0;
	
	virtual void submit() = 0;

	virtual bool setRawData(const std::string& varname, void* data, u32 size, bool ignoreIfAlreadyUpdate = false) = 0;

	virtual bool setFloat(const std::string& varname, f32 val, bool ignoreIfAlreadyUpdate = false) = 0;

	virtual bool setVector(const std::string& varname, const f32* val, bool ignoreIfAlreadyUpdate = false) = 0;

//	virtual bool setVector3(const std::string& varname, const f32* val, bool ignoreIfAlreadyUpdate = false) = 0;

	virtual bool setMatrix(const std::string& varname, const f32* matrix, bool ignoreIfAlreadyUpdate = false) = 0;

	virtual bool setMatrix(const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdate = false) = 0;

	virtual bool setMatrix(const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdate = false) = 0;

	virtual bool setTexture(const std::string& varname, ITexture* texture) = 0;

	virtual bool setTransposedMatrixArray(const std::string& var, const f32* matrixs, u32 count, bool ignoreIfAlreadyUpdate = false) = 0;

	virtual void update(bool forceUpdate = false) = 0;

	virtual bool isAlreadyUpdated(const std::string& varname) = 0;

	u32 getSortCode() const
	{
		return mSortCode;
	}

	virtual u32 getTextureCount() const = 0;

	virtual void reset() = 0;

	virtual ~IShader()
	{
		
	}

protected:
	u32						mSortCode;
	std::string				mName;
};

#endif


