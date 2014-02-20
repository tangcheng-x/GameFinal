#ifndef __IRESOURCE_FACTORY_H_INCLUDE__
#define __IRESOURCE_FACTORY_H_INCLUDE__

#include "IReferenceCounted.h"
#include "ITexture.h"
#include "IRenderState.h"
#include "gfGIFormat.h"
#include "IShader.h"
#include "IPipeline.h"
#include "IInputLayout.h"
#include "IRenderState.h"
#include "ISimpleMesh.h"
#include "IModelMesh.h"
#include "IAnimatedMesh.h"
#include "IAnimatedMeshBuffer.h"

class IResourceFactory : public IReferenceCounted
{
public:
	virtual ~IResourceFactory(){}
	virtual ITexture* loadTextureFromFile(
		const std::string& name,
		const std::string& filepath, 
		u32 sortcode) = 0;

	virtual IRenderState* createRenderState(const std::string& name) = 0;

	virtual IPipeline* createPipeline(const std::string& name,
		IShader** shaders,
		u32 shaderCount,
		IInputLayout* inputlayout,
		E_PRIMITIVE_TYPE primitiveType,
		IRenderState* renderState) = 0;

	virtual IShader* createShaderFromFile(
		E_SHADER_TYPE shaderType,
		const char* szFilename,
		const char* szFunctionName,
		u32 sortCode,
		const std::string& shaderName) = 0;

	virtual IInputLayout* createInputLayout(
		u32 sortCode, 
		const std::vector<SInputLayoutElement>& elements,
		IShader* shader,
		u32 hashcode) = 0;

	virtual ISimpleMesh* createSimpleMesh(
		const std::string& name,
		u32 sortcode,
		void* vertices,
		void* indices,
		u32 vertexCount,
		u32 vertexStride,
		u32 indiceCount,
		bool bit32Index,
		E_MEMORY_USAGE usage = EMU_STATIC) = 0;

	virtual IModelMesh* createModelMesh(
		const std::string& name,
		u32 sortcode,
		void* vertices,
		void* indices,
		u32 vertexCount,
		u32 vertexStride,
		u32 indiceCount,
		bool bit32Index,
		const std::vector<IModelMesh::SModelSubset>& subsets,
		E_MEMORY_USAGE usage = EMU_STATIC) = 0;

	virtual IAnimatedMesh* createAnimatedModelMesh(
		const std::string& name,
		u32 sortcode,
		void* vertices,
		void* animateVertices,
		void* indices,
		u32 vertexCount,
		u32 animateVertexCount,
		u32 indicesCount,
		u32 vertexStride,
		u32 animateVertexStride,
		bool bit32Index,
		E_MEMORY_USAGE usage,
		const std::vector<SAnimatedModelSubset>& subsets,
		const std::vector<SModelBone>& bones,
		const std::vector<SBoneAnimationClip>& animateClips) = 0;

	virtual IAnimatedMeshBuffer* createAnimatedMeshBuffer(
		E_MEMORY_USAGE usage,
		void* vertices,
		void* animateVertices,
		void* indices,
		u32 vertexCount,
		u32 animateVertexCount,
		u32 indicesCount,
		u32 vertexStride,
		u32 animateVertexStride,
		bool bit32Index) = 0;

	virtual IMeshBuffer* createMeshBuffer(
		E_MEMORY_USAGE usage,
		void* vertices,
		void* indices,
		u32 vertexCount,
		u32 indicesCount,
		u32 vertexStride,
		bool bit32Index) = 0;
};


#endif
