#ifndef __MODEL_FILE_GENERATOR_H__
#define __MODEL_FILE_GENERATOR_H__

#include "ModelReader.h"
#include "tinyxml2.h"

bool generateMaterialFile(const std::string& file_name_without_ext, 
	std::vector<SModelMaterial>& materials,
	const std::vector<SModelSubsetWrapper>& subsets);

bool generatePipelineFile(const std::string& file_name_without_ext, const SModelFileHeader& header, bool animated);

bool generateShaderFile(const std::string& file_name_without_ext, const SModelFileHeader& header);

bool generateMeshFile(const std::string& file_name_without_ext,
	const SModelFileHeader& header,
	const std::vector<SModelSubsetWrapper>& subset_wrappers,
	const std::vector<SModelBoneWrapper>& bones,
	const std::vector<SBoneAnimationClip>& animation_clips,
	u8* vertex_buffer,
	u8* animate_vertex_buffer,
	u8* indice_buffer);

/* 将骨骼树写入文件 */
void writeBonesTree(FILE* fp, const std::vector<SModelBoneWrapper>& bones);

/* 将动画信息写入 */
void writeAnimations(FILE* fp, const std::vector<SBoneAnimationClip>& clips);

void writeSubsets(FILE* fp, const std::vector<SModelSubsetWrapper>& subset_wrappers);

#endif

