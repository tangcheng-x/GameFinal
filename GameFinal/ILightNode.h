#ifndef __ILIGHTNODE_H_INCLUDE__
#define __ILIGHTNODE_H_INCLUDE__

#include "ISceneNode.h"
#include "ISceneManager.h"

enum E_LIGHT_TYPE
{
	ELT_POINT,
	ELT_DIRECTIONAL,
	ELT_SPOT
};
/*
D3DLIGHTTYPE  Type;
D3DCOLORVALUE Diffuse;
D3DCOLORVALUE Specular;
D3DCOLORVALUE Ambient;
D3DVECTOR     Position;
D3DVECTOR     Direction;
float         Range;
float         Falloff;
float         Attenuation0;
float         Attenuation1;
float         Attenuation2;
float         Theta;
float         Phi;

*/
struct SLightData
{
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Ambient;
	XMFLOAT3	Position;
	f32			Range;
	XMFLOAT3	Direction;
	f32			Falloff;
	XMFLOAT3	Attenuations;
	f32			Theta;
};

class ILightNode : public ISceneNode
{
public:
	ILightNode(ISceneNode* parent, ISceneManager* smgr,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0))
		:ISceneNode(parent, smgr, position, XMFLOAT3(0, 0, 0), XMFLOAT3(1.0f, 1.0f, 1.0f))
	{
		
	}

	virtual void setId(u32 id) = 0;

	virtual u32 getId() const = 0;

	virtual void setType(E_LIGHT_TYPE type) = 0;
	
	virtual E_LIGHT_TYPE getType() const = 0;

	virtual void setLightData(const SLightData& lightData) = 0;

	virtual const SLightData& getLightData() const = 0;

	virtual void setDiffuse(const XMFLOAT4& diffuse) = 0;

	virtual XMFLOAT4 getDiffuse() const = 0;

	virtual void setSpecular(const XMFLOAT4& specular) = 0;

	virtual XMFLOAT4 getSpecular() const = 0;

	virtual void setAmbient(const XMFLOAT4& ambient) = 0;

	virtual XMFLOAT4 getAmbient() const = 0;

	virtual void setAttenuation(f32 a0, f32 a1, f32 a2) = 0;

	virtual XMFLOAT3 getAttenuation() const = 0;

	virtual void setDirection(const XMFLOAT3& direction) = 0;

	virtual XMFLOAT3 getDirection() const = 0;

	virtual void setRange(f32 range) = 0;

	virtual f32 getRange() const = 0;

	virtual void setFalloff(f32 falloff) = 0;

	virtual f32 getFalloff() const = 0;

	virtual void setTheta(f32 theta) = 0;

	virtual f32 getTheta() const = 0;

};

#endif
