#include <iostream>
#include <Windows.h>
#include <GameFinal.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/material.h>
#include "ModelReader.h"
#include "ModelFileGenerator.h"

#pragma comment(lib, "assimp.lib")

int main()
{
	SModelFileHeader					model_file_header;
	std::vector<SModelMaterial>			materials;
	std::string							file_full_path;
	std::string							file_name;
	std::string							file_name_without_ext;
	std::string							file_extension;
	u32									vertex_elements;
	u8*									vertex_buffer = nullptr;
	u8*									animate_vertex_buffer = nullptr;
	u8*									indice_buffer = nullptr;
	f32									mesh_scale = 1.0f;		/* 模型的缩放比例 */
	std::vector<SModelBoneWrapper>		bone_wrappers;
	std::vector<SModelSubsetWrapper>	subset_wrappers;
	std::vector<SBoneAnimationClip>		animation_clips;

	std::cout << "Please enter file name: " << std::endl;
	std::cin >> file_full_path;

	vertex_elements = EMVE_POSITION | EMVE_NORMAL | EMVE_TEXCOORD;
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(file_full_path.c_str(),
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	if (!scene)
	{
		printf("Can't load %s\n", file_full_path.c_str());
		return 1;
	}

	model_file_header.Version = 0x01;
	model_file_header.VertexElements = vertex_elements;
	model_file_header.BoneWeightCount = 4;
	model_file_header.AnimatedMesh = scene->HasAnimations();
	if (model_file_header.AnimatedMesh)
	{
		model_file_header.AnimatedMesh = isReallyAnimatedMesh(scene);
	}

	calcVertexStride(model_file_header);
	parseFilePath(file_full_path, file_name, file_name_without_ext, file_extension);
	
	loadMaterials(scene, file_name_without_ext, file_extension, materials);
	fillSubsets(scene, materials, model_file_header, subset_wrappers);

	if (model_file_header.AnimatedMesh)
	{
		buildBoneTree(bone_wrappers, scene->mRootNode, -1);
		model_file_header.BoneCount = bone_wrappers.size();
		fillSubsetsBones(subset_wrappers, bone_wrappers, model_file_header, scene);
		loadAnimations(model_file_header, animation_clips, bone_wrappers, scene);
	}

	if (model_file_header.VertexCount > 0)
		vertex_buffer = (u8*)malloc(model_file_header.VertexBufferSize);

	if (model_file_header.AnimateVertexCount > 0)
		animate_vertex_buffer = (u8*)malloc(model_file_header.AnimateVertexBufferSize);

	indice_buffer = (u8*)malloc(model_file_header.IndexBufferSize);

	fillVertexAndIndicesData(scene, model_file_header, mesh_scale,
		subset_wrappers, vertex_buffer, animate_vertex_buffer, indice_buffer);

	generateMaterialFile(file_name_without_ext, materials, subset_wrappers);

	if (model_file_header.AnimateVertexCount > 0)
		generatePipelineFile(file_name_without_ext, model_file_header, true);
	
	if (model_file_header.VertexCount > 0)
		generatePipelineFile(file_name_without_ext, model_file_header, false);

	generateShaderFile(file_name_without_ext, model_file_header);

	generateMeshFile(file_name_without_ext, model_file_header, subset_wrappers,
		bone_wrappers, animation_clips,
		vertex_buffer, animate_vertex_buffer, indice_buffer);

	system("pause");
	return 0;
}

