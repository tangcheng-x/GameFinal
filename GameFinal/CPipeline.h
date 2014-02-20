#ifndef __CPIPELINE_H__
#define __CPIPELINE_H__

#include "GameFinal.h"

class CPipeline : public IPipeline
{
public:
	CPipeline(const std::string& name, IShader** shaders, u32 shaderCount, IInputLayout* inputLayout, E_PRIMITIVE_TYPE primitiveType);

	virtual u32 setMatrix(const std::string& varname, const f32* m, bool ignoreIfAlreadyUpdated = false);

	virtual bool setMatrix(E_SHADER_TYPE shaderType, const std::string& varname, const f32* m, bool ignoreIfAlreadyUpdated = false);


	virtual u32 setMatrix(const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdated = false);

	virtual u32 setMatrix(E_SHADER_TYPE shaderType, const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdated = false);

	virtual u32 setMatrix(const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdated = false);

	virtual u32 setMatrix(E_SHADER_TYPE shaderType, const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdated = false);

	virtual u32 setTransposedMatrixArray(E_SHADER_TYPE shaderType, const std::string& varname, const f32* matrixs, u32 count, bool ignoreIfAlreadyUpdated = false);

	virtual u32 setVector(const std::string& varname, const f32* v, bool ignoreIfAlreadyUpdated = false);

	virtual bool setVector(E_SHADER_TYPE shaderType, const std::string& varname, const f32* v, bool ignoreIfAlreadyUpdated = false);

	virtual u32 setFloat(const std::string& varname, f32 v, bool ignoreIfAlreadyUpdated = false);

	virtual bool setFloat(E_SHADER_TYPE shaderType, const std::string& varname, f32 v, bool ignoreIfAlreadyUpdated = false);

	virtual u32 setRawValue(const std::string& varname, void* raw, u32 size, bool ignoreIfAlreadyUpdated = false);

	virtual bool setRawValue(E_SHADER_TYPE shaderType, const std::string& varname, void* raw, u32 size, bool ignoreIfAlreadyUpdated = false);

	virtual u32 setTexture(const std::string& varname, ITexture* texture);

	virtual bool setTexture(E_SHADER_TYPE shaderType, const std::string& varname, ITexture* texture);

	//virtual bool setInt(u32 v) = 0;

	//virtual void updateInputLayout();

	virtual IShader* getShader(E_SHADER_TYPE type)
	{
		return mShaders[type];
	}

	virtual IInputLayout* getInputLayout()
	{
		return mInputLayout;
	}

	virtual E_PRIMITIVE_TYPE getPrimitiveType() const
	{
		return mPrimitiveType;
	}

	virtual void addShaderAutoVariable(const SShaderAutoVariable& var);

	virtual const std::vector<SShaderAutoVariable>& getShaderAutoVariables() const;

	virtual ~CPipeline();

	virtual u64 getSortCode() const = 0;

	virtual const std::string& getName() const
	{
		return mName;
	}

protected:
	std::string			mName;
	IShader*			mShaders[5];
	IInputLayout*		mInputLayout;

	E_PRIMITIVE_TYPE	mPrimitiveType;

	std::vector<SShaderAutoVariable> mShaderAutoVariables;

private:
	
};

#endif
