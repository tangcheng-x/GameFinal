#include "stdafx.h"
#include "CPipeline.h"

CPipeline::CPipeline(const std::string& name, 
	IShader** shaders, 
	u32 shaderCount, 
	IInputLayout* inputLayout,
	E_PRIMITIVE_TYPE primitiveType)
	:mName(name)
	, mPrimitiveType(primitiveType)
	, mInputLayout(inputLayout)
{
	for (u32 i = 0; i < 5; i++)
		mShaders[i] = nullptr;
	

	for (u32 i = 0; i < shaderCount; i++)
	{
		if (!shaders[i])
			continue;

		E_SHADER_TYPE type = shaders[i]->getType();
		if (mShaders[type] != nullptr)
		{
			throw std::runtime_error("can not put the same type shader into one pipeline");
		}
		mShaders[type] = shaders[i];
		mShaders[type]->grab();
	}

	if (!mInputLayout)
	{
		GF_PRINT_CONSOLE_INFO("ERROR: The input-layout is null in pipeline('%s')\n", name.c_str());
		throw std::runtime_error("The input-layout is null");
	}

	mInputLayout->grab();
}

CPipeline::~CPipeline()
{
	for (u32 i = 0; i < 5; i++)
	{
		ReleaseReferenceCounted(mShaders[i]);
	}

	ReleaseReferenceCounted(mInputLayout);
}

u32 CPipeline::setMatrix(const std::string& varname, const f32* m, bool ignoreIfAlreadyUpdated)
{
	u32 result = 0;
	for (u32 i = 0; i < 5; i++)
	{
		if (mShaders[i] != nullptr && mShaders[i]->setMatrix(varname, m, ignoreIfAlreadyUpdated))
			result++;
	}
	return result;
}

bool CPipeline::setMatrix(E_SHADER_TYPE shaderType, const std::string& varname, const f32* m, bool ignoreIfAlreadyUpdated)
{
	return mShaders[shaderType] != nullptr && mShaders[shaderType]->setMatrix(varname, m, ignoreIfAlreadyUpdated);
}


u32 CPipeline::setMatrix(const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdated)
{
	u32 result = 0;
	for (u32 i = 0; i < 5; i++)
	{
		if (mShaders[i] != nullptr && mShaders[i]->setMatrix(varname, matrix, ignoreIfAlreadyUpdated))
			result++;
	}
	return result;
}

u32 CPipeline::setMatrix(E_SHADER_TYPE shaderType, const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdated)
{
	return mShaders[shaderType] != nullptr && mShaders[shaderType]->setMatrix(varname, matrix, ignoreIfAlreadyUpdated);
}

u32 CPipeline::setMatrix(const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdated)
{
	u32 result = 0;
	for (u32 i = 0; i < 5; i++)
	{
		if (mShaders[i] != nullptr && mShaders[i]->setMatrix(varname, matrix, ignoreIfAlreadyUpdated))
			result++;
	}
	return result;
}

u32 CPipeline::setMatrix(E_SHADER_TYPE shaderType, const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdated)
{
	return mShaders[shaderType] != nullptr && mShaders[shaderType]->setMatrix(varname, matrix, ignoreIfAlreadyUpdated);
}

u32 CPipeline::setTransposedMatrixArray(E_SHADER_TYPE shaderType, const std::string& varname, const f32* matrixs, u32 count, bool ignoreIfAlreadyUpdated)
{
	return mShaders[shaderType] && mShaders[shaderType]->setTransposedMatrixArray(varname, matrixs, count, ignoreIfAlreadyUpdated);
}

u32 CPipeline::setVector(const std::string& varname, const f32* v, bool ignoreIfAlreadyUpdated)
{
	u32 result = 0;
	for (u32 i = 0; i < 5; i++)
	{
		if (mShaders[i] != nullptr && mShaders[i]->setVector(varname, v, ignoreIfAlreadyUpdated))
			result++;
	}
	return result;
}

bool CPipeline::setVector(E_SHADER_TYPE shaderType, const std::string& varname, const f32* v, bool ignoreIfAlreadyUpdated)
{
	return mShaders[shaderType] != nullptr && mShaders[shaderType]->setVector(varname, v, ignoreIfAlreadyUpdated);
}

u32 CPipeline::setFloat(const std::string& varname, f32 v, bool ignoreIfAlreadyUpdated)
{
	u32 result = 0;
	for (u32 i = 0; i < 5; i++)
	{
		if (mShaders[i] != nullptr && mShaders[i]->setFloat(varname, v, ignoreIfAlreadyUpdated))
			result++;
	}
	return result;
}

bool CPipeline::setFloat(E_SHADER_TYPE shaderType, const std::string& varname, f32 value, bool ignoreIfAlreadyUpdated)
{
	return mShaders[shaderType] != nullptr && mShaders[shaderType]->setFloat(varname, value, ignoreIfAlreadyUpdated);
}

u32 CPipeline::setRawValue(const std::string& varname, void* raw, u32 size, bool ignoreIfAlreadyUpdated)
{
	u32 result = 0;
	for (u32 i = 0; i < 5; i++)
	{
		if (mShaders[i] != nullptr && mShaders[i]->setRawData(varname, raw, size, ignoreIfAlreadyUpdated))
		result++;
	}
	return result;
}

bool CPipeline::setRawValue(E_SHADER_TYPE shaderType, const std::string& varname, void* raw, u32 size, bool ignoreIfAlreadyUpdated)
{
	return mShaders[shaderType] != nullptr && mShaders[shaderType]->setRawData(varname, raw, size, ignoreIfAlreadyUpdated);
}

u32 CPipeline::setTexture(const std::string& varname, ITexture* texture)
{
	u32 result = 0;
	for (u32 i = 0; i < 5; i++)
	{
		if (mShaders[i] != nullptr && mShaders[i]->setTexture(varname, texture))
			result++;
	}
	return result;
}

bool CPipeline::setTexture(E_SHADER_TYPE shaderType, const std::string& varname, ITexture* texture)
{
	return mShaders[shaderType] != nullptr && mShaders[shaderType]->setTexture(varname, texture);
}


void CPipeline::addShaderAutoVariable(const SShaderAutoVariable& var)
{
	mShaderAutoVariables.push_back(var);
}

const std::vector<SShaderAutoVariable>& CPipeline::getShaderAutoVariables() const
{
	return mShaderAutoVariables;
}
