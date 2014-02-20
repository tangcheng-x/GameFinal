#include "stdafx.h"
#include "CResourceXmlParser.h"

CResourceXmlParser::CResourceXmlParser()
{
	// init input-layout format mapping
	mVertexFormatMapping["FLOAT4"] = EGF_R32G32B32A32_FLOAT;
	mVertexFormatMapping["R32G32B32A32"] = EGF_R32G32B32A32_FLOAT;
	mVertexFormatMapping["FLOAT3"] = EGF_R32G32B32_FLOAT;
	mVertexFormatMapping["R32G32B32"] = EGF_R32G32B32_FLOAT;
	mVertexFormatMapping["FLOAT2"] = EGF_R32G32_FLOAT;
	mVertexFormatMapping["R32G32"] = EGF_R32G32_FLOAT;
	mVertexFormatMapping["R8G8B8A8_UINT"] = EGF_R8G8B8A8_UINT;
	mVertexFormatMapping["BYTE4"] = EGF_R8G8B8A8_UINT;

	mPrimitiveTypeMapping["POINTLIST"] = EPT_POINTLIST;
	mPrimitiveTypeMapping["POINT_LIST"] = EPT_POINTLIST;
	mPrimitiveTypeMapping["POINT"] = EPT_POINTLIST;
	mPrimitiveTypeMapping["LINELIST"] = EPT_LINELIST;
	mPrimitiveTypeMapping["LINE_LIST"] = EPT_LINELIST;
	mPrimitiveTypeMapping["LINE_STRIP"] = EPT_LINESTRIP;
	mPrimitiveTypeMapping["LINESTRIP"] = EPT_LINESTRIP;
	mPrimitiveTypeMapping["TRIANGLELIST"] = EPT_TRIANGLELIST;
	mPrimitiveTypeMapping["TRIANGLE_LIST"] = EPT_TRIANGLELIST;
	mPrimitiveTypeMapping["TRIANGLE"] = EPT_TRIANGLELIST;
	mPrimitiveTypeMapping["TRIANGLESTRIP"] = EPT_TRIANGLESTRIP;
	mPrimitiveTypeMapping["TRIANGLE_STRIP"] = EPT_TRIANGLESTRIP;

	// init shader variables mapping
	mShaderVariableMapping["world_matrix"] = SShaderVariableAttribute(ESAVT_WORLD_MATRIX);
	mShaderVariableMapping["inverse_world_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_WORLD_MATRIX);
	mShaderVariableMapping["transpose_world_matrix"] = SShaderVariableAttribute(ESAVT_TRANSPOSE_WORLD_MATRIX);
	mShaderVariableMapping["inverse_transpose_world_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_TRANSPOSE_WORLD_MATRIX);
	mShaderVariableMapping["view_matrix"] = SShaderVariableAttribute(ESAVT_VIEW_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["inverse_view_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_VIEW_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["transpose_view_matrix"] = SShaderVariableAttribute(ESAVT_TRANSPOSE_VIEW_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["inverse_transpose_view_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_TRANSPOSE_VIEW_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["projection_matrix"] = SShaderVariableAttribute(ESAVT_PROJ_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["inverse_projection_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_PROJ_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["transpose_projection_matrix"] = SShaderVariableAttribute(ESAVT_TRANSPOSE_PROJ_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["inverse_transpose_projection_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_TRANSPOSE_PROJ_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["worldview_matrix"] = SShaderVariableAttribute(ESAVT_WORLD_VIEW_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["inverse_worldview_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_WORLD_VIEW_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["transpose_worldview_matrix"] = SShaderVariableAttribute(ESAVT_TRANSPOSE_WORLD_VIEW_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["inverse_transpose_worldview_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["viewproj_matrix"] = SShaderVariableAttribute(ESAVT_VIEW_PROJ_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["inverse_viewproj_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_VIEW_PROJ_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["transpose_viewproj_matrix"] = SShaderVariableAttribute(ESAVT_TRANSPOSE_VIEW_PROJ_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["inverse_transpose_viewproj_matrix"] = SShaderVariableAttribute(ESAVT_INVERSE_TRANSPOSE_VIEW_PROJ_MATRIX, EUF_PER_FRAME);
	mShaderVariableMapping["camera_position"] = SShaderVariableAttribute(ESAVT_CAMERA_POSITION, EUF_PER_FRAME);
	mShaderVariableMapping["bone_transforms"] = SShaderVariableAttribute(ESAVT_BONE_TRANSFORMS, EUF_PER_OBJECT);

	mShaderVariableMapping["light"] = SShaderVariableAttribute(ESAVT_LIGHT, EUF_PER_FRAME);
	mShaderVariableMapping["material"] = SShaderVariableAttribute(ESAVT_MATERIAL, EUF_PER_OBJECT);
	mShaderVariableMapping["material_ambient"] = SShaderVariableAttribute(ESAVT_MATERIAL_AMBIENT, EUF_PER_OBJECT);
	mShaderVariableMapping["material_diffuse"] = SShaderVariableAttribute(ESAVT_MATERIAL_DIFFUSE, EUF_PER_OBJECT);
	mShaderVariableMapping["material_specular"] = SShaderVariableAttribute(ESAVT_MATERIAL_SPECULAR, EUF_PER_OBJECT);
	mShaderVariableMapping["material_emissive"] = SShaderVariableAttribute(ESAVT_MATERIAL_EMISSIVE, EUF_PER_OBJECT);
	mShaderVariableMapping["texture"] = SShaderVariableAttribute(ESAVT_TEXTURE);

	// init render state mapping.
	mRenderStateMapping["FILL_MODE"] = ERS_FILL_MODE;
	mRenderStateMapping["CULL_MODE"] = ERS_CULL_MODE;
	mRenderStateMapping["DEPTH_BIAS"] = ERS_DEPTH_BIAS;
	mRenderStateMapping["DEPTH_BIAS_CLAMP"] = ERS_DEPTH_BIAS_CLAMP;
	mRenderStateMapping["SCISSOR_ENABLE"] = ERS_SCISSOR_ENABLE;
	mRenderStateMapping["MULTISAMPLE_ENABLE"] = ERS_MULTISAMPLE_ENABLE;
	mRenderStateMapping["ANTI_LINE_ENABLE"] = ERS_ANTI_LINE_ENABLE;
	mRenderStateMapping["BLEND_ENABLE"] = ERS_BLEND_ENABLE;
	mRenderStateMapping["SRC_BLEND"] = ERS_SRC_BLEND;
	mRenderStateMapping["DEST_BLEND"] = ERS_DEST_BLEND;
	mRenderStateMapping["BLEND_OP"] = ERS_BLEND_OP;
	mRenderStateMapping["SRC_BLEND_ALPHA"] = ERS_SRC_BLEND_ALPHA;
	mRenderStateMapping["DEST_BLEND_ALPHA"] = ERS_DEST_BLEND_ALPHA;
	mRenderStateMapping["BLEND_OP_ALPHA"] = ERS_BLEND_OP_ALPHA;
	mRenderStateMapping["BLEND_WRITE_MASK"] = ERS_BLEND_WRITE_MASK;
	mRenderStateMapping["ALPHA_COVERAGE_ENABLE"] = ERS_ALPHA_COVERAGE_ENABLE;
	mRenderStateMapping["INDEPENDENT_BLEND_ENABLE"] = ERS_INDEPENDENT_BLEND_ENABLE;
	mRenderStateMapping["DEPTH_ENABLE"] = ERS_DEPTH_ENABLE;
	mRenderStateMapping["DEPTH_WRITE_ENABLE"] = ERS_DEPTH_WRITE_ENABLE;
	mRenderStateMapping["DEPTH_FUNC"] = ERS_DEPTH_FUNC;
	mRenderStateMapping["STENCIL_ENABLE"] = ERS_STENCIL_ENABLE;
	mRenderStateMapping["STENCIL_READ_MASK"] = ERS_STENCIL_READ_MASK;
	mRenderStateMapping["STENCIL_WRITE_MASK"] = ERS_STENCIL_WRITE_MASK;
	mRenderStateMapping["STENCIL_FAIL_OP"] = ERS_STENCIL_FAIL_OP;
	mRenderStateMapping["STENCIL_DEPTH_FAIL_OP"] = ERS_STENCIL_DEPTH_FAIL_OP;
	mRenderStateMapping["STENCIL_PASS_OP"] = ERS_STENCIL_PASS_OP;
	mRenderStateMapping["STENCIL_FUNC"] = ERS_STENCIL_FUNC;
	mRenderStateMapping["BACKFACE_STENCIL_FAIL_OP"] = ERS_BACKFACE_STENCIL_FAIL_OP;
	mRenderStateMapping["BACKFACE_STENCIL_DEPTH_FAIL_OP"] = ERS_BACKFACE_STENCIL_DEPTH_FAIL_OP;
	mRenderStateMapping["BACKFACE_STENCIL_PASS_OP"] = ERS_BACKFACE_STENCIL_PASS_OP;
	mRenderStateMapping["BACKFACE_STENCIL_FUNC"] = ERS_BACKFACE_STENCIL_FUNC;

	/* init fill modes mapping. */
	mFillModeMapping["WIREFRAME"] = E_FILL_WIREFRAME;
	mFillModeMapping["SOLID"] = E_FILL_SOLID;

	mCullModeMapping["NONE"] = E_CULL_NONE;
	mCullModeMapping["CW"] = E_CULL_CW;
	mCullModeMapping["CCW"] = E_CULL_CCW;

	mBlendTypeMapping["ZERO"] = E_BLEND_ZERO;
	mBlendTypeMapping["ONE"] = E_BLEND_ONE;
	mBlendTypeMapping["SRC_COLOR"] = E_BLEND_SRC_COLOR;
	mBlendTypeMapping["INV_SRC_COLOR"] = E_BLEND_INV_SRC_COLOR;
	mBlendTypeMapping["SRC_ALPHA"] = E_BLEND_SRC_ALPHA;
	mBlendTypeMapping["INV_SRC_ALPHA"] = E_BLEND_INV_SRC_ALPHA;
	mBlendTypeMapping["DEST_ALPHA"] = E_BLEND_DEST_ALPHA;
	mBlendTypeMapping["INV_DEST_ALPHA"] = E_BLEND_INV_DEST_ALPHA;
	mBlendTypeMapping["DEST_COLOR"] = E_BLEND_DEST_COLOR;
	mBlendTypeMapping["INV_DEST_COLOR"] = E_BLEND_INV_DEST_COLOR;
	mBlendTypeMapping["SRC_ALPHA_SAT"] = E_BLEND_SRC_ALPHA_SAT;
	mBlendTypeMapping["BLEND_FACTOR"] = E_BLEND_BLEND_FACTOR;
	mBlendTypeMapping["INV_BLEND_FACTOR"] = E_BLEND_ONE;
	mBlendTypeMapping["SRC1_COLOR"] = E_BLEND_SRC1_COLOR;
	mBlendTypeMapping["INV_SRC1_COLOR"] = E_BLEND_INV_SRC1_COLOR;
	mBlendTypeMapping["SRC1_ALPHA"] = E_BLEND_SRC1_ALPHA;
	mBlendTypeMapping["INV_SRC1_ALPHA"] = E_BLEND_INV_SRC1_ALPHA;

	mBlendOpMapping["ADD"] = EBO_ADD;
	mBlendOpMapping["SUBTRACT"] = EBO_SUBTRACT;
	mBlendOpMapping["REV_SUBTRACT"] = EBO_REV_SUBTRACT;
	mBlendOpMapping["MIN"] = EBO_MIN;
	mBlendOpMapping["MAX"] = EBO_MAX;

	mComparisonFuncMapping["NEVER"] = ECF_NEVER;
	mComparisonFuncMapping["LESS"] = ECF_LESS;
	mComparisonFuncMapping["EQUAL"] = ECF_EQUAL;
	mComparisonFuncMapping["LESS_EQUAL"] = ECF_LESS_EQUAL;
	mComparisonFuncMapping["GREATER"] = ECF_GREATER;
	mComparisonFuncMapping["NOT_EQUAL"] = ECF_NOT_EQUAL;
	mComparisonFuncMapping["GREATER_EQUAL"] = ECF_GREATER_EQUAL;
	mComparisonFuncMapping["ALWAYS"] = ECF_ALWAYS;

	mStencilOpMapping["KEEP"] = ESO_KEEP;
	mStencilOpMapping["ZERO"] = ESO_ZERO;
	mStencilOpMapping["REPLACE"] = ESO_REPLACE;
	mStencilOpMapping["INCR_SAT"] = ESO_INCR_SAT;
	mStencilOpMapping["DECR_SAT"] = ESO_DECR_SAT;
	mStencilOpMapping["INVERT"] = ESO_INVERT;
	mStencilOpMapping["INCR"] = ESO_INCR;
	mStencilOpMapping["DECR"] = ESO_DECR;
}

bool CResourceXmlParser::handlePipelineNode(const std::string& filepath, tinyxml2::XMLElement* pipeline_node, SPipelineCreateParams& createParams) const
{
	/* get the name of pipeline. */
	const char* pipeline_name = pipeline_node->Attribute("name");
	if (pipeline_name)
		createParams.Name = pipeline_name;

	/* get the primitive type of pipeline. */
	tinyxml2::XMLElement* primitive_type_node = pipeline_node->FirstChildElement("primitive-type");
	createParams.PrimitiveType = EPT_TRIANGLELIST;
	if (primitive_type_node)
	{
		const char* primitive_type_text = primitive_type_node->Attribute("value");
		if (primitive_type_text)
		{
			std::string primitive_type_string = primitive_type_text;
			E_PRIMITIVE_TYPE primitiveType = getPrimitiveType(primitive_type_string);
			if (primitiveType != EPT_UNDEFINED)
				createParams.PrimitiveType = primitiveType;
		}
	}

	/* get the input-layout */
	tinyxml2::XMLElement* input_layout_node = pipeline_node->FirstChildElement("input-layout");
	if (!input_layout_node)
	{
		GF_PRINT_CONSOLE_INFO("The <pipeline> doesn't have <input-layout> element in pipeline file (%s).\n", filepath.c_str());
		return false;
	}

	tinyxml2::XMLElement* element_node = input_layout_node->FirstChildElement("element");

	u32 slotOffsets[16];
	memset(slotOffsets, 0, sizeof(slotOffsets));

	while (element_node)
	{
		SInputLayoutElement element;
		if (!handleInputLayoutElementNode(filepath, element_node, element))
			return false;

		element.Offset = slotOffsets[element.Slot];
		slotOffsets[element.Slot] += getFormatOffset(element.Format);

		createParams.InputLayoutElements.push_back(element);
		element_node = element_node->NextSiblingElement("element");
	}

	// get the shaders root node
	tinyxml2::XMLElement* shaders_node = pipeline_node->FirstChildElement("shaders");
	if (!shaders_node)
	{
		GF_PRINT_CONSOLE_INFO("The <pipeline> doesn't have <shaders> element in pipeline file (%s).\n", filepath.c_str());
		return false;
	}

	// get vertex shader, which is necessary.
	tinyxml2::XMLElement* vertex_shader_node = shaders_node->FirstChildElement("vertex-shader");
	if (!vertex_shader_node)
	{
		GF_PRINT_CONSOLE_INFO("The <pipeline> doesn't have <vertex-shader> in pipeline file (%s).\n", filepath.c_str());
		return false;
	}
	handleShaderNode(filepath, EST_VERTEX_SHADER, vertex_shader_node, createParams);

	// get the pixel shader, which is also necessary.
	tinyxml2::XMLElement* pixel_shader_node = shaders_node->FirstChildElement("pixel-shader");
	if (!pixel_shader_node)
	{
		GF_PRINT_CONSOLE_INFO("The <pipeline> doesn't have <pixel-shader> in pipeline file (%s).\n", filepath.c_str());
		return false;
	}
	handleShaderNode(filepath, EST_PIXEL_SHADER, pixel_shader_node, createParams);

	//get hull shader
	tinyxml2::XMLElement* hull_shader_node = shaders_node->FirstChildElement("hull-shader");
	if (hull_shader_node)
	{
		handleShaderNode(filepath, EST_HULL_SHADER, hull_shader_node, createParams);
	}

	//get domain shader
	tinyxml2::XMLElement* domain_shader_node = shaders_node->FirstChildElement("domain-shader");
	if (domain_shader_node)
	{
		handleShaderNode(filepath, EST_DOMAIN_SHADER, domain_shader_node, createParams);
	}

	//get geometry shader
	tinyxml2::XMLElement* geometry_shader_node = shaders_node->FirstChildElement("geometry-shader");
	if (domain_shader_node)
	{
		handleShaderNode(filepath, EST_GEOMETRY_SHADER, geometry_shader_node, createParams);
	}

	/* get render states */
	tinyxml2::XMLElement* render_states_node = pipeline_node->FirstChildElement("render-states");
	if (render_states_node)
	{
		tinyxml2::XMLElement* render_state_node = render_states_node->FirstChildElement("render-state");
		while (render_state_node)
		{
			handleRenderStateNode(filepath, render_state_node, createParams);
			render_state_node = render_state_node->NextSiblingElement("render-state");
		}
	}

	return true;
}

bool CResourceXmlParser::parsePipelineFile(const std::string& filepath, std::vector<SPipelineCreateParams>& createParamsArray) const
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_NO_ERROR)
		return false;

	/* get the pipeline root node. */
	tinyxml2::XMLElement* root_node = doc.FirstChildElement("pipelines");
	if (!root_node)
	{
		GF_PRINT_CONSOLE_INFO("The <pipelines> is not the root element in the pipeline file: %s.\n", filepath.c_str());
		return false;
	}

	tinyxml2::XMLElement* pipeline_node = root_node->FirstChildElement("pipeline");

	while (pipeline_node)
	{
		SPipelineCreateParams createParams;
		if (handlePipelineNode(filepath, pipeline_node, createParams))
			createParamsArray.push_back(createParams);
		
		pipeline_node = pipeline_node->NextSiblingElement("pipeline");
	}

	return true;
}


E_PRIMITIVE_TYPE CResourceXmlParser::getPrimitiveType(std::string& s) const
{
	std::transform(s.begin(), s.end(), s.begin(), std::toupper);
	auto it = mPrimitiveTypeMapping.find(s);
	if (it != mPrimitiveTypeMapping.end())
		return it->second;

	return EPT_UNDEFINED;
}

bool CResourceXmlParser::handleInputLayoutElementNode(const std::string& filepath, tinyxml2::XMLElement* node, SInputLayoutElement& element) const
{
	// the 'semantic-name' and 'format' attributes are necessary, others are optional. 
	const char* semanticName = node->Attribute("semantic");
	if (!semanticName)
	{
		GF_PRINT_CONSOLE_INFO("The <element>('%s') doesn't have a valid semantic name in the pipeline file(%s).\n", 
			node->Name(), filepath.c_str());
		return false;
	}
	element.SemanticName = semanticName;

	const char* format = node->Attribute("format");
	if (!format)
	{
		GF_PRINT_CONSOLE_INFO("The <element>('%s') doesn't have a 'format' attribute in the pipeline file(%s).\n", 
			node->Name(), filepath.c_str());
		return false;
	}

	element.Format = getInputLayoutFormat(std::string(format));
	if (element.Format == EGF_UNKNOWN)
	{
		GF_PRINT_CONSOLE_INFO("The Format '%s' is not valid format of <element> '%s' in the pipeline file(%s).\n",
			format, node->Name(), filepath.c_str());
		return false;
	}

	// if the user does not give 'index' attribute in the xml, this code will return 0, which is the default.
	u32 semanticIndex = node->UnsignedAttribute("index");
	element.SemanticIndex = semanticIndex;

	// get the slot
	u32 slot = node->UnsignedAttribute("slot");
	element.Slot = slot;

	// get the 'instance' attribute
	bool isInstance;
	isInstance = node->BoolAttribute("instance");
	element.InstanceData = isInstance;

	if (element.InstanceData)
	{
		element.InstanceDataStepRate = node->UnsignedAttribute("instance-rate");
	}
	else
	{
		element.InstanceDataStepRate = 0;
	}
	return true;
}

E_GI_FORMAT	CResourceXmlParser::getInputLayoutFormat(std::string& s) const
{
	std::transform(s.begin(), s.end(), s.begin(), std::toupper);
	auto it = mVertexFormatMapping.find(s);
	if (it != mVertexFormatMapping.end())
		return it->second;

	return EGF_UNKNOWN;

}

bool CResourceXmlParser::handleShaderNode(const std::string& filepath,
	E_SHADER_TYPE shaderType,
	tinyxml2::XMLElement* node,
	SPipelineCreateParams& createParams) const
{
	SShaderCreateParams shaderCreateParams;
	shaderCreateParams.Type = shaderType;
	
	const char* shaderFilename = node->Attribute("file");
	if (!shaderFilename)
	{
		GF_PRINT_CONSOLE_INFO("A 'file' attribute must be in the <'%s'> element in the %s.\n", 
			node->Name(), filepath.c_str());
		return false;
	}

	shaderCreateParams.FileName = shaderFilename;

	const char* functionName = node->Attribute("main");
	if (!functionName)
	{
		GF_PRINT_CONSOLE_INFO("A 'main' attribute must be in the <'%s'> element in the %s.\n",
			node->Name(), filepath.c_str());
		return false;
	}

	shaderCreateParams.FunctionName = functionName;

	// find the auto variables of this shader
	tinyxml2::XMLElement* variable_node = node->FirstChildElement("variable");

	while (variable_node)
	{
		handleShaderVariableNode(filepath, shaderType, variable_node, createParams);
		variable_node = variable_node->NextSiblingElement("variable");
	}

	createParams.Shaders.push_back(shaderCreateParams);

	return true;
}

bool CResourceXmlParser::handleShaderVariableNode(const std::string& filepath,
	E_SHADER_TYPE shaderType,
	tinyxml2::XMLElement* node,
	SPipelineCreateParams& createParams) const
{
	const char* variableName = node->Attribute("name");
	if (!variableName)
	{
		GF_PRINT_CONSOLE_INFO("A 'name' attribute must be in the <'%s'> element in the %s.\n",
			node->Name(), filepath.c_str());
		return false;
	}

	const char* variableValue = node->Attribute("value");
	if (!variableValue)
	{
		GF_PRINT_CONSOLE_INFO("A 'value' attribute must be in the <'%s'> element in the %s.\n",
			node->Name(), filepath.c_str());
		return false;
	}

	u32 indexParam = node->UnsignedAttribute("index");
	auto it = mShaderVariableMapping.find(variableValue);

	// if the variable doesn't have effective meaning, just neglect it
	if (it == mShaderVariableMapping.end())
	{
		GF_PRINT_CONSOLE_INFO("'%s' is not a valid value of shader variable in this %s.\n",
			variableValue, filepath.c_str());
		return false;
	}

	// get the update-frequency attribute
	E_UPDATE_FREQUENCY freq = it->second.DefaultUpdateFrequency;
	const char* freqText = node->Attribute("update-frequency");
	if (freqText)
	{
		if (stricmp(freqText, "frame") == 0)
			freq = EUF_PER_FRAME;
		else if (stricmp(freqText, "object") == 0)
			freq = EUF_PER_OBJECT;
	}

	SShaderAutoVariable var;
	var.VariableName = variableName;
	var.Type = it->second.Meaning;
	var.ShaderType = shaderType;
	var.IndexParam = indexParam;
	var.UpdateFrequency = freq;

	createParams.ShaderAutoVariables.push_back(var);

	return true;
}

bool CResourceXmlParser::handleRenderStateNode(const std::string& filepath,
	tinyxml2::XMLElement* node,
	SPipelineCreateParams& createParams) const
{
	const char* name = node->Attribute("name");
	if (!name)
	{
		GF_PRINT_CONSOLE_INFO("The render state must have 'name' attribute in pipeline file %s.\n", filepath.c_str());
		return false;
	}

	const char* value_str = node->Attribute("value");
	if (!value_str)
	{
		GF_PRINT_CONSOLE_INFO("The render state must have 'value' attribute in pipeline file %s.\n", filepath.c_str());
		return false;
	}

	E_RENDER_STATE renderStateType = getRenderStateType(std::string(name));
	if (renderStateType == ERS_UNKNOWN)
	{
		GF_PRINT_CONSOLE_INFO("'%s' is not a valid name of a render state in pipeline file %s.\n", 
			name, filepath.c_str());
		return false;
	}

	SRenderStateCreateParams rsCreateParams;
	rsCreateParams.StateType = renderStateType;

	u32 value;
	bool boolValue;
	bool bValidRenderState = false;
	switch (renderStateType)
	{
		/* if the render state's value need a float. */
	case ERS_DEPTH_BIAS_CLAMP:
		if (node->QueryFloatAttribute("value", &rsCreateParams.FloatValue) == tinyxml2::XML_NO_ERROR)
			bValidRenderState = true;
		break;
		/* if the render state' value need a E_FILL_MODE. */
	case ERS_FILL_MODE:
		if (getRenderStateValue(std::string(value_str), mFillModeMapping, rsCreateParams.DwordValue))
			bValidRenderState = true;
		break;
		/* if the render state's value need a E_CULL_MODE. */
	case ERS_CULL_MODE:
		if (getRenderStateValue(std::string(value_str), mCullModeMapping, rsCreateParams.DwordValue))
			bValidRenderState = true;
		break;
		/* if the render state's value need a E_BLEND_TYPE. */
	case ERS_SRC_BLEND:
	case ERS_DEST_BLEND:
	case ERS_SRC_BLEND_ALPHA:
	case ERS_DEST_BLEND_ALPHA:
		if (getRenderStateValue(std::string(value_str), mBlendTypeMapping, rsCreateParams.DwordValue))
			bValidRenderState = true;
		break;
		/* if the render state's value need a E_BLEND_OP. */
	case ERS_BLEND_OP:
	case ERS_BLEND_OP_ALPHA:
		if (getRenderStateValue(std::string(value_str), mBlendOpMapping, rsCreateParams.DwordValue))
			bValidRenderState = true;
		break;
		/* if the render state's value need a E_COMPARISON_FUNC. */
	case ERS_DEPTH_FUNC:
	case ERS_STENCIL_FUNC:
	case ERS_BACKFACE_STENCIL_FUNC:
		if (getRenderStateValue(std::string(value_str), mComparisonFuncMapping, rsCreateParams.DwordValue))
			bValidRenderState = true;
		break;
		/* if the render state's value need a E_STENCIL_OP. */
	case ERS_STENCIL_FAIL_OP:
	case ERS_STENCIL_DEPTH_FAIL_OP:
	case ERS_STENCIL_PASS_OP:
	case ERS_BACKFACE_STENCIL_FAIL_OP:
	case ERS_BACKFACE_STENCIL_DEPTH_FAIL_OP:
	case ERS_BACKFACE_STENCIL_PASS_OP:
		if (getRenderStateValue(std::string(value_str), mStencilOpMapping, rsCreateParams.DwordValue))
			bValidRenderState = true;
		break;
		/* if the render state's value need a bool value.*/
	case ERS_SCISSOR_ENABLE:
	case ERS_MULTISAMPLE_ENABLE:
	case ERS_ANTI_LINE_ENABLE:
	case ERS_BLEND_ENABLE:
	case ERS_ALPHA_COVERAGE_ENABLE:
	case ERS_INDEPENDENT_BLEND_ENABLE:
	case ERS_DEPTH_ENABLE:
	case ERS_DEPTH_WRITE_ENABLE:
	case ERS_STENCIL_ENABLE:
		if (getBoolValue(value_str, boolValue))
		{
			rsCreateParams.DwordValue = boolValue;
			bValidRenderState = true;
		}
		break;
		/* just need a unsigned int. may be a mask */
	default:
		if (getMaskValue(value_str, rsCreateParams.DwordValue))
			bValidRenderState = true;
		break;
	}

	if (bValidRenderState)
	{
		createParams.RenderStates.push_back(rsCreateParams);
	}
	else
	{
		GF_PRINT_CONSOLE_INFO("The '%s' render state' value ('%s') is not valid in the file(%s)\n", name, value_str, filepath.c_str());
	}

	return bValidRenderState;
}

E_RENDER_STATE CResourceXmlParser::getRenderStateType(std::string& s) const
{
	std::transform(s.begin(), s.end(), s.begin(), std::toupper);
	auto it = mRenderStateMapping.find(s);
	if (it != mRenderStateMapping.end())
		return it->second;

	return ERS_UNKNOWN;
}

bool CResourceXmlParser::getRenderStateValue(std::string& s, const std::map<std::string, u32> mapping, u32& value) const
{
	std::transform(s.begin(), s.end(), s.begin(), std::toupper);
	auto it = mapping.find(s);
	if (it != mapping.end())
	{
		value = it->second;
		return true;
	}

	return false;
}

bool CResourceXmlParser::getBoolValue(const char* s, bool& value) const
{
	if (stricmp(s, "true") == 0)
	{
		value = true;
		return true;
	}
	
	if (stricmp(s, "false") == 0)
	{
		value = false;
		return true;
	}

	return false;
}

bool CResourceXmlParser::getMaskValue(const char* s, u32& value) const
{
	/* 像STENCIL_READ_MASK等掩码，用户可能指定10进制或16进制的值，
	因此需要对字符串进行判断，如果以0x开头的则表明是十六进制*/

	//如果是16进制
	u32 len = strlen(s);
	if (len > 2 && s[0] == '0' && (s[1] == 'x' || s[2] == 'X'))
	{
		if (sscanf_s(s + 2, "%x", &value) == 1)
			return true;
		else
			return false;
	}

	//如果是10进制
	if (sscanf_s(s, "%d", &value) == 1)
		return true;
	return false;
}

bool CResourceXmlParser::parseMaterialFile(const std::string& filepath, std::vector<SMaterialCreateParams>& createParamsArray) const
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_NO_ERROR)
		return false;

	/* get the pipeline root node. */
	tinyxml2::XMLElement* root_node = doc.FirstChildElement("materials");
	if (!root_node)
	{
		GF_PRINT_CONSOLE_INFO("The <materials> is not the root element in the material file: %s.\n", filepath.c_str());
		return false;
	}

	tinyxml2::XMLElement* material_node = root_node->FirstChildElement("material");

	while (material_node)
	{
		SMaterialCreateParams createParams;
		if (handleMaterialNode(filepath, material_node, createParams))
			createParamsArray.push_back(createParams);

		material_node = material_node->NextSiblingElement("material");
	}
	return true;
}

bool CResourceXmlParser::handleMaterialNode(const std::string& filepath,
	tinyxml2::XMLElement* node,
	SMaterialCreateParams& createParams) const
{
	const char* name = node->Attribute("name");
	if (!name)
	{
		GF_PRINT_CONSOLE_INFO("One material in file '%s' doesn't have 'name' attribute, which is a must \
							  							  							  so it can't be created.", filepath.c_str());
		return false;
	}

	createParams.Name = name;

	/* handle the ambient, diffuse, specular, emissive node, if it exists.*/
	tinyxml2::XMLElement* ambient_node = node->FirstChildElement("ambient");
	if (ambient_node)
		handleMaterialColorNode(filepath, createParams.Name, ambient_node, createParams.MaterialColors.Ambient);

	tinyxml2::XMLElement* diffuse_node = node->FirstChildElement("diffuse");
	if (diffuse_node)
		handleMaterialColorNode(filepath, createParams.Name, diffuse_node, createParams.MaterialColors.Diffuse);

	tinyxml2::XMLElement* specular_node = node->FirstChildElement("specular");
	if (specular_node)
		handleMaterialColorNode(filepath, createParams.Name, specular_node, createParams.MaterialColors.Specular);

	tinyxml2::XMLElement* emissive_node = node->FirstChildElement("emissive");
	if (emissive_node)
		handleMaterialColorNode(filepath, createParams.Name, emissive_node, createParams.MaterialColors.Emissive);

	/* handle the pipelines */
	tinyxml2::XMLElement* pipelines_node = node->FirstChildElement("pipelines");
	if (!pipelines_node)
	{
		GF_PRINT_CONSOLE_INFO("The material '%s' must have a <pipelines> node, and at least one <pipeline> sub-node \
				in the file(%s).\n", name, filepath.c_str());

		return false;
	}

	tinyxml2::XMLElement* pipeline_node = pipelines_node->FirstChildElement("pipeline");
	while (pipeline_node)
	{
		const char* pipelineName = pipeline_node->Attribute("name");
		if (pipelineName)
		{
			createParams.PipelineNames.push_back(pipelineName);
		}
		pipeline_node = pipeline_node->NextSiblingElement("pipeline");
	}

	// the material at least has one pipeline
	if (createParams.PipelineNames.empty())
	{
		GF_PRINT_CONSOLE_INFO("The material named '%s' must at least have one valid pipeline. (file location: %s).\n",
			name, filepath.c_str());
		return false;
	}

	/* handle the textures */
	tinyxml2::XMLElement* textures_node = node->FirstChildElement("textures");
	if (textures_node)
	{
		tinyxml2::XMLElement* texture_node = textures_node->FirstChildElement("texture");
		/* 这个数组用来记录每个层中纹理的个数（共8层），如果某层的纹理数超过1，则给出警告. */
		u32 textureIndexOccupies[8] = { 0 };
		while (texture_node)
		{
			SMaterialTextureParam textureParam;
			if (handleMaterialTextureNode(filepath, createParams.Name, texture_node, textureParam))
			{
				createParams.TextureParams.push_back(textureParam);
				textureIndexOccupies[textureParam.Index]++;
			}
			texture_node = texture_node->NextSiblingElement("texture");
		}

		/* check if two textures with the same index. if so, give warning. */
		for (u32 i = 0; i < SMaterial::MAX_TEXTURE_COUNT; i++)
		{
			if (textureIndexOccupies[i] > 1)
			{
				GF_PRINT_CONSOLE_INFO("WARNING: The material '%s' has textures with same index, \
								 which may cause an error at runtime. (file location: %s)",
								 name, filepath.c_str());
				break;
			}
		}
	}

	return true;
}

bool CResourceXmlParser::handleMaterialTextureNode(const std::string& filepath,
	const std::string& materialName,
	tinyxml2::XMLElement* node, SMaterialTextureParam& param) const
{
	const char* textureName = node->Attribute("name");
	if (!textureName)
	{
		GF_PRINT_CONSOLE_INFO("The <texture> element must have 'name' attribute in material named '%s'.\
							  	 (file location: %s", materialName.c_str(), filepath.c_str());
		return false;
	}

	u32 index = node->UnsignedAttribute("index");
	param.Name = textureName;
	param.Index = index;

	return true;
}


bool CResourceXmlParser::handleMaterialColorNode(const std::string& filepath,
	const std::string& materialName,
	tinyxml2::XMLElement* node, 
	XMFLOAT4& color) const
{
	const char* color_str = node->Attribute("color");
	if (!color_str)
	{
		GF_PRINT_CONSOLE_INFO("The <%s> element doesn't have a 'color' attribute, which is a must.\
							  							  in the material(%s) and the file(%s)",
														  node->Name(), materialName.c_str(), filepath.c_str());
		return false;
	}

	/* set default color values. */
	color.x = color.y = color.z = 0.0f;
	color.w = 1.0f;

	/* parse the color string - like : "1.2f, 0.2, 0.5f, 1.0f" */
	/* remove the 'f' charactor, which may be a user's habit. */
	char s[64];
	memset(s, 0, sizeof(s));
	const char* p1 = color_str;
	char* p2 = s;
	while (*p1 != '\0')
	{
		if ((*p1 >= '0' && *p1 <= '9') || *p1 == '.' || *p1 == ',')
		{
			(*p2++) = *p1;
		}
		p1++;
	}

	sscanf_s(s, "%f,%f,%f,%f", &color.x, &color.y, &color.z, &color.w);

	return true;
}


