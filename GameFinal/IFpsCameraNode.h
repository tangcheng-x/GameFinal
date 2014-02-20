#ifndef __FPS_CAMERA_NODE_INTERFACE_H__
#define __FPS_CAMERA_NODE_INTERFACE_H__

#include "ICameraNode.h"

class IFpsCameraNode : public ICameraNode
{
public:
	IFpsCameraNode(ISceneNode* parent, ISceneManager* smgr,
		const XMFLOAT3& position,
		f32 aspectRatio,
		f32 fov,
		f32 nearZ,
		f32 farZ,
		f32 maxUpAngle,
		f32 maxDownAngle)
		:ICameraNode(parent, smgr, position, aspectRatio, fov, nearZ, farZ)
		, mMaxUpAngle(maxUpAngle)
		, mMaxDownAngle(maxDownAngle)
	{

	}

	
	
protected:
	f32								mMaxUpAngle;			/* 最大抬头角度 */
	f32								mMaxDownAngle;			/* 最大低头角度 */
};

#endif
