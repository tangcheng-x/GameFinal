#include "stdafx.h"
#include "CShaderVariableInjection.h"

void CShaderVariableInjection::inject(IMeshNode* mesh, IPipeline* pipeline, u32 subset)
{
	const std::vector<SShaderAutoVariable>& shaderVariables = pipeline->getShaderAutoVariables();
	u32 shaderVariableCount = shaderVariables.size();

	for (u32 i = 0; i < shaderVariableCount; i++)
	{
		injectVariable(shaderVariables[i], mesh, pipeline, subset);
	}
}

void CShaderVariableInjection::injectBoneTransforms(IPipeline* pipeline, const f32* matrixs, u32 count)
{
	const std::vector<SShaderAutoVariable>& shaderVariables = pipeline->getShaderAutoVariables();
	u32 shaderVariableCount = shaderVariables.size();

	for (u32 i = 0; i < shaderVariableCount; i++)
	{
		if (shaderVariables[i].Type == ESAVT_BONE_TRANSFORMS)
		{
			pipeline->setTransposedMatrixArray(shaderVariables[i].ShaderType,
				shaderVariables[i].VariableName,
				matrixs, count, shaderVariables[i].UpdateFrequency == EUF_PER_FRAME);
		}
	}
}

void CShaderVariableInjection::injectVariable(
	const SShaderAutoVariable& var, 
	IMeshNode* mesh, 
	IPipeline* pipeline, 
	u32 subset)
{
	E_SHADER_AUTO_VARIABLE_TYPE type = var.Type;
	if (type >= ESAVT_TRANSFORMS_BEGIN && type <= ESAVT_TRANSFORMS_END)
	{
		injectTransformations(var, mesh, pipeline);
	}
	else if (type >= ESAVT_MATERIAL_BEGIN && type <= ESAVT_MATERIAL_END)
	{
		injectMaterial(var, pipeline, mesh->getMaterial(subset));
	}
	else if (type >= ESAVT_SCENE_BEGIN && type <= ESAVT_SCENE_END)
	{
		injectSceneInfo(var, mesh, pipeline);
	}
}

void CShaderVariableInjection::injectMaterial(IMaterial* material, IPipeline* pipeline)
{
	const std::vector<SShaderAutoVariable>& shaderVariables = pipeline->getShaderAutoVariables();
	u32 shaderVariableCount = shaderVariables.size();

	for (u32 i = 0; i < shaderVariableCount; i++)
	{
		const SShaderAutoVariable& var = shaderVariables[i];
		if (var.Type >= ESAVT_MATERIAL_BEGIN && var.Type <= ESAVT_MATERIAL_END)
		{
			injectMaterial(var, pipeline, material);
		}
	}
}


void CShaderVariableInjection::injectTransformations(const SShaderAutoVariable& var, ISceneNode* node, IPipeline* pipeline)
{
	XMMATRIX M;
	ICameraNode* camera = nullptr;

	ISceneManager* sceneManager = node->getSceneManager();

	/* 是否需要设置该变量 */
	bool flag = false;

	switch (var.Type)
	{
	case ESAVT_WORLD_MATRIX:
		M = node->getAbsoluteTransformation();
		flag = true;
		break;
	case ESAVT_INVERSE_WORLD_MATRIX:
	{
		M = node->getAbsoluteTransformation();
		XMVECTOR det = XMMatrixDeterminant(M);
		M = XMMatrixInverse(&det, M);
		flag = true;
	}
		break;
	case ESAVT_TRANSPOSE_WORLD_MATRIX:
		M = node->getAbsoluteTransformation();
		M = XMMatrixTranspose(M);
		flag = true;
		break;
	case ESAVT_INVERSE_TRANSPOSE_WORLD_MATRIX: // (A-1)T
		M = node->getAbsoluteTransformation();
		M = Math::InverseTranspose(M);
		flag = true;
		break;
	case ESAVT_VIEW_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewMatrix());
			flag = true;
		}
		break;
	case ESAVT_INVERSE_VIEW_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewMatrix());
			XMVECTOR det = XMMatrixDeterminant(M);
			M = XMMatrixInverse(&det, M);
			flag = true;
		}
		break;
	case ESAVT_TRANSPOSE_VIEW_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewMatrix());
			M = XMMatrixTranspose(M);
			flag = true;
		}
		break;
	case ESAVT_INVERSE_TRANSPOSE_VIEW_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewMatrix());
			M = Math::InverseTranspose(M);
			flag = true;
		}
		break;
	case ESAVT_PROJ_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getProjMatrix());
			flag = true;
		}
		break;
	case ESAVT_INVERSE_PROJ_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getProjMatrix());
			XMVECTOR det = XMMatrixDeterminant(M);
			M = XMMatrixInverse(&det, M);
			flag = true;
		}
		break;
	case ESAVT_TRANSPOSE_PROJ_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getProjMatrix());
			M = XMMatrixTranspose(M);
			flag = true;
		}
		break;
	case ESAVT_INVERSE_TRANSPOSE_PROJ_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getProjMatrix());
			M = Math::InverseTranspose(M);
			flag = true;
		}
		break;
	case ESAVT_VIEW_PROJ_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewProjMatrix());
			flag = true;
		}
		break;
	case ESAVT_INVERSE_VIEW_PROJ_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewProjMatrix());
			XMVECTOR det = XMMatrixDeterminant(M);
			M = XMMatrixInverse(&det, M);
			flag = true;
		}
		break;
	case ESAVT_TRANSPOSE_VIEW_PROJ_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewProjMatrix());
			M = XMMatrixTranspose(M);
			flag = true;
		}
		break;
	case ESAVT_INVERSE_TRANSPOSE_VIEW_PROJ_MATRIX:
		camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewProjMatrix());
			M = Math::InverseTranspose(M);
			flag = true;
		}
		break;
	}

	if (flag)
	{
		E_SHADER_TYPE shaderType = var.ShaderType;
		bool ignoreIfAlreadyUpdate = (var.UpdateFrequency == EUF_PER_FRAME);
		pipeline->setMatrix(shaderType, var.VariableName, M, ignoreIfAlreadyUpdate);
	}
	
}

void CShaderVariableInjection::injectMaterial(const SShaderAutoVariable& var, IPipeline* pipeline, IMaterial* material)
{
	if (!material)
		return;

	E_SHADER_TYPE shaderType = var.ShaderType;
	bool ignoreIfAlreadyUpdate = (var.UpdateFrequency == EUF_PER_FRAME);

	/* 如果是注入整个Material结构体 */
	if (var.Type == ESAVT_MATERIAL)
	{
		pipeline->setRawValue(shaderType, var.VariableName, &material->Colors, sizeof(material->Colors), ignoreIfAlreadyUpdate);
		return;
	}


	/* 如果是注入Material中的某个颜色分量 (Ambient, Diffuse, Specular, Emissive) */
	XMFLOAT4 materialComponent;
	bool flag = false;
	switch (var.Type)
	{
	case ESAVT_MATERIAL_AMBIENT:
		materialComponent = material->Colors.Ambient;
		flag = true;
		break;
	case ESAVT_MATERIAL_DIFFUSE:
		materialComponent = material->Colors.Diffuse;
		flag = true;
		break;
	case ESAVT_MATERIAL_SPECULAR:
		materialComponent = material->Colors.Specular;
		flag = true;
		break;
	case ESAVT_MATERIAL_EMISSIVE:
		materialComponent = material->Colors.Emissive;
		flag = true;
		break;
	}

	if (flag)
	{
		pipeline->setVector(shaderType, var.VariableName, reinterpret_cast<const f32*>(&materialComponent), ignoreIfAlreadyUpdate);
	}

	/* 如果是需要注入材质 */
	if (var.Type == ESAVT_TEXTURE)
	{
		ITexture* texture = material->getTexture(var.IndexParam);
		if (texture != nullptr)
		{
			pipeline->setTexture(shaderType, var.VariableName, texture);
		}
	}
}


void CShaderVariableInjection::injectSceneInfo(const SShaderAutoVariable& var, ISceneNode* node, IPipeline* pipeline)
{
	ISceneManager* sceneManager = node->getSceneManager();
	E_SHADER_TYPE shaderType = var.ShaderType;
	bool ignoreIfAlreadyUpdate = (var.UpdateFrequency == EUF_PER_FRAME);

	/* inject camera position. */
	if (var.Type == ESAVT_CAMERA_POSITION)
	{
		ICameraNode* camera = sceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			XMFLOAT3 pos = camera->getAbsolutePosition();
			XMFLOAT4 p = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
			pipeline->setVector(shaderType, var.VariableName, reinterpret_cast<f32*>(&p), ignoreIfAlreadyUpdate);
		}
		return;
	}

	/* 注入整个光照结构体 */
	bool flag = false;
	ILightNode* light = nullptr;
	switch (var.Type)
	{
	case ESAVT_LIGHT:
		light = sceneManager->getLightNode(var.IndexParam);
		flag = true;
		break;
	}

	if (flag && light)
	{
		const SLightData& lightData = light->getLightData();
		pipeline->setRawValue(shaderType, var.VariableName, (void*)&lightData, sizeof(SLightData), ignoreIfAlreadyUpdate);
	}
}
