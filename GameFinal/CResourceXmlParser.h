#ifndef __RESOURCE_XML_PARSER_CLASS_H__
#define __RESOURCE_XML_PARSER_CLASS_H__

#include "IResourceXmlParser.h"
#include "gfGIFormat.h"
#include "tinyxml/tinyxml2.h"

class CResourceXmlParser : public IResourceXmlParser
{
public:
	struct SShaderVariableAttribute
	{
		E_SHADER_AUTO_VARIABLE_TYPE		Meaning;
		E_UPDATE_FREQUENCY				DefaultUpdateFrequency;

		SShaderVariableAttribute(){}

		SShaderVariableAttribute(E_SHADER_AUTO_VARIABLE_TYPE meaning, E_UPDATE_FREQUENCY freq = EUF_PER_OBJECT)
			:Meaning(meaning), DefaultUpdateFrequency(freq)
		{

		}
	};

public:
	CResourceXmlParser();
	
	virtual bool parsePipelineFile(const std::string& filepath, std::vector<SPipelineCreateParams>& createParamsArray) const;

	virtual bool parseMaterialFile(const std::string& filepath, std::vector<SMaterialCreateParams>& createParamsArray) const;

private:

	virtual bool handlePipelineNode(const std::string& filepath, tinyxml2::XMLElement* node, SPipelineCreateParams& createParams) const;

	E_PRIMITIVE_TYPE getPrimitiveType(std::string& s) const;

	E_GI_FORMAT		getInputLayoutFormat(std::string& s) const;

	E_RENDER_STATE	getRenderStateType(std::string& s) const;

	bool			getRenderStateValue(std::string& s, const std::map<std::string, u32> mapping, u32& value) const;

	bool			getBoolValue(const char* s, bool& value) const;

	bool			getMaskValue(const char* s, u32& value) const;

	/* this function just tell the char is a valid charactor to form a float. */
	bool			isFloatCharactor(char c) const { return (c >= '0' && c <= '9') || c == '.'; }

	bool handleInputLayoutElementNode(const std::string& filepath,
		tinyxml2::XMLElement* node, 
		SInputLayoutElement& element) const;

	bool handleShaderNode(const std::string& filepath, 
		E_SHADER_TYPE shaderType, 
		tinyxml2::XMLElement* node, 
		SPipelineCreateParams& createParams) const;

	bool handleShaderVariableNode(const std::string& filepath, 
		E_SHADER_TYPE shaderType,
		tinyxml2::XMLElement* node,
		SPipelineCreateParams& createParams) const;

	bool handleRenderStateNode(const std::string& filepath,
		tinyxml2::XMLElement* node,
		SPipelineCreateParams& createParams) const;

	bool handleMaterialNode(const std::string& filepath,
		tinyxml2::XMLElement* node,
		SMaterialCreateParams& createParams) const;

	/* handle <ambient> <diffuse> <specular> <emissive> element in material file */
	bool handleMaterialColorNode(const std::string& filepath,
		const std::string& materialName,
		tinyxml2::XMLElement* node, XMFLOAT4& color) const;

	/* handle <texture> element in material file. */
	bool handleMaterialTextureNode(const std::string& filepath,
		const std::string& materialName,
		tinyxml2::XMLElement* node, SMaterialTextureParam& param) const;
	

	/* 顶点格式的文本形式与枚举类型的映射 */
	std::map<std::string, E_GI_FORMAT>				mVertexFormatMapping;

	/* 图元类型的文本形式与枚举类型的映射 */
	std::map<std::string, E_PRIMITIVE_TYPE>			mPrimitiveTypeMapping;

	/* 自动注入Shader变量的映射 */
	std::map<std::string, SShaderVariableAttribute>	mShaderVariableMapping;
	
	/* 绘制状态文本形式与枚举类型的映射 */
	std::map<std::string, E_RENDER_STATE>			mRenderStateMapping;

	/* E_FILL_MODE 类型的文本形式与枚举类型的映射 */
	std::map<std::string, u32>						mFillModeMapping;

	/* E_CULL_MODE 类型的文本形式与枚举类型的映射 */
	std::map<std::string, u32>						mCullModeMapping;

	/* E_BLEND_TYPE 类型的文本形式与枚举类型的映射 */
	std::map<std::string, u32>						mBlendTypeMapping;

	/* E_BLEND_OP 类型的文本形式与枚举类型的映射 */
	std::map<std::string, u32>						mBlendOpMapping;

	/* E_COMPARISON_FUNC 类型的文本形式与枚举类型的映射 */
	std::map<std::string, u32>						mComparisonFuncMapping;

	/* E_STENCIL_OP 类型的文本形式与枚举类型的映射 */
	std::map<std::string, u32>						mStencilOpMapping;
};

#endif
