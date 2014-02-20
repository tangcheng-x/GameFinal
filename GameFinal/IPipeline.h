#ifndef __IPIPELINE_H__
#define __IPIPELINE_H__

#include "gfTypes.h"
#include "gfUtil.h"
#include "IShader.h"
#include "IInputLayout.h"
#include "IRenderState.h"

enum E_PRIMITIVE_TYPE
{
	EPT_UNDEFINED = 0,
	EPT_POINTLIST = 1,
	EPT_LINELIST = 2,
	EPT_LINESTRIP = 3,
	EPT_TRIANGLELIST = 4,
	EPT_TRIANGLESTRIP = 5
};

//param_indexed_auto
enum E_SHADER_AUTO_VARIABLE_TYPE
{
	/* transforms */
	ESAVT_WORLD_MATRIX,
	ESAVT_INVERSE_WORLD_MATRIX,
	ESAVT_TRANSPOSE_WORLD_MATRIX,
	ESAVT_INVERSE_TRANSPOSE_WORLD_MATRIX,
	ESAVT_VIEW_MATRIX,
	ESAVT_INVERSE_VIEW_MATRIX,
	ESAVT_TRANSPOSE_VIEW_MATRIX,
	ESAVT_INVERSE_TRANSPOSE_VIEW_MATRIX,
	ESAVT_PROJ_MATRIX,
	ESAVT_INVERSE_PROJ_MATRIX,
	ESAVT_TRANSPOSE_PROJ_MATRIX,
	ESAVT_INVERSE_TRANSPOSE_PROJ_MATRIX,
	ESAVT_WORLD_VIEW_MATRIX,
	ESAVT_INVERSE_WORLD_VIEW_MATRIX,
	ESAVT_TRANSPOSE_WORLD_VIEW_MATRIX,
	ESAVT_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX,
	ESAVT_VIEW_PROJ_MATRIX,
	ESAVT_INVERSE_VIEW_PROJ_MATRIX,
	ESAVT_TRANSPOSE_VIEW_PROJ_MATRIX,
	ESAVT_INVERSE_TRANSPOSE_VIEW_PROJ_MATRIX,
	ESAVT_WORLD_VIEW_PROJ_MATRIX,
	ESAVT_INVERSE_WORLD_VIEW_PROJ_MATRIX,
	ESAVT_TRANSPOSE_WORLD_VIEW_PROJ_MATRIX,
	ESAVT_INVERSE_TRANSPOSE_WORLD_VIEW_PROJ_MATRIX,

	/* materials */
	ESAVT_MATERIAL,
	ESAVT_MATERIAL_AMBIENT,
	ESAVT_MATERIAL_DIFFUSE,
	ESAVT_MATERIAL_SPECULAR,
	ESAVT_MATERIAL_EMISSIVE,
	ESAVT_TEXTURE,

	/* scene info */
	ESAVT_CAMERA_POSITION,
	ESAVT_LIGHT,

	/* bone transforms. */
	ESAVT_BONE_TRANSFORMS
};

enum E_UPDATE_FREQUENCY
{
	EUF_PER_OBJECT,
	EUF_PER_FRAME
};

#define ESAVT_TRANSFORMS_BEGIN ESAVT_WORLD_MATRIX
#define ESAVT_TRANSFORMS_END   ESAVT_INVERSE_TRANSPOSE_WORLD_VIEW_PROJ_MATRIX

#define ESAVT_MATERIAL_BEGIN ESAVT_MATERIAL
#define ESAVT_MATERIAL_END   ESAVT_TEXTURE

#define ESAVT_SCENE_BEGIN ESAVT_CAMERA_POSITION
#define ESAVT_SCENE_END   ESAVT_LIGHT

struct SShaderAutoVariable
{
	std::string						VariableName;
	E_SHADER_AUTO_VARIABLE_TYPE		Type;
	E_SHADER_TYPE					ShaderType;
	E_UPDATE_FREQUENCY				UpdateFrequency;
	u32								IndexParam;
};


class IPipeline : public IReferenceCounted
{
public:
	
	virtual u32 setMatrix(const std::string& varname, const f32* m, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual bool setMatrix(E_SHADER_TYPE shaderType, const std::string& varname, const f32* m, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual u32 setMatrix(const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual u32 setMatrix(E_SHADER_TYPE shaderType, const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual u32 setMatrix(const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual u32 setMatrix(E_SHADER_TYPE shaderType, const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual u32 setTransposedMatrixArray(E_SHADER_TYPE shaderType, const std::string& varname, const f32* matrixs, u32 count, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual u32 setVector(const std::string& varname, const f32* v, bool ignoreIfAlreadyUpdated = false) = 0;
	
	virtual bool setVector(E_SHADER_TYPE shaderType, const std::string& varname, const f32* v, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual u32 setFloat(const std::string& varname, f32 v, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual bool setFloat(E_SHADER_TYPE shaderType, const std::string& varname, f32 v, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual u32 setRawValue(const std::string& varname, void* raw, u32 size, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual bool setRawValue(E_SHADER_TYPE shaderType, const std::string& varname, void* raw, u32 size, bool ignoreIfAlreadyUpdated = false) = 0;

	virtual u32 setTexture(const std::string& varname, ITexture* texture) = 0;

	virtual bool setTexture(E_SHADER_TYPE shaderType, const std::string& varname, ITexture* texture) = 0;

	virtual IShader* getShader(E_SHADER_TYPE type) = 0;

	virtual IInputLayout* getInputLayout() = 0;

	virtual IRenderState* getRenderState() = 0;

	virtual E_PRIMITIVE_TYPE getPrimitiveType() const = 0;

	virtual void addShaderAutoVariable(const SShaderAutoVariable& var) = 0;

	virtual const std::vector<SShaderAutoVariable>& getShaderAutoVariables() const = 0;

	virtual void apply() = 0;

	virtual u64 getSortCode() const = 0;

	virtual const std::string& getName() const = 0;
};

#endif

