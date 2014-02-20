#include "stdafx.h"
#include "CSimpleMesh.h"

void CSimpleMesh::bind()
{
	mMeshBuffer->bind();
}

void CSimpleMesh::draw() const
{
	mMeshBuffer->draw();
}
