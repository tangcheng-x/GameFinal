#ifndef __SHADER_VARIABLE_INJECTION_CLASS_H__
#define __SHADER_VARIABLE_INJECTION_CLASS_H__

#include "IShaderVariableInjection.h"
#include "ISceneManager.h"
#include "ILightNode.h"

class CShaderVariableInjection
{
public:
	
	CShaderVariableInjection(){}

	static void inject(IMeshNode* mesh, IPipeline* pipeline, u32 subset = 0);

	static void injectBoneTransforms(IPipeline* pipeline, const f32* matrixs, u32 count);

	static void injectMaterial(IMaterial* material, IPipeline* pipeline);

	/* inject just one variable*/
	static void injectVariable(const SShaderAutoVariable& var, IMeshNode* mesh, IPipeline* pipeline, u32 subset);

	/* inject world, view, proj transforms. */
	static void injectTransformations(const SShaderAutoVariable& var, ISceneNode* node, IPipeline* pipeline);

	/* inject materials. */
	static void injectMaterial(const SShaderAutoVariable& var, IPipeline* pipeline, IMaterial* material);

	/* inject scene informations */
	static void injectSceneInfo(const SShaderAutoVariable& var, ISceneNode* node, IPipeline* pipeline);


private:

};

#endif
