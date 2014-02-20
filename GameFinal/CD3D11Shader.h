#ifndef __CD3D11SHADER_H__
#define __CD3D11SHADER_H__

#include "GameFinal.h"
#include "D3DUtil.h"
#include "CD3D11Driver.h"

struct SShaderConstantBuffer
{
	std::string			Name;
	u32					Size;
	bool				Dirty;
	
	void*				pData;


	SShaderConstantBuffer(const char* Name)
		:Name(Name)
	{
		Dirty = false;
		pData = NULL;
	}

	~SShaderConstantBuffer()
	{
		free(pData);
		pData = nullptr;
	}
};

struct SShaderConstantVariable
{
	u32								ConstantBufferIndex;
	bool							AlreadyUpdated; /* this flag is just for those per-frame-updated variables. */
	D3D11_SHADER_VARIABLE_DESC		Desc;
	D3D11_SHADER_TYPE_DESC			TypeDesc;
};

class CD3D11Shader : public IShader
{
public:
	CD3D11Shader(u32 id, 
		std::string name, 
		ID3D11Device* pd3dDevice, 
		ID3D11DeviceContext* pd3dDeviceContext,
		CD3D11Driver* pd3dDriver)
		:IShader(id, name), 
		md3dDevice(pd3dDevice), 
		md3dDeviceContext(pd3dDeviceContext),
		md3dDriver(pd3dDriver)
	{
		
	}

	virtual const char* getShaderProfile() const = 0;

	virtual bool loadFromFile(const char* szFileName, const char* szFunctionName) = 0;

	ID3D10Blob* getShaderBuffer()
	{
		return mShaderBuffer;
	}

	bool compile(const char* szFileName, const char* szFunctionName, E_SHADER_TYPE type);

	bool initContext();

	virtual ~CD3D11Shader()
	{
		ReleaseCOM(mShaderBuffer);
	}


	SShaderConstantVariable* getConstantVariable(const std::string& varname);

	virtual bool setRawData(const std::string& varname, void* data, u32 size, bool ignoreIfAlreadyUpdate = false);

	virtual bool setFloat(const std::string& varname, f32 val, bool ignoreIfAlreadyUpdate = false);

	virtual bool setVector(const std::string& varname, const f32* val, bool ignoreIfAlreadyUpdate = false);

	//virtual bool setVector3(const std::string& varname, const f32* val, bool ignoreIfAlreadyUpdate = false) = 0;

	virtual bool setMatrix(const std::string& varname, const f32* matrix, bool ignoreIfAlreadyUpdate = false);

	virtual bool setMatrix(const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdate = false);

	virtual bool setMatrix(const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdate = false);

	virtual bool setTexture(const std::string& varname, ITexture* texture);

	virtual bool setTransposedMatrixArray(const std::string& var, const f32* matrixs, u32 count, bool ignoreIfAlreadyUpdate = false);

	bool applyConstantBuffer(ID3D11DeviceContext* pd3dDeviceContext, u32 index);

	virtual void update(bool forceUpdate = false);

	virtual void submit() = 0;

	virtual u32 getTextureCount() const
	{
		return mSrvNum;
	}

	virtual void reset();

	virtual bool isAlreadyUpdated(const std::string& varname);

protected:
	bool setRawData(SShaderConstantVariable* cv, void* data, u32 size);

	ID3D11Device*						md3dDevice;
	ID3D11DeviceContext*				md3dDeviceContext;
	ID3D10Blob*							mShaderBuffer;

	CD3D11Driver*						md3dDriver;

	std::string							mName;
	ID3D11Buffer*						md3dConstantBuffers[16];
	u32									mSrvNum;

	std::vector<std::unique_ptr<SShaderConstantBuffer>>		mConstantBuffers;
	std::map<std::string, SShaderConstantVariable>			mConstantVariables;

	std::map<std::string, D3D11_SHADER_INPUT_BIND_DESC>		mShaderResourceDescs;

};

#endif