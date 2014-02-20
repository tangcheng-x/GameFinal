#include "stdafx.h"
#include "CMeshNode.h"
#include "ISceneManager.h"
#include "gfMath.h"
#include "ILightNode.h"
#include "CShaderVariableInjection.h"

void CMeshNode::render()
{
	if (!mMaterial)
		return;

	mMesh->bind();

	u32 pipelineCount = mMaterial->getPipelineCount();
	for (u32 i = 0; i < pipelineCount; i++)
	{
		if (mMaterial->isPipelineEnabled(i))
		{
			IPipeline* pipeline = mMaterial->getPipeline(i);
			
			CShaderVariableInjection::inject(this, pipeline, 0);

			pipeline->apply();

			mMesh->draw();
		}
	}
}

void CMeshNode::OnRegisterSceneNode()
{
	if (mVisible)
	{
		calcSortCode();
		mSceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}
}

bool CMeshNode::setMaterialName(const std::string& name, u32 subset)
{
	IMaterial* material = mSceneManager->getVideoDriver()->getMaterialManager()->get(name);
	if (material == nullptr)
		return false;

	ReleaseReferenceCounted(mMaterial);
	mMaterial = material;
	AddReferenceCounted(mMaterial);
	return true;
}

void CMeshNode::calcSortCode()
{
	/* mesh - 8 bit
	pipeline - 48 bit
	material - 8 bit
	*/
	mSortCode = ((u64)mMesh->getSortCode() << 56) | (mMaterial->getPipeline(0)->getSortCode() << 8);
	ITexture* pTexture = mMaterial->getTexture(0);
	if (pTexture != nullptr)
		mSortCode |= pTexture->getSortCode();
}



/*

void CMeshNode::injectShaderVariables(IPipeline* pipeline)
{
	const std::vector<SShaderAutoVariable>& shaderVariables = pipeline->getShaderAutoVariables();
	u32 shaderVariableCount = shaderVariables.size();
	
	for (u32 i = 0; i < shaderVariableCount; i++)
	{
		E_SHADER_AUTO_VARIABLE_TYPE type = shaderVariables[i].Type;
		if (type >= ESAVT_MATRIX_BEGIN && type <= ESAVT_MATRIX_END)
		{
			injectMatrixShaderAutoVariable(pipeline, shaderVariables[i]);
		}
		else if (type == ESAVT_LIGHT)
		{
			injectLightAutoVariable(pipeline, shaderVariables[i]);
		}
		else if (type == ESAVT_TEXTURE)
		{
			injectTextureAutoVariable(pipeline, shaderVariables[i]);
		}
		else if (type >= ESAVT_MATERIAL_BEGIN && type <= ESAVT_MATERIAL_END)
		{
			injectMaterialAutoVariable(pipeline, shaderVariables[i]);
		}
		else if (type >= ESAVT_VECTOR_BEGIN && type <= ESAVT_VECTOR_END)
		{
			injectVectorAutoVariable(pipeline, shaderVariables[i]);
		}
	}
}

void CMeshNode::injectMatrixShaderAutoVariable(IPipeline* pipeline, const SShaderAutoVariable& var)
{
	XMMATRIX M;
	ICameraNode* camera = nullptr;

	switch (var.Type)
	{
	case ESAVT_WORLD_MATRIX:
		M = this->getAbsoluteTransformation();
		break;
	case ESAVT_INVERSE_WORLD_MATRIX:
		{	
			M = this->getAbsoluteTransformation();
			XMVECTOR det = XMMatrixDeterminant(M);
			M = XMMatrixInverse(&det, M);
		}
		break;
	case ESAVT_TRANSPOSE_WORLD_MATRIX:
		M = this->getAbsoluteTransformation();
		M = XMMatrixTranspose(M);
		break;
	case ESAVT_INVERSE_TRANSPOSE_WORLD_MATRIX: // (A-1)T
		M = this->getAbsoluteTransformation();
		M = Math::InverseTranspose(M);
		break;
	case ESAVT_VIEW_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
			M = XMLoadFloat4x4(&camera->getViewMatrix());
		break;
	case ESAVT_INVERSE_VIEW_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewMatrix());
			XMVECTOR det = XMMatrixDeterminant(M);
			M = XMMatrixInverse(&det, M);
		}
		break;
	case ESAVT_TRANSPOSE_VIEW_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewMatrix());
			M = XMMatrixTranspose(M);
		}
		break;
	case ESAVT_INVERSE_TRANSPOSE_VIEW_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewMatrix());
			M = Math::InverseTranspose(M);
		}
		break;
	case ESAVT_PROJ_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getProjMatrix());
		}
		break;
	case ESAVT_INVERSE_PROJ_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getProjMatrix());
			XMVECTOR det = XMMatrixDeterminant(M);
			M = XMMatrixInverse(&det, M);
		}
		break;
	case ESAVT_TRANSPOSE_PROJ_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getProjMatrix());
			M = XMMatrixTranspose(M);
		}
		break;
	case ESAVT_INVERSE_TRANSPOSE_PROJ_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getProjMatrix());
			M = Math::InverseTranspose(M);
		}
		break;
	case ESAVT_VIEW_PROJ_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			//XMFLOAT4X4 viewproj = camera->getViewProjMatrix();
			M = XMLoadFloat4x4(&camera->getViewProjMatrix());
		}
		break;
	case ESAVT_INVERSE_VIEW_PROJ_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewProjMatrix());
			XMVECTOR det = XMMatrixDeterminant(M);
			M = XMMatrixInverse(&det, M);
		}
		break;
	case ESAVT_TRANSPOSE_VIEW_PROJ_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewProjMatrix());
			M = XMMatrixTranspose(M);
		}
		break;
	case ESAVT_INVERSE_TRANSPOSE_VIEW_PROJ_MATRIX:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			M = XMLoadFloat4x4(&camera->getViewProjMatrix());
			M = Math::InverseTranspose(M);
		}
		break;
	}

	E_SHADER_TYPE shaderType = var.ShaderType; 
	bool ignoreIfAlreadyUpdate = (var.UpdateFrequency == EUF_PER_FRAME);
	if (shaderType == EST_UNDEFINED_SHADER)
		pipeline->setMatrix(var.VariableName, M, ignoreIfAlreadyUpdate);
	else
		pipeline->setMatrix(shaderType, var.VariableName, M, ignoreIfAlreadyUpdate);
}

void CMeshNode::injectLightAutoVariable(IPipeline* pipeline, const SShaderAutoVariable& var)
{
	ILightNode* light = mSceneManager->getLightNode(var.IndexParam);
	if (light != nullptr)
	{
		const SLightData& lightData = light->getLightData();
		E_SHADER_TYPE shaderType = var.ShaderType;
		bool ignoreIfAlreadyUpdate = (var.UpdateFrequency == EUF_PER_FRAME);
		if (shaderType == EST_UNDEFINED_SHADER)
			pipeline->setRawValue(var.VariableName, (void*)&lightData, sizeof(SLightData), ignoreIfAlreadyUpdate);
		else
			pipeline->setRawValue(shaderType, var.VariableName, (void*)&lightData, sizeof(SLightData), ignoreIfAlreadyUpdate);
	}
}

void CMeshNode::injectTextureAutoVariable(IPipeline* pipeline, const SShaderAutoVariable& var)
{
	ITexture* texture = mMaterial->getTexture(var.IndexParam);
	if (texture != nullptr)
	{
		E_SHADER_TYPE shaderType = var.ShaderType;
		if (shaderType == EST_UNDEFINED_SHADER)
			pipeline->setTexture(var.VariableName, texture);
		else
			pipeline->setTexture(shaderType, var.VariableName, texture);
	}
}

void CMeshNode::injectMaterialAutoVariable(IPipeline* pipeline, const SShaderAutoVariable& var)
{
	E_SHADER_TYPE shaderType = var.ShaderType;
	bool ignoreIfAlreadyUpdate = (var.UpdateFrequency == EUF_PER_FRAME);

	if (var.Type == ESAVT_MATERIAL)
	{
		if (shaderType == EST_UNDEFINED_SHADER)
			pipeline->setRawValue(var.VariableName, &mMaterial->Colors, sizeof(mMaterial->Colors), ignoreIfAlreadyUpdate);
		else 
			pipeline->setRawValue(shaderType, var.VariableName, &mMaterial->Colors, sizeof(mMaterial->Colors), ignoreIfAlreadyUpdate);

		return;
	}

	XMFLOAT4 materialComponent;
	switch (var.Type)
	{
	case ESAVT_MATERIAL_AMBIENT:
		materialComponent = mMaterial->Colors.Ambient;
		break;
	case ESAVT_MATERIAL_DIFFUSE:
		materialComponent = mMaterial->Colors.Diffuse;
		break;
	case ESAVT_MATERIAL_SPECULAR:
		materialComponent = mMaterial->Colors.Specular;
		break;
	case ESAVT_MATERIAL_EMISSIVE:
		materialComponent = mMaterial->Colors.Emissive;
		break;
	}

	if (shaderType == EST_UNDEFINED_SHADER)
		pipeline->setVector(var.VariableName, reinterpret_cast<const f32*>(&materialComponent), ignoreIfAlreadyUpdate);
	else
		pipeline->setVector(shaderType, var.VariableName, reinterpret_cast<const f32*>(&materialComponent), ignoreIfAlreadyUpdate);
}

void CMeshNode::injectVectorAutoVariable(IPipeline* pipeline, const SShaderAutoVariable& var)
{
	bool ignoreIfAlreadyUpdate = (var.UpdateFrequency == EUF_PER_FRAME);
	ICameraNode* camera = nullptr;
	const f32* value = nullptr;
	switch (var.Type)
	{
	case ESAVT_CAMERA_POSITION:
		camera = mSceneManager->getCameraNode(var.IndexParam);
		if (camera)
		{
			XMFLOAT3 pos = camera->getAbsolutePosition();
			XMFLOAT4 p = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
			value = reinterpret_cast<f32*>(&p);
		}
		break;
	}

	pipeline->setVector(var.ShaderType, var.VariableName, value, ignoreIfAlreadyUpdate);
}
*/

