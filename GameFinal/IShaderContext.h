#ifndef __ISHADERCONTEXT_H__
#define __ISHADERCONTEXT_H__

#include "gfTypes.h"
#include "gfUtil.h"
#include "ITexture.h"
#include "IReferenceCounted.h"

/*
enum E_SHADER_TYPE;

class IShaderContext : public IReferenceCounted
{
public:
	IShaderContext(const std::string& name, E_SHADER_TYPE shaderType)
		:mName(name), m_shaderType(shaderType)
	{

	}

	const std::string& getName() const
	{
		return mName;
	}

	E_SHADER_TYPE getShaderType() const
	{
		return m_shaderType;
	}

	virtual bool setRawData(const std::string& varname, void* data, u32 size) = 0;

	virtual bool setFloat(const std::string& varname, f32 val) = 0;

	virtual bool setVector(const std::string& varname, const f32* val) = 0;

	virtual bool setMatrix(const std::string& varname, const f32* matrix) = 0;

	virtual bool setMatrix(const std::string& varname, CXMMATRIX matrix) = 0;

	virtual bool setMatrix(const std::string& varname, const XMFLOAT4X4& matrix) = 0;

	virtual bool setTexture(const std::string& varname, ITexture* texture) = 0;

	virtual void update(bool forceUpdate=false) = 0;

	virtual void submit(E_SHADER_TYPE shaderType) = 0;
	
protected:
	std::string mName;
	const E_SHADER_TYPE m_shaderType;
};
*/


#endif