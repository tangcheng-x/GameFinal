#ifndef __CLIGHTNODE_H_INCLUDE__
#define __CLIGHTNODE_H_INCLUDE__

#include "ILightNode.h"

class CLightNode : public ILightNode
{
public:
	CLightNode(u32 id, ISceneNode* parent, ISceneManager* smgr,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0))
		:ILightNode(parent, smgr, position)
		, m_id(id)
	{
		
	}

	virtual void setId(u32 id)
	{
		m_id = id;
	}

	virtual u32 getId() const
	{
		return m_id;
	}

	virtual void setType(E_LIGHT_TYPE type)
	{
		m_type = type;
	}

	virtual E_LIGHT_TYPE getType() const
	{
		return m_type;
	}

	virtual void setLightData(const SLightData& lightData)
	{
		m_lightData = lightData;
	}

	virtual const SLightData& getLightData() const
	{
		return m_lightData;
	}

	virtual void render();

	virtual void OnRegisterSceneNode();

	virtual void setDiffuse(const XMFLOAT4& diffuse)
	{
		m_lightData.Diffuse = diffuse;
	}

	virtual XMFLOAT4 getDiffuse() const 
	{
		return m_lightData.Diffuse;
	}

	virtual void setSpecular(const XMFLOAT4& specular)
	{
		m_lightData.Specular = specular;
	}

	virtual XMFLOAT4 getSpecular() const
	{
		return m_lightData.Specular;
	}

	virtual void setAmbient(const XMFLOAT4& ambient)
	{
		m_lightData.Ambient = ambient;
	}

	virtual XMFLOAT4 getAmbient() const
	{
		return m_lightData.Ambient;
	}

	virtual void setAttenuation(f32 a0, f32 a1, f32 a2)
	{
		m_lightData.Attenuations = XMFLOAT3(a0, a1, a2);
	}

	virtual XMFLOAT3 getAttenuation() const
	{
		return m_lightData.Attenuations;
	}

	virtual void setDirection(const XMFLOAT3& direction)
	{
		m_direction = direction;
	}

	virtual XMFLOAT3 getDirection() const
	{
		return m_direction;
	}

	virtual void setRange(f32 range)
	{
		m_lightData.Range = range;
	}

	virtual f32 getRange() const
	{
		return m_lightData.Range;
	}

	virtual void setFalloff(f32 falloff)
	{
		m_lightData.Falloff = falloff;
	}

	virtual f32 getFalloff() const
	{
		return m_lightData.Falloff;
	}

	virtual void setTheta(f32 theta)
	{
		m_lightData.Theta = theta;
	}

	virtual f32 getTheta() const
	{
		return m_lightData.Theta;
	}

protected:
	u32					m_id;
	E_LIGHT_TYPE		m_type;
	SLightData			m_lightData;
	XMFLOAT3			m_direction;

	/*
	XMFLOAT4			m_diffuse;
	XMFLOAT4			m_specular;
	XMFLOAT4			m_ambient;
	XMFLOAT3			m_direction;
	f32					m_range;
	XMFLOAT3			m_attenuation;
	*/
};

#endif
