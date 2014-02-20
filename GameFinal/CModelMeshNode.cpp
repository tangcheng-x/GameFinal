#include "stdafx.h"
#include "CModelMeshNode.h"
#include "ISceneManager.h"
#include "CShaderVariableInjection.h"

bool CModelMeshNode::setMaterial(IMaterial* material, u32 subset)
{
	if (subset >= mMesh->getSubsetCount())
		return false;

	mMaterials[subset] = material;
	return true;
}

IMaterial* CModelMeshNode::getMaterial(u32 subset)
{
	if (subset >= mMesh->getSubsetCount())
		return nullptr;

	return mMaterials[subset];
}

bool CModelMeshNode::setMaterialName(const std::string& name, u32 subset)
{
	if (subset >= mMesh->getSubsetCount())
		return false;

	IMaterial* material = mSceneManager->getVideoDriver()->getMaterialManager()->get(name);
	if (material == nullptr)
		return false;

	mMaterials[subset] = material;
	return true;
}

void CModelMeshNode::render()
{
	mMesh->bind();

	IPipeline* prePipeline = nullptr; /* 记录前一个流水线 */
	IMaterial* preMaterial = nullptr; /* 记录前一个子集的材质 */

	u32 subsetCount = mMesh->getSubsetCount();

	for (u32 i = 0; i < subsetCount; i++)
	{
		IMaterial* material = mMaterials[i];
		if (!material)
			continue;

		
		for (u32 j = 0; j < material->getPipelineCount(); j++)
		{
			IPipeline* pipeline = material->getPipeline(j);
			if (!pipeline)
				continue;

			//如果当前的流水线和上一条相同，则除了材质外的其他变量不用注入
			if (pipeline == prePipeline)
			{
				//当且仅当材质也不同于上一个时才注入
				if (preMaterial != material)
				{
					f32 t = timeGetTime();
					CShaderVariableInjection::injectMaterial(material, pipeline);
					pipeline->apply();	

					//fprintf(fp, "%d ", timeGetTime() - t);
				}
			}
			else // 如果两条流水线不同，全部变量都需要更新
			{
				CShaderVariableInjection::inject(this, pipeline, i);
				pipeline->apply();
				prePipeline = pipeline;
			}
			mMesh->drawSubset(i);
		}
		
		preMaterial = material;
	}
	
}

void CModelMeshNode::OnRegisterSceneNode()
{
	if (mVisible)
	{
		calcSortCode();
		mSceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}
}

void CModelMeshNode::calcSortCode()
{
	/* mesh - 8 bit
	pipeline - 48 bit
	material-texture - 8 bit
	*/
	mSortCode = ((u64)mMesh->getSortCode() << 56);

	IMaterial* material = mMaterials[0];
	if (material)
	{
		IPipeline* pipeline = material->getPipeline(0);
		if (pipeline)
		{
			mSortCode |= (pipeline->getSortCode() << 8);
		}

		ITexture* texture = material->getTexture(0);
		if (texture)
		{
			mSortCode |= texture->getSortCode();
		}
	}
}
