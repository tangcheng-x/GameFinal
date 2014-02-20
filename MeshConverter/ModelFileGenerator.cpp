#include "ModelFileGenerator.h"
#include <fstream>

/* 产生用于material中color属性的颜色字符串, 格式为(r, g, b, a)*/
void generateColorString(XMFLOAT4 color, char s[128])
{
	sprintf(s, "%f, %f, %f, %f", color.x, color.y, color.z, color.w);
}

void generatePipelineName(const std::string& file_name_without_ext, char s[128], bool animated)
{
	if (animated)
		sprintf(s, "%s/skeleton_pipeline_01", file_name_without_ext.c_str());
	else
		sprintf(s, "%s/pipeline_01", file_name_without_ext.c_str());
}

/* 判断该material的流水线是否是动画的 */
bool isAnimatedMaterial(const SModelMaterial& material, const std::vector<SModelSubsetWrapper>& subsets)
{
	for (u32 i = 0; i < subsets.size(); i++)
	{
		if (stricmp(material.Name, subsets[i].Subset.MaterialName) == 0)
			return subsets[i].Subset.Skinned;
	}
	return false;
}

bool generateMaterialFile(const std::string& file_name_without_ext, std::vector<SModelMaterial>& materials, 
	const std::vector<SModelSubsetWrapper>& subsets)
{
	char color_str[128];
	char pipeline_name[128];	//the 'name' attribute of <pipeline>
	char animate_pipeline_name[128];

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* root_node = doc.NewElement("materials");

	// generate the pipeline' name
	
	generatePipelineName(file_name_without_ext, pipeline_name, false);
	generatePipelineName(file_name_without_ext, animate_pipeline_name, true);

	doc.InsertFirstChild(root_node);

	for (u32 i = 0; i < materials.size(); i++)
	{
		const SModelMaterial& material = materials[i];
		bool isAnimated = isAnimatedMaterial(material, subsets);
		
		// create the <material> node
		tinyxml2::XMLElement* material_node = doc.NewElement("material");
		
		//set the 'name' attribute of <material>
		material_node->SetAttribute("name", material.Name);

		// create the <ambient> sub-node
		tinyxml2::XMLElement* ambient_node = doc.NewElement("ambient");
		generateColorString(material.Ambient, color_str);
		ambient_node->SetAttribute("color", color_str);

		// create the <diffuse> sub-node
		tinyxml2::XMLElement* diffuse_node = doc.NewElement("diffuse");
		generateColorString(material.Diffuse, color_str);
		diffuse_node->SetAttribute("color", color_str);

		// create the <specular> sub-node
		tinyxml2::XMLElement* specular_node = doc.NewElement("specular");
		generateColorString(material.Specular, color_str);
		specular_node->SetAttribute("color", color_str);

		// create the emissive sub-node 
		tinyxml2::XMLElement* emissive_node = doc.NewElement("emissive");
		generateColorString(material.Emissive, color_str);
		emissive_node->SetAttribute("color", color_str);

		// add the four colos's node to <material>
		material_node->InsertEndChild(ambient_node);
		material_node->InsertEndChild(diffuse_node);
		material_node->InsertEndChild(specular_node);
		material_node->InsertEndChild(emissive_node);

		// create the <pipelines> node
		tinyxml2::XMLElement* pipelines_node = doc.NewElement("pipelines");
		tinyxml2::XMLElement* pipeline_node = doc.NewElement("pipeline");
		// set the 'name' attribute of <pipeline> node

		if (!isAnimated)
			pipeline_node->SetAttribute("name", pipeline_name);
		else
			pipeline_node->SetAttribute("name", animate_pipeline_name);

		pipelines_node->InsertEndChild(pipeline_node);

		// add the <pipelines> node to <material>
		material_node->InsertEndChild(pipelines_node);

		// create the <textures> node
		tinyxml2::XMLElement* textures_node = doc.NewElement("textures");
		
		for (u32 j = 0; j < material.TextureNames.size(); j++)
		{
			const std::string& texture_name = material.TextureNames[j];
			// create a <texture> sub-node
			tinyxml2::XMLElement* texture_node = doc.NewElement("texture");
			texture_node->SetAttribute("name", texture_name.c_str());

			// if the material textures' count <= 1, then it's no use creating the index
			if (material.TextureNames.size() > 1)
			{
				texture_node->SetAttribute("index", j);
			}

			textures_node->InsertEndChild(texture_node);
		}

		// add the <textures> node to <material>
		material_node->InsertEndChild(textures_node);

		root_node->InsertEndChild(material_node);
	}

	const std::string& material_file_name = file_name_without_ext + std::string(".material.xml");
	doc.SaveFile(material_file_name.c_str());

	return true;
}

void addVertexElementNode(tinyxml2::XMLDocument& doc, 
	tinyxml2::XMLElement* inputlayout_node,
	const char* semantic, 
	const char* format)
{
	tinyxml2::XMLElement* element_node = doc.NewElement("element");
	element_node->SetAttribute("semantic", semantic);
	element_node->SetAttribute("format", format);
	inputlayout_node->InsertEndChild(element_node);
}

const char* getBoneIndicesFormat(u32 count)
{
	if (count == 1)
		return "r8_uint";
	else if (count == 2)
		return "r8g8_uint";
	else if (count == 3 || count == 4)
		return "byte3";
}

bool generatePipelineFile(const std::string& file_name_without_ext, const SModelFileHeader& header, bool animated)
{
	char					pipeline_name[128];
	char					shader_file_name[128];
	char					pipeline_file_name[128];
	tinyxml2::XMLDocument	doc;

	
	// generate the pipeline's name
	generatePipelineName(file_name_without_ext, pipeline_name, animated);

	// generate shader file's name
	sprintf_s(shader_file_name, "%s.hlsl", file_name_without_ext.c_str());

	// generate pipeline file's name
	if (animated)
		sprintf_s(pipeline_file_name, "%s_skeleton.pipeline.xml", file_name_without_ext.c_str());
	else
		sprintf_s(pipeline_file_name, "%s.pipeline.xml", file_name_without_ext.c_str());

	tinyxml2::XMLElement* root_node = doc.NewElement("pipelines");
	doc.InsertEndChild(root_node);
	
	tinyxml2::XMLElement* pipeline_node = doc.NewElement("pipeline");
	pipeline_node->SetAttribute("name", pipeline_name);
	root_node->InsertEndChild(pipeline_node);

	//create input-layout node
	tinyxml2::XMLElement* inputlayout_node = doc.NewElement("input-layout");
	
	// create the input-layout's <element> node according to the vertex Components
	if (header.VertexElements & EMVE_POSITION)
		addVertexElementNode(doc, inputlayout_node, "POSITION", "float3");
	if (header.VertexElements & EMVE_NORMAL)
		addVertexElementNode(doc, inputlayout_node, "NORMAL", "float3");
	if (header.VertexElements & EMVE_TANGENT)
		addVertexElementNode(doc, inputlayout_node, "TANGENT", "float3");
	if (header.VertexElements & EMVE_BINORMAL)
		addVertexElementNode(doc, inputlayout_node, "BINORMAL", "float3");
	if (header.VertexElements & EMVE_TEXCOORD)
		addVertexElementNode(doc, inputlayout_node, "TEXCOORD", "float2");

	if (animated)
	{
		addVertexElementNode(doc, inputlayout_node, "BONEINDICES", "byte4");
		addVertexElementNode(doc, inputlayout_node, "WEIGHTS", "float3");
	}

	// add the <input-layout> node to the <pipeline>
	pipeline_node->InsertEndChild(inputlayout_node);

	// create shaders node 
	tinyxml2::XMLElement* shaders_node = doc.NewElement("shaders");

	// create <vertex-shader>
	tinyxml2::XMLElement* vertex_shader_node = doc.NewElement("vertex-shader");

	// set the vertex shader's file name and the main function
	vertex_shader_node->SetAttribute("file", shader_file_name);
	if (animated)
		vertex_shader_node->SetAttribute("main", "skeleton_vs_main");
	else
		vertex_shader_node->SetAttribute("main", "vs_main");

	// set 'world matrix' 'viewproj_matrix' variable in vertex shader
	tinyxml2::XMLElement* world_variable_node = doc.NewElement("variable");
	world_variable_node->SetAttribute("name", "gWorld");
	world_variable_node->SetAttribute("value", "world_matrix");

	tinyxml2::XMLElement* viewproj_variable_node = doc.NewElement("variable");
	viewproj_variable_node->SetAttribute("name", "gViewProj");
	viewproj_variable_node->SetAttribute("value", "viewproj_matrix");

	vertex_shader_node->InsertEndChild(world_variable_node);
	vertex_shader_node->InsertEndChild(viewproj_variable_node);

	if (animated)
	{
		tinyxml2::XMLElement* bone_variable_node = doc.NewElement("variable");
		bone_variable_node->SetAttribute("name", "gBoneTransforms");
		bone_variable_node->SetAttribute("value", "bone_transforms");
		vertex_shader_node->InsertEndChild(bone_variable_node);
	}

	// create <pixel-shader>
	tinyxml2::XMLElement* pixel_shader_node = doc.NewElement("pixel-shader");

	// set the pixel shader's file name and main function
	pixel_shader_node->SetAttribute("file", shader_file_name);
	pixel_shader_node->SetAttribute("main", "ps_main");

	// if the vertex components include "Texcoord", then add on 'texture' variable
	if (header.VertexElements & EMVE_TEXCOORD)
	{
		tinyxml2::XMLElement* texture_variable_node = doc.NewElement("variable");
		texture_variable_node->SetAttribute("name", "gTexture");
		texture_variable_node->SetAttribute("value", "texture");
		pixel_shader_node->InsertEndChild(texture_variable_node);
	}

	// add <vertex-shader> <pixel-shader> shader to <shaders>
	shaders_node->InsertEndChild(vertex_shader_node);
	shaders_node->InsertEndChild(pixel_shader_node);

	// add <shaders> to <pipeline>
	pipeline_node->InsertEndChild(shaders_node);

	// add some comments to remind user to add render-state
	tinyxml2::XMLComment* render_states_comment = doc.NewComment("You can put render-states here.");
	pipeline_node->InsertEndChild(render_states_comment);

	doc.SaveFile(pipeline_file_name);

	return true;
}

bool generateShaderFile(const std::string& file_name_without_ext, const SModelFileHeader& header)
{
	std::string	shader_file_name = file_name_without_ext + std::string(".hlsl");

	std::ofstream file;
	file.open(shader_file_name, std::ios::out);

	if (file.fail())
	{
		printf("%s", "Can not create shader file %s.\n", shader_file_name.c_str());
		return false;
	}
	
	file << "cbuffer cbPerObject\n"
		<< "{\n"
		<< "\tfloat4x4 gWorld;\n";
	if (header.AnimatedMesh)
	{
		file << "\tfloat4x4 gBoneTransforms[96];\n";
	}

	file << "};\n\n";

	file << "cbuffer cbPerFrame\n"
		<< "{\n"
		<< "\tfloat4x4 gViewProj;\n"
		<< "};\n\n";

	if (header.VertexElements & EMVE_TEXCOORD)
	{
		file << "Texture2D gTexture;\n"
			<< "SamplerState gSampleState;\n\n";
	}

	// write the VertexIn struct.
	if (header.VertexCount > 0)
	{
		file << "struct VertexIn" << "\n"
			<< "{\n";

		if (header.VertexElements & EMVE_POSITION)
			file << "\t" << "float3 PosL : POSITION;" << "\n";

		if (header.VertexElements & EMVE_NORMAL)
			file << "\t" << "float3 NormalL : NORMAL;" << "\n";

		if (header.VertexElements & EMVE_TANGENT)
			file << "\t" << "float3 TangentL : TANGENT;" << "\n";

		if (header.VertexElements & EMVE_BINORMAL)
			file << "\t" << "float3 BinormalL : BINORMAL;" << "\n";

		if (header.VertexElements & EMVE_TEXCOORD)
			file << "\t" << "float2 Tex : TEXCOORD;" << "\n";

		file << "};\n\n";
	}
	

	// the SkeletonVertexIn struct
	if (header.AnimateVertexCount > 0)
	{
		file << "struct SkeletonVertexIn" << "\n"
			<< "{\n";

		if (header.VertexElements & EMVE_POSITION)
			file << "\t" << "float3 PosL : POSITION;" << "\n";

		if (header.VertexElements & EMVE_NORMAL)
			file << "\t" << "float3 NormalL : NORMAL;" << "\n";

		if (header.VertexElements & EMVE_TANGENT)
			file << "\t" << "float3 TangentL : TANGENT;" << "\n";

		if (header.VertexElements & EMVE_BINORMAL)
			file << "\t" << "float3 BinormalL : BINORMAL;" << "\n";

		if (header.VertexElements & EMVE_TEXCOORD)
			file << "\t" << "float2 Tex : TEXCOORD;" << "\n";

		file << "\t" << "uint4  BoneIndices : BONEINDICES;" << "\n";
		file << "\t" << "float3 Weights : WEIGHTS;" << "\n";

		file << "};\n\n";
	}

	// write the VertexOut struct

	file << "struct VertexOut" << "\n"
		<< "{\n";

	file << "\t" << "float4 PosH : SV_POSITION;" << "\n";
	if (header.VertexElements & EMVE_TEXCOORD)
	{
		file << "\t" << "float2 Tex : TEXCOORD;" << "\n";
	}

	file << "};\n\n";
	
	/* 顶点着色器代码 */

	if (header.VertexCount > 0)
	{
		file << "VertexOut vs_main(VertexIn vin)" << "\n"
			<< "{\n"
			<< "\t" << "VertexOut vout;" << "\n"
			<< "\t" << "float4 PosW = mul(float4(vin.PosL, 1.0f), gWorld);" << "\n"
			<< "\t" << "vout.PosH = mul(PosW, gViewProj);" << "\n";

		if (header.VertexElements & EMVE_TEXCOORD)
			file << "\t" << "vout.Tex = vin.Tex;" << "\n";

		file << "\t" << "return vout;" << "\n";
		file << "}\n\n";
	}

	if (header.AnimateVertexCount > 0)
	{
		file << "VertexOut skeleton_vs_main(SkeletonVertexIn vin)" << "\n"
			<< "{\n"
			<< "\t" << "VertexOut vout;" << "\n"
			<< "\t" << "float4 PosW = mul(float4(vin.PosL, 1.0f), gWorld);" << "\n"
			<< "\t" << "vout.PosH = mul(PosW, gViewProj);" << "\n";

		if (header.VertexElements & EMVE_TEXCOORD)
			file << "\t" << "vout.Tex = vin.Tex;" << "\n";

		file << "\t" << "return vout;" << "\n";
		file << "}\n\n";
	}

	
	/* 像素着色器代码 */
	file << "float4 ps_main(VertexOut pin) : SV_TARGET" << "\n"
		<< "{\n";

	if (header.VertexElements & EMVE_TEXCOORD)
		file << "\t" << "return gTexture.Sample(gSampleState, pin.Tex);" << "\n";
	else
		file << "\t" << "return float4(1.0f, 1.0f, 1.0f, 1.0f);" << "\n";

	file << "}\n\n";

	file.close();
	return true;
}

void writeSubsets(FILE* fp, const std::vector<SModelSubsetWrapper>& subset_wrappers)
{
	/* 先写入没有动画的子集，再写入有动画的子集 */
	for (u32 i = 0; i < subset_wrappers.size(); i++)
	{
		if (!subset_wrappers[i].Subset.Skinned)
			fwrite(&subset_wrappers[i].Subset, sizeof(SModelFileSubset), 1, fp);
	}

	for (u32 i = 0; i < subset_wrappers.size(); i++)
	{
		if (subset_wrappers[i].Subset.Skinned)
		{
			fwrite(&subset_wrappers[i].Subset, sizeof(SModelFileSubset), 1, fp);
		}	
	}

	/* 将含有动画的子集的骨骼信息写入 */
	for (u32 i = 0; i < subset_wrappers.size(); i++)
	{
		if (subset_wrappers[i].Subset.Skinned)
			fwrite(&subset_wrappers[i].SubsetBones[0], sizeof(SModelSubsetBone), subset_wrappers[i].SubsetBones.size(), fp);
	}
}

bool generateMeshFile(const std::string& file_name_without_ext,
	const SModelFileHeader& header,
	const std::vector<SModelSubsetWrapper>& subset_wrappers,
	const std::vector<SModelBoneWrapper>& bones,
	const std::vector<SBoneAnimationClip>& animation_clips,
	u8* vertex_buffer,
	u8* animate_vertex_buffer,
	u8* indice_buffer)
{
	char		mesh_file_name[128];
	sprintf_s(mesh_file_name, "%s.mesh", file_name_without_ext.c_str());

	FILE* fp = fopen(mesh_file_name, "wb");
	if (!fp)
	{
		printf_s("can't create mesh file.\n");
		return false;
	}

	fwrite(&header, sizeof(header), 1, fp);
	writeSubsets(fp, subset_wrappers);

	/* 如果是骨骼动画*/
	if (header.AnimatedMesh)
	{
		writeBonesTree(fp, bones);
		writeAnimations(fp, animation_clips);
	}

	if (header.VertexBufferSize > 0)
		fwrite(vertex_buffer, header.VertexBufferSize, 1, fp);

	if (header.AnimatedMesh && header.AnimateVertexBufferSize > 0)
		fwrite(animate_vertex_buffer, header.AnimateVertexBufferSize, 1, fp);

	fwrite(indice_buffer, header.IndexBufferSize, 1, fp);
	fclose(fp);

	return true;
}

