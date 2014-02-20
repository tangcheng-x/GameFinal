#ifndef __MATERIAL_STRUCT_H__
#define __MATERIAL_STRUCT_H__

#include "ITexture.h"
#include "IPipeline.h"
#include "SReferenceCounted.h"


struct SMaterial : public SReferenceCounted
{
public:
	struct Material
	{
		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
		XMFLOAT4 Emissive;
	};

	

	SMaterial(const std::string& name, u32 sortcode, Material colors, IPipeline* pipelines[], u32 pipelineCount)
		:mName(name), mSortCode(sortcode), Colors(colors), mPipelineCount(pipelineCount)
	{
		memset(mTextures, 0, sizeof(mTextures));
		memset(mPipelinesEnabled, 0, sizeof(mPipelinesEnabled));
		memset(mPipelines, 0, sizeof(mPipelines));

		for (u32 i = 0; i < pipelineCount; i++)
		{
			mPipelines[i] = pipelines[i];
			if (pipelines[i])
			{
				pipelines[i]->grab();
				mPipelinesEnabled[i] = true;
			}
		}
	}

	
	SMaterial(const SMaterial& material)
	{
		mName = material.mName;
		mSortCode = material.mSortCode;
		mPipelineCount = material.mPipelineCount;
		memcpy(&Colors, &material.Colors, sizeof(Material));
		for (u32 i = 0; i < mPipelineCount; i++)
		{
			mPipelines[i] = material.mPipelines[i];
			mPipelinesEnabled[i] = material.mPipelinesEnabled[i];
			if (mPipelines[i])
				mPipelines[i]->grab();
		}

		for (u32 i = 0; i < MAX_TEXTURE_COUNT; i++)
		{
			mTextures[i] = material.mTextures[i];
			if (mTextures[i])
				mTextures[i]->grab();
		}
	}
	
	
	~SMaterial()
	{
		for (u32 i = 0; i < MAX_TEXTURE_COUNT; i++)
			ReleaseReferenceCounted(mTextures[i]);

		for (u32 i = 0; i < mPipelineCount; i++)
			ReleaseReferenceCounted(mPipelines[i]);
	}
	

	const std::string& getName() const
	{
		return mName;
	}

	IPipeline* getPipeline(u32 index)
	{
		if (index >= MAX_PIPELINE_COUNT)
			return nullptr;

		return mPipelines[index];
	}

	u32 getPipelineCount() const
	{
		return mPipelineCount;
	}

	bool isPipelineEnabled(u32 index) const
	{
		if (index >= MAX_PIPELINE_COUNT)
			return false;

		return mPipelinesEnabled[index];
	}

	void enablePipeline(u32 index, bool enabled = true)
	{
		if (index >= MAX_PIPELINE_COUNT)
			return;

		mPipelinesEnabled[index] = enabled;
	}

	bool setTexture(u32 layer, ITexture* texture)
	{
		if (layer >= MAX_TEXTURE_COUNT)
			return false;

		ReleaseReferenceCounted(mTextures[layer]);
		mTextures[layer] = texture;
		if (texture)
			mTextures[layer]->grab();
		
		return true;
	}

	ITexture* getTexture(u32 layer)
	{
		if (layer >= MAX_TEXTURE_COUNT)
			return nullptr;

		return mTextures[layer];
	}

	u32 getSortCode() const
	{
		return mSortCode;
	}

	const static u32 MAX_TEXTURE_COUNT;
	const static u32 MAX_PIPELINE_COUNT;


public:

	Material Colors;

private:
	std::string			mName;
	u32					mSortCode;
	IPipeline*			mPipelines[8];
	bool				mPipelinesEnabled[8];
	u32					mPipelineCount;
	ITexture*			mTextures[8];
};

typedef SMaterial IMaterial;

#endif