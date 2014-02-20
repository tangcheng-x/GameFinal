#include "stdafx.h"
#include "CLightNode.h"

void CLightNode::render()
{

}

void CLightNode::OnRegisterSceneNode()
{
	if (mVisible)
	{
		m_lightData.Position = getAbsolutePosition();
		XMVECTOR lightdir = XMVectorSet(m_direction.x, m_direction.y, m_direction.z, 0.0f);

		if (m_type != ELT_POINT)
		{
			XMMATRIX M = getAbsoluteTransformation();
			lightdir = XMVector3TransformNormal(lightdir, M);
			lightdir = XMVector3Normalize(lightdir);

			XMStoreFloat3(&m_lightData.Direction, lightdir);
		}

		//mSceneManager->registerNodeForRendering(this, ENT_LIGHT_NODE);

		ISceneNode::OnRegisterSceneNode();
	}
}
