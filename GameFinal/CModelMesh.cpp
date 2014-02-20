#include "stdafx.h"
#include "CModelMesh.h"

void CModelMesh::drawSubset(u32 index) const
{
	if (index >= mSubsets.size())
		return;

	const SModelSubset& subset = mSubsets[index];
	mMeshBuffer->drawIndexed(subset.StartIndexLocation, subset.IndexCount, subset.BaseVertexLocation);
}

void CModelMesh::bind()
{
	mMeshBuffer->bind();
}

