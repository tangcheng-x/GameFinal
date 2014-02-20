#include "ModelReader.h"

void loadMaterials(const aiScene* scene,
	const std::string& file_name_without_ext,
	const std::string& file_ext,
	std::vector<SModelMaterial>& materials)
{
	materials.clear();
	materials.resize(scene->mNumMaterials);

	for (u32 i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* mtrl = scene->mMaterials[i];
		aiColor4D color;
		//diffuse
		mtrl->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		materials[i].Diffuse = XMFLOAT4(color.r, color.g, color.b, color.a);

		// x file has no ambient, so give diffuse color to it
		if (_stricmp(file_ext.c_str(), "x") == 0)
		{
			materials[i].Ambient = materials[i].Diffuse;
		}
		else
		{
			mtrl->Get(AI_MATKEY_COLOR_AMBIENT, color);
			materials[i].Ambient = XMFLOAT4(color.r, color.g, color.b, color.a);
		}

		//specular
		mtrl->Get(AI_MATKEY_COLOR_SPECULAR, color);
		materials[i].Specular = XMFLOAT4(color.r, color.g, color.b, color.a);

		//emissive
		mtrl->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		materials[i].Emissive = XMFLOAT4(color.r, color.g, color.b, color.a);

		aiString texture_path;
		u32 texture_count = mtrl->GetTextureCount(aiTextureType_DIFFUSE);
		for (u32 j = 0; j < texture_count; j++)
		{
			mtrl->GetTexture(aiTextureType_DIFFUSE, j, &texture_path);
			std::string name = texture_path.C_Str();
			materials[i].TextureNames.push_back(name);
		}

		// fill the name
		sprintf(materials[i].Name, "%s/material_%02d", file_name_without_ext.c_str(), i);
	}
}

bool parseFilePath(const std::string& fullpath, std::string& filename, std::string& name_without_ext, std::string& ext)
{
	u32 last_slash_pos = fullpath.find_last_of('/');
	if (last_slash_pos == std::string::npos)
	{
		/* if the os is windows, the '\' is also valid path separator */
		last_slash_pos = fullpath.find_last_of('\\');
	}

	if (last_slash_pos == std::string::npos)
		filename = fullpath;
	else
		filename = fullpath.substr(last_slash_pos + 1);

	/* 查找文件路径中最后一个'.'点号的位置 */
	u32 last_dot_pos = filename.find_last_of('.');

	/* 如果没有扩展名，则文件名不合法 */
	if (last_dot_pos == std::string::npos)
		return false;

	name_without_ext = filename.substr(0, last_dot_pos);
	ext = filename.substr(last_dot_pos + 1);

	return true;
}

void fillSubsets(const aiScene* scene,
	const std::vector<SModelMaterial>& materials,
	SModelFileHeader& file_header,
	std::vector<SModelSubsetWrapper>& subsets_wrappers)
{
	u32 vertex_base = 0;
	u32 animate_vertex_base = 0;
	u32 start_index_location = 0;
	file_header.AnimateSubsetCount = 0;
	u32 meshCount = scene->mNumMeshes;
	subsets_wrappers.resize(meshCount);
	for (u32 i = 0; i < meshCount; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		if (mesh->HasBones())
		{
			subsets_wrappers[i].Subset.BaseVertexLocation = animate_vertex_base;
			subsets_wrappers[i].Subset.Skinned = true;
			animate_vertex_base += mesh->mNumVertices;
			file_header.AnimateSubsetCount++;
		}
		else
		{
			subsets_wrappers[i].Subset.BaseVertexLocation = vertex_base;
			subsets_wrappers[i].Subset.Skinned = false;
			vertex_base += mesh->mNumVertices;
		}

		subsets_wrappers[i].Subset.StartIndexLocation = start_index_location;
		subsets_wrappers[i].Subset.IndexCount = mesh->mNumFaces * 3;
		strcpy_s(subsets_wrappers[i].Subset.MaterialName, materials[mesh->mMaterialIndex].Name);
		start_index_location += mesh->mNumFaces * 3;
	}

	// update the model file's header struct.
	file_header.VertexCount = vertex_base;
	file_header.AnimateVertexCount = animate_vertex_base;
	file_header.IndiceCount = start_index_location;
	file_header.SubsetCount = meshCount;

	/* if the indice's count exceed 0xffff, then it have to use 32 bit indice. */
	if (max(animate_vertex_base, vertex_base) > (1 << 16))
	{
		file_header.Bit32Indice = true;
		file_header.IndexBufferSize = sizeof(u32) * file_header.IndiceCount;
	}
	else
	{
		file_header.Bit32Indice = false;
		file_header.IndexBufferSize = sizeof(u16) *file_header.IndiceCount;
	}
		

	file_header.AnimateVertexBufferSize = animate_vertex_base * file_header.AnimateVertexStride;
	file_header.VertexBufferSize = vertex_base * file_header.VertexStride;
}

void calcVertexStride(SModelFileHeader& header)
{
	u32 stride = 0;
	u32 vertex_elements = header.VertexElements;

	if (vertex_elements & EMVE_POSITION)
		stride += sizeof(XMFLOAT3);

	if (vertex_elements & EMVE_NORMAL)
		stride += sizeof(XMFLOAT3);

	if (vertex_elements & EMVE_TANGENT)
		stride += sizeof(XMFLOAT3);

	if (vertex_elements & EMVE_BINORMAL)
		stride += sizeof(XMFLOAT3);

	if (vertex_elements & EMVE_TEXCOORD)
		stride += sizeof(XMFLOAT2);

	header.VertexStride = stride;
	stride += sizeof(u8)* 4;
	stride += sizeof(f32)* (header.BoneWeightCount - 1);
	header.AnimateVertexStride = stride;
}

/* 如果所有子集都不含有动画，则这个网格模型也不含有动画 */
bool isReallyAnimatedMesh(const aiScene* scene)
{
	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		if (mesh->HasBones())
			return true;
	}
	return false;
}

void fillVertexAndIndicesData(const aiScene* scene, 
	SModelFileHeader& header, 
	f32 mesh_scale, 
	const std::vector<SModelSubsetWrapper>& subset_wrappers,
	u8* pVertices, 
	u8* pAnimateVertices,
	u8* pIndices)
{
	u8* vertices;

	u32 vertex_elements = header.VertexElements;
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
	XMFLOAT2 texcoord;
	u8		 bone_indices[4];
	f32		 bone_weights[4];

	/* 用来存储AABB的最大点和最小点 */
	XMFLOAT3 min_point = XMFLOAT3(INF_FLOAT, INF_FLOAT, INF_FLOAT);
	XMFLOAT3 max_point = XMFLOAT3(-INF_FLOAT, -INF_FLOAT, -INF_FLOAT);
	
	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		u32 vertexCount = mesh->mNumVertices;
		u32 faceCount = mesh->mNumFaces;
		if (mesh->HasBones())
			vertices = pAnimateVertices;
		else
			vertices = pVertices;
		

		for (u32 j = 0; j < vertexCount; j++)
		{
			if (vertex_elements & EMVE_POSITION)
			{
				position.x = mesh->mVertices[j].x * mesh_scale;
				position.y = mesh->mVertices[j].y * mesh_scale;
				position.z = mesh->mVertices[j].z * mesh_scale;

				*(XMFLOAT3*)vertices = position;
				vertices += sizeof(XMFLOAT3);

				/* 这里需要更新AABB */
				if (position.x < min_point.x)
					min_point.x = position.x;
				if (position.y < min_point.y)
					min_point.y = position.y;
				if (position.z < min_point.z)
					min_point.z = position.z;

				if (position.x > max_point.x)
					max_point.x = position.x;
				if (position.y > max_point.y)
					max_point.y = position.y;
				if (position.z > max_point.z)
					max_point.z = position.z;
			}

			if (vertex_elements & EMVE_NORMAL)
			{
				normal.x = mesh->mNormals[j].x;
				normal.y = mesh->mNormals[j].y;
				normal.z = mesh->mNormals[j].z;

				*(XMFLOAT3*)vertices = normal;
				vertices += sizeof(XMFLOAT3);
			}

			if (vertex_elements & EMVE_TANGENT)
			{
				tangent.x = mesh->mTangents[j].x;
				tangent.y = mesh->mTangents[j].y;
				tangent.z = mesh->mTangents[j].z;

				*(XMFLOAT3*)vertices = tangent;
				vertices += sizeof(XMFLOAT3);
			}

			if (vertex_elements & EMVE_BINORMAL)
			{
				binormal.x = mesh->mBitangents[j].x;
				binormal.y = mesh->mBitangents[j].y;
				binormal.z = mesh->mBitangents[j].z;

				*(XMFLOAT3*)vertices = binormal;
				vertices += sizeof(XMFLOAT3);
			}

			if (vertex_elements & EMVE_TEXCOORD)
			{
				texcoord.x = mesh->mTextureCoords[0][j].x;
				texcoord.y = mesh->mTextureCoords[0][j].y;

				*(XMFLOAT2*)vertices = texcoord;
				vertices += sizeof(XMFLOAT2);
			}

			if (subset_wrappers[i].Subset.Skinned)
			{
				const std::vector<SVertexSkinInfo>& skinInfo = subset_wrappers[i].VertexSkinInfos[j];
				for (u32 k = 0; k < header.BoneWeightCount; k++)
				{
					if (k >= skinInfo.size())
					{
						bone_indices[k] = 0;
						bone_weights[k] = 0;
					}
					else
					{
						bone_indices[k] = skinInfo[k].BoneId;
						bone_weights[k] = skinInfo[k].Weight;
					}
				}

				memcpy(vertices, bone_weights, sizeof(f32)* (header.BoneWeightCount - 1));
				vertices += (header.BoneWeightCount - 1) * sizeof(f32);

				memcpy(vertices, bone_indices, sizeof(u8)* header.BoneWeightCount);
				vertices += 4 * sizeof(u8);
			}

			if (mesh->HasBones())
				pAnimateVertices = vertices;
			else
				pVertices = vertices;
		}

		// fill the indices data
		// if the mesh use 16 bit indices
		if (!header.Bit32Indice)
		{
			u16* indices = (u16*)pIndices;
			for (u32 j = 0; j < faceCount; j++)
			{
				indices[0] = mesh->mFaces[j].mIndices[0];
				indices[1] = mesh->mFaces[j].mIndices[1];
				indices[2] = mesh->mFaces[j].mIndices[2];

				indices += 3;
			}

			pIndices += faceCount * 3 * sizeof(u16);
		}
		else // if the mesh use 32 bit indices
		{
			u32* indices = (u32*)pIndices;
			for (u32 j = 0; j < faceCount; j++)
			{
				indices[0] = mesh->mFaces[j].mIndices[0];
				indices[1] = mesh->mFaces[j].mIndices[1];
				indices[2] = mesh->mFaces[j].mIndices[2];

				indices += 3;
			}
			pIndices += faceCount * 3 * sizeof(u32);
		}


	}

	/* 重新计算AABB */
	header.Aabb.Center.x = (min_point.x + max_point.x) * 0.5f;
	header.Aabb.Center.y = (min_point.y + max_point.y) * 0.5f;
	header.Aabb.Center.z = (min_point.z + max_point.z) * 0.5f;

	header.Aabb.Extents.x = (max_point.x - min_point.x) * 0.5f;
	header.Aabb.Extents.y = (max_point.y - min_point.y) * 0.5f;
	header.Aabb.Extents.z = (max_point.z - min_point.z) * 0.5f;
}
