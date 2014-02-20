#include "stdafx.h"
#include "CAnimatedMeshNode.h"
#include "CShaderVariableInjection.h"
#include "ISceneManager.h"

bool CAnimatedMeshNode::setMaterial(IMaterial* material, u32 subset)
{
	if (subset >= mMesh->getSubsetCount())
		return false;

	mMaterials[subset] = material;
	return true;
}

IMaterial* CAnimatedMeshNode::getMaterial(u32 subset)
{
	if (subset >= mMesh->getSubsetCount())
		return nullptr;

	return mMaterials[subset];
}

bool CAnimatedMeshNode::setMaterialName(const std::string& name, u32 subset)
{
	if (subset >= mMesh->getSubsetCount())
		return false;

	IMaterial* material = mSceneManager->getVideoDriver()->getMaterialManager()->get(name);
	if (material == nullptr)
		return false;

	mMaterials[subset] = material;
	return true;
}

void CAnimatedMeshNode::updateSubsetBones(const std::vector<SModelSubsetBone>& bones)
{
	for (u32 i = 0; i < bones.size(); i++)
	{
		u32 boneIndex = bones[i].BoneIndex;
		XMMATRIX toRootMatrix = XMLoadFloat4x4(&mAbsoluteBoneTransforms[boneIndex]);
		XMMATRIX offsetMatrix = XMLoadFloat4x4(&bones[i].OffsetTransform);

		XMMATRIX combined = offsetMatrix * toRootMatrix;
		combined = XMMatrixTranspose(combined);

		XMStoreFloat4x4(&mSubsetBoneTransforms[i], combined);
	}
}

void CAnimatedMeshNode::render()
{
	XMFLOAT4X4 preAbsoluteTransform;

	mMesh->bind();

	IPipeline* prePipeline = nullptr; /* 记录前一个流水线 */
	IMaterial* preMaterial = nullptr; /* 记录前一个子集的材质 */

	u32 subsetCount = mMesh->getSubsetCount();

	updateAbsoluteBoneTransforms();
	const std::vector<SAnimatedModelSubset>& subsets = mMesh->getSubsets();

	for (u32 i = 0; i < subsetCount; i++)
	{
		IMaterial* material = mMaterials[i];
		if (!material)
			continue;

		/* transform subset bones. */
		if (subsets[i].Skinned)
		{
			updateSubsetBones(subsets[i].Bones);
		}
		else
		{
			// 如果该子集无动画，则需要改变世界变换
			u32 boneId = subsets[i].BoneId;
			if (boneId >= 0)
			{
				preAbsoluteTransform = mAbsoluteTransformation;
				XMFLOAT4X4 boneTransform = mAbsoluteBoneTransforms[boneId];
				XMMATRIX absoluteMatrix = XMLoadFloat4x4(&mAbsoluteTransformation);
				XMMATRIX boneMatrix = XMLoadFloat4x4(&boneTransform);
				XMStoreFloat4x4(&mAbsoluteTransformation, boneMatrix * absoluteMatrix);
			}
		}

		for (u32 j = 0; j < material->getPipelineCount(); j++)
		{
			IPipeline* pipeline = material->getPipeline(j);
			if (!pipeline)
				continue;

		//	IPipeline* pipeline2 = subsets[0].Material->getPipeline(0);
			//pipeline = subsets[subsets.size() - 1].Material->getPipeline(0);

			// 如果该子集有动画，骨骼变换都是需要注入的
			if (subsets[i].Skinned)
			{
				CShaderVariableInjection::injectBoneTransforms(pipeline,
					reinterpret_cast<f32*>(&mSubsetBoneTransforms[0]),
					subsets[i].Bones.size());
			}
		
			CShaderVariableInjection::inject(this, pipeline, i);
			pipeline->apply();
			/*

			//如果当前的流水线和上一条相同，则除了材质外的其他变量不用注入
			if (pipeline == prePipeline)
			{
				//当且仅当材质也不同于上一个时才注入
				if (preMaterial != material)
				{
					CShaderVariableInjection::injectMaterial(material, pipeline);
					pipeline->apply();
				}
			}
			else // 如果两条流水线不同，全部变量都需要更新
			{
				CShaderVariableInjection::inject(this, pipeline, i);
				pipeline->apply();
				prePipeline = pipeline;
			}
			*/

			mMesh->drawSubset(i);
			
		}

		// 如果世界变换经过了改变，则需要回复 
		if (!subsets[i].Skinned && subsets[i].BoneId >= 0)
		{
			mAbsoluteTransformation = preAbsoluteTransform;
		}

		//preMaterial = material;
	}

}

void CAnimatedMeshNode::OnRegisterSceneNode()
{
	if (mVisible)
	{
		calcSortCode();
		mSceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}
}

void CAnimatedMeshNode::calcSortCode()
{
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

bool CAnimatedMeshNode::setAnimation(u32 id)
{
	if (id >= mMesh->getAnimationCount())
		return false;

	mCurrAnimationId = id;
	return true;
}

bool CAnimatedMeshNode::setAnimation(const std::string& name)
{
	s32 id = mMesh->getAnimationId(name);
	if (id == -1)
		return false;

	mCurrAnimationId = id;
}

void CAnimatedMeshNode::addTime(f32 t)
{
	if (mCurrAnimationId < 0)
		return;

	mTimePos += t;
	mMesh->getRelativeBoneTransforms(mCurrAnimationId, mTimePos, mRelativeBoneTransforms);
}

void CAnimatedMeshNode::updateAbsoluteBoneTransforms()
{
	if (mCurrAnimationId < 0)
		mMesh->getRelativeBoneTransforms(mAbsoluteBoneTransforms);
	else
		mMesh->getAbsoluteBoneTransforms(mRelativeBoneTransforms, mAbsoluteBoneTransforms);
}
