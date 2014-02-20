#include "stdafx.h"
#include "CResourceLoader.h"
#include "CResourceXmlParser.h"
#include "CModelFileParser.h"
#include "gfTypes.h"

CResourceLoader::CResourceLoader(IResourceGroupManager* ResourceGroupManager,
	ITextureManager* TextureManager,
	IShaderManager* ShaderManager,
	IInputLayoutManager* InputlayoutManager,
	IRenderStateManager* RenderStateManager,
	IPipelineManager* PipelineManager,
	IMaterialManager* MaterialManager,
	IMeshManager* MeshManager)
	:mResourceGroupManager(ResourceGroupManager)
	, mTextureManager(TextureManager)
	, mShaderManager(ShaderManager)
	, mInputlayoutManager(InputlayoutManager)
	, mRenderStateManager(RenderStateManager)
	, mPipelineManager(PipelineManager)
	, mMaterialManager(MaterialManager)
	, mMeshManager(MeshManager)
{
	mResourceXmlParser = new CResourceXmlParser;
	mModelFileParser = new CModelFileParser;
}

bool CResourceLoader::loadTexture(const std::string& name)
{
	std::string fullpath;
	if (!mResourceGroupManager->getFullPath(name, fullpath))
	{
		GF_PRINT_CONSOLE_INFO("The texture named %s doesn't exist.\n", name);
		return false;
	}

	ITexture* texture = mTextureManager->load(name, fullpath);
	if (texture == nullptr)
		return false;

	return true;
}

bool CResourceLoader::loadPipeline(const std::string& fullpath, const IResourceXmlParser::SPipelineCreateParams& createParams) const
{
	/* if the pipeline with the same name has already been loaded,
	maybe some mistake has occurred. such as two pipeline file with the same
	pipeline name. */
	IPipeline* pipeline = mPipelineManager->get(createParams.Name);
	if (pipeline)
	{
		GF_PRINT_CONSOLE_INFO("Pipeline '%s' (in the file '%s') has already been loaded. It can't been loaded again. \
							  			Do you put the pipelines with same names in different files ?\n",
										createParams.Name, fullpath.c_str());
		return false;
	}


	u32 shaderCount = createParams.Shaders.size();
	IShader* shaders[5];
	IShader* vertexShader;
	for (u32 i = 0; i < shaderCount; i++)
	{
		const IResourceXmlParser::SShaderCreateParams shaderCreateParams = createParams.Shaders[i];
		std::string shaderName = shaderCreateParams.FileName + std::string("-") + shaderCreateParams.FunctionName;
		IShader* shader = mShaderManager->get(shaderName);

		/* if the shader has not been loaded. Load it first. */
		if (!shader)
		{
			std::string shaderFullPath;
			if (!mResourceGroupManager->getFullPath(shaderCreateParams.FileName, shaderFullPath))
			{
				GF_PRINT_CONSOLE_INFO("Pipeline '%s' creation failed. Because the shader file '%s' doesn't exist.\n",
					createParams.Name.c_str(), shaderCreateParams.FileName.c_str());
				return false;
			}

			shader = mShaderManager->load(shaderCreateParams.Type, shaderFullPath, shaderCreateParams.FunctionName, shaderName);
			if (shader == nullptr)
			{
				GF_PRINT_CONSOLE_INFO("Pipeline '%s' creation failed. Due to the '%s' function in '%s' shader file.\n",
					createParams.Name.c_str(), shaderCreateParams.FunctionName.c_str(), shaderFullPath.c_str());
				return false;
			}
		}

		shaders[i] = shader;

		/* find the vertex shader, which will be used to create input-layout soon.*/
		if (shader->getType() == EST_VERTEX_SHADER)
		{
			vertexShader = shader;
		}
	}

	/* create the input-layout. */
	/* if the input-layout with the same layout has been created before, just get it.*/
	IInputLayout* inputLayout = mInputlayoutManager->get(createParams.InputLayoutElements);
	// if there is no input-layout with the same vertex formats. just create it.
	if (!inputLayout)
	{
		inputLayout = mInputlayoutManager->create(createParams.InputLayoutElements, vertexShader);
		if (!inputLayout)
		{
			GF_PRINT_CONSOLE_INFO("Pipeline '%s' creation failed. Due to the input-layout create failure in file '%s'.\n",
				createParams.Name, fullpath.c_str());
			return false;
		}
	}


	/* create render state */
	std::string rsname = createParams.Name + ".rs";
	IRenderState* pRenderState = mRenderStateManager->get(rsname);
	if (!pRenderState)
	{
		pRenderState = mRenderStateManager->create(rsname);
		if (!pRenderState)
		{
			GF_PRINT_CONSOLE_INFO("Pipeline '%s' creation failed. Due to the render-state create failure in file '%s'.\n",
				createParams.Name, fullpath.c_str());
			return false;
		}
	}

	// set all the render states.
	for (u32 i = 0; i < createParams.RenderStates.size(); i++)
	{
		const IResourceXmlParser::SRenderStateCreateParams& rsCreateParam = createParams.RenderStates[i];
		switch (rsCreateParam.StateType)
		{
			/* if the render-state need a float value */
		case ERS_DEPTH_BIAS_CLAMP:
			pRenderState->setFloat(rsCreateParam.StateType, rsCreateParam.FloatValue);
			break;
			/* others are unsigned int. */
		default:
			pRenderState->set(rsCreateParam.StateType, rsCreateParam.DwordValue);
			break;
		}
	}

	pRenderState->confirm();

	// create the pipeline object
	pipeline = mPipelineManager->create(createParams.Name, shaders, shaderCount, inputLayout,
		createParams.PrimitiveType, pRenderState);
	if (!pipeline)
	{
		GF_PRINT_CONSOLE_INFO("Pipeline '%s' creation failed (in file '%s').\n",
			createParams.Name, fullpath.c_str());

		// TODO: should drop pRenderState object.
		return false;
	}

	/* set the shader auto-inject variables. */
	for (u32 i = 0; i < createParams.ShaderAutoVariables.size(); i++)
	{
		pipeline->addShaderAutoVariable(createParams.ShaderAutoVariables[i]);
	}

	return true;
}

bool CResourceLoader::loadPipelinesFromFile(const std::string& name)
{
	
	std::string fullpath;
	if (!mResourceGroupManager->getFullPath(name, fullpath))
	{
		GF_PRINT_CONSOLE_INFO("The pipeline named %s doesn't exist.\n", name);
		return false;
	}

	std::vector<IResourceXmlParser::SPipelineCreateParams> createParamsArray;
	if (!mResourceXmlParser->parsePipelineFile(fullpath, createParamsArray))
		return false;
	
	for (u32 i = 0; i < createParamsArray.size(); i++)
	{
		loadPipeline(fullpath, createParamsArray[i]);
	}

	return true;
}

bool CResourceLoader::loadMaterialsFromFile(const std::string& name)
{
	std::string fullpath;
	if (!mResourceGroupManager->getFullPath(name, fullpath))
	{
		GF_PRINT_CONSOLE_INFO("The material file named %s doesn't exist.\n", name.c_str());
		return false;
	}

	std::vector<IResourceXmlParser::SMaterialCreateParams> createParamsArray;
	if (!mResourceXmlParser->parseMaterialFile(fullpath, createParamsArray))
		return false;

	for (u32 i = 0; i < createParamsArray.size(); i++)
	{
		loadMaterial(fullpath, createParamsArray[i]);
	}

	return true;
}

bool CResourceLoader::loadMaterial(const std::string& fullpath, 
	const IResourceXmlParser::SMaterialCreateParams& createParams) const
{
	IMaterial* material = mMaterialManager->get(createParams.Name);
	if (material)
	{
		GF_PRINT_CONSOLE_INFO("Material '%s' (in the file '%s') has already been loaded. It can't been loaded again. \
					Do you put materials with same names in material files ?\n",
					createParams.Name.c_str(), fullpath.c_str());
		return false;
	}

	IPipeline* pipelines[8];

	for (u32 i = 0; i < createParams.PipelineNames.size(); i++)
	{
		const std::string& pipelineName = createParams.PipelineNames[i];
		IPipeline* pipeline = mPipelineManager->get(pipelineName);
		if (!pipeline)
		{
			GF_PRINT_CONSOLE_INFO("The pipeline named '%s' doesn't exist in material '%s' (file location: %s).\n",
				pipelineName.c_str(), createParams.Name.c_str(), fullpath.c_str());
			return false;
		}

		pipelines[i] = pipeline;
	}

	material = mMaterialManager->create(createParams.Name, createParams.MaterialColors,
		pipelines, createParams.PipelineNames.size());

	if (!material)
	{
		GF_PRINT_CONSOLE_INFO("The material named '%s' created failed! ( file location: %s ).\n",
			createParams.Name.c_str(), fullpath.c_str());
		return false;
	}

	for (u32 i = 0; i < createParams.TextureParams.size(); i++)
	{
		ITexture* texture = mTextureManager->get(createParams.TextureParams[i].Name);
		if (!texture)
		{
			GF_PRINT_CONSOLE_INFO("Texture '%s' doesn't exist in material '%s'. (file location: %s).\n",
				createParams.TextureParams[i].Name.c_str(), createParams.Name.c_str(), fullpath.c_str());
		}
		else
		{
			material->setTexture(createParams.TextureParams[i].Index, texture);
		}
	}
	return true;
}

bool CResourceLoader::loadMeshFromFile(const std::string& name)
{
	std::string fullpath;
	if (!mResourceGroupManager->getFullPath(name, fullpath))
	{
		GF_PRINT_CONSOLE_INFO("The mesh named %s doesn't exist.\n", name.c_str());
		return false;
	}

	IModelFileParser::SModelMeshCreateParams createParams;
	if (!mModelFileParser->parseModelFile(fullpath, createParams))
		return false;

	if (mMeshManager->getMesh(name))
	{
		GF_PRINT_CONSOLE_INFO("The mesh named '%s' has already existed. \
							  Do you have two different mesh files named '%s' ?\n", 
							  name.c_str(), name.c_str());
		return false;
	}

	if (!createParams.Header.AnimatedMesh)
	{
		std::vector<IModelMesh::SModelSubset> subsets(createParams.Header.SubsetCount);
		for (u32 i = 0; i < createParams.Header.SubsetCount; i++)
		{
			subsets[i].StartIndexLocation = createParams.Subsets[i].StartIndexLocation;
			subsets[i].IndexCount = createParams.Subsets[i].IndexCount;
			subsets[i].BaseVertexLocation = createParams.Subsets[i].BaseVertexLocation;

			std::string materialName = createParams.Subsets[i].MaterialName;
			IMaterial* material = mMaterialManager->get(materialName);
			if (!material)
			{
				GF_PRINT_CONSOLE_INFO("FAIL:The material (%s) of model mesh (%s) can't be found.\n",
					materialName.c_str(), name.c_str());

				return false;
			}

			subsets[i].Material = material;
		}

		IModelMesh* mesh = mMeshManager->createModelMesh(name, createParams.VertexBuffer, createParams.IndexBuffer,
			createParams.Header.VertexCount, createParams.Header.VertexStride,
			createParams.Header.IndiceCount, subsets, createParams.Header.Bit32Indice, EMU_STATIC);

		if (!mesh)
			return false;
	}
	else
	{
		/* get subsets */
		std::vector<SAnimatedModelSubset> subsets(createParams.Header.SubsetCount);
		for (u32 i = 0; i < createParams.Header.SubsetCount; i++)
		{
			subsets[i].StartIndexLocation = createParams.Subsets[i].StartIndexLocation;
			subsets[i].IndexCount = createParams.Subsets[i].IndexCount;
			subsets[i].BaseVertexLocation = createParams.Subsets[i].BaseVertexLocation;
			subsets[i].Skinned = createParams.Subsets[i].Skinned;
			subsets[i].Bones = createParams.SubsetBones[i];
			subsets[i].BoneId = createParams.Subsets[i].BoneId;
			std::string materialName = createParams.Subsets[i].MaterialName;
			IMaterial* material = mMaterialManager->get(materialName);
			if (!material)
			{
				GF_PRINT_CONSOLE_INFO("FAIL:The material (%s) of model mesh (%s) can't be found.\n",
					materialName.c_str(), name.c_str());

				return false;
			}

			subsets[i].Material = material;
		}

		// get bone trees
		const std::vector<SModelBone>& bonesTree = createParams.Bones;

		// get animations
		const std::vector<SBoneAnimationClip>& animateClips = createParams.AnimationClips;

		IAnimatedMesh* mesh = mMeshManager->createAnimatedModelMesh(name,
			createParams.VertexBuffer,
			createParams.AnimateVertexBuffer,
			createParams.IndexBuffer,
			createParams.Header.Bit32Indice,
			createParams.Header.VertexCount,
			createParams.Header.AnimateVertexCount,
			createParams.Header.IndiceCount,
			createParams.Header.VertexStride,
			createParams.Header.AnimateVertexStride,
			EMU_STATIC,
			bonesTree,
			subsets,
			animateClips);

		if (!mesh)
			return false;
	}

	return true;
}
