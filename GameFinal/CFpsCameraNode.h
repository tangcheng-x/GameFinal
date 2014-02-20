#ifndef __FPS_CAMERA_NODE_CLASS_H__
#define __FPS_CAMERA_NODE_CLASS_H__

#include "IFpsCameraNode.h"

class CFpsCameraNode : public IFpsCameraNode
{
public:
	CFpsCameraNode(ISceneNode* parent, ISceneManager* smgr,
		const XMFLOAT3& position,
		const XMFLOAT3& lookat,
		const XMFLOAT3& up,
		f32 aspectRatio,
		f32 fov,
		f32 nearZ,
		f32 farZ,
		f32 maxUpAngle,
		f32 maxDownAngle)
		:IFpsCameraNode(parent, smgr, position, aspectRatio, fov, nearZ, farZ, maxUpAngle, maxDownAngle)
	{
		XMVECTOR worldup = XMLoadFloat3(&up);
		worldup = XMVector3Normalize(worldup);

		XMVECTOR eye = XMLoadFloat3(&position);
		XMVECTOR target = XMLoadFloat3(&lookat);
		XMVECTOR look = XMVector3Normalize(target - eye);

		XMVECTOR right = XMVector3Cross(worldup, look);
		right = XMVector3Normalize(right);

		XMVECTOR dir = XMVector3Cross(right, worldup);
		dir = XMVector3Normalize(dir);

		XMVECTOR angle = XMVector3AngleBetweenNormals(dir, look);

		/* 判断当前是抬头还是低头，如果是抬头，则angle角为负，否则为正 
		   这里根据look向量与worldup向量的点积判断，如果点积为正，则为抬头，
		   如果点积为负，则为低头 */

		mPitchAngle = XMVectorGetX(angle);
		if (XMVectorGetX(XMVector3Dot(look, worldup)) > 0)
			mPitchAngle = -mPitchAngle;
		
		adjustPitchAngle();

		XMStoreFloat3(&mWorldUp, worldup);
		XMStoreFloat3(&mWalkDir, dir);
	}

	virtual void render()
	{

	}

	virtual void walk(f32 unit)
	{
		mPosition.x += mWalkDir.x * unit;
		mPosition.y += mWalkDir.y * unit;
		mPosition.z += mWalkDir.z * unit;
	}

	virtual void fly(f32 unit)
	{
		mPosition.x += mWorldUp.x * unit;
		mPosition.y += mWorldUp.y * unit;
		mPosition.z += mWorldUp.z * unit;
	}

	virtual void strafe(f32 unit)
	{
		XMVECTOR worldup = XMLoadFloat3(&mWorldUp);
		XMVECTOR dir = XMLoadFloat3(&mWalkDir);
		XMVECTOR right = XMVector3Cross(worldup, dir);

		XMVECTOR pos = XMLoadFloat3(&mPosition);
		pos += right * unit;

		XMStoreFloat3(&mPosition, pos);
	}

	/* rotate along camera's right vector. */
	virtual void pitch(f32 angle)
	{
		mPitchAngle += angle;
		adjustPitchAngle();
	}

	/* rotate along camera's up vector. */
	virtual void yaw(f32 angle)
	{
		XMVECTOR up = XMLoadFloat3(&mWorldUp);
		XMVECTOR dir = XMLoadFloat3(&mWalkDir);

		/* construct the rotate matrix. */
		XMMATRIX matRot = XMMatrixRotationNormal(up, angle);

		dir = XMVector3TransformNormal(dir, matRot);
		dir = XMVector3Normalize(dir);

		XMStoreFloat3(&mWalkDir, dir);
	}

	/* rotate along camera's look vector. */
	virtual void roll(f32 angle)
	{
		/* the roll of fps camera is meaningless. */
	}

	virtual void setOrientation(CXMMATRIX M)
	{
		
	}

	virtual XMMATRIX getRelativeTransformation() const
	{
		return XMLoadFloat4x4(&mRelativeTransform);
	}

	virtual XMMATRIX getAbsoluteTransformation()
	{
		return XMLoadFloat4x4(&mAbsoluteTransformation);
	}

	virtual void updateAbsoluteTransformation()
	{
		XMMATRIX absoluteMatrix;

		XMVECTOR worldup = XMLoadFloat3(&mWorldUp);
		XMVECTOR dir = XMLoadFloat3(&mWalkDir);
		//dir = XMVector3Normalize(dir);

		XMVECTOR right = XMVector3Cross(worldup, dir);
		right = XMVector3Normalize(right);

		/* rotate along the right vector, to get the look vector. */
		XMMATRIX rotMat = XMMatrixRotationNormal(right, mPitchAngle);
		XMVECTOR look = XMVector3TransformNormal(dir, rotMat);
		look = XMVector3Normalize(look);

		XMVECTOR up = XMVector3Cross(look, right);
		up = XMVector3Normalize(up);
			
		XMStoreFloat3(&mUp, up);
		XMStoreFloat3(&mLook, look);
		XMStoreFloat3(&mRight, right);

		XMMATRIX R = XMMatrixSet(
			mRight.x, mRight.y, mRight.z, 0.0f,
			mUp.x, mUp.y, mUp.z, 0.0f,
			mLook.x, mLook.y, mLook.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		XMMATRIX T = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
		XMMATRIX relativeMatrix = R * T;

		XMStoreFloat4x4(&mRelativeTransform, relativeMatrix);

		if (mParent)
		{
			absoluteMatrix = XMMatrixMultiply(mParent->getAbsoluteTransformation(), relativeMatrix);
		}
		else {
			absoluteMatrix = relativeMatrix;
		}

		XMStoreFloat4x4(&mAbsoluteTransformation, absoluteMatrix);

		// adjust the dir, to prevent the accumulated error.
		dir = XMVector3Normalize(XMVector3Cross(right, worldup));
		XMStoreFloat3(&mWalkDir, dir);

		/* update the view-matrix, proj-matrix, view-proj-matrix */
		XMMATRIX view;
		XMVECTOR pos = XMLoadFloat3(&mPosition);

		view._11 = mRight.x;
		view._21 = mRight.y;
		view._31 = mRight.z;
		view._41 = -XMVectorGetX(XMVector3Dot(right, pos));

		view._12 = mUp.x;
		view._22 = mUp.y;
		view._32 = mUp.z;
		view._42 = -XMVectorGetX(XMVector3Dot(up, pos));

		view._13 = mLook.x;
		view._23 = mLook.y;
		view._33 = mLook.z;
		view._43 = -XMVectorGetX(XMVector3Dot(look, pos));

		view._14 = 0;
		view._24 = 0;
		view._34 = 0;
		view._44 = 1.0f;

		XMMATRIX proj = calcProjMatrix();

		XMStoreFloat4x4(&mViewMatrix, view);
		XMStoreFloat4x4(&mProjMatrix, proj);
		XMStoreFloat4x4(&mViewProjMatrix, view * proj);
	}

private:

	void adjustPitchAngle()
	{
		/* the pitch angle must between -maxUpAngle and maxDownAngle*/

		if (mPitchAngle > mMaxDownAngle)
			mPitchAngle = mMaxDownAngle;
		if (mPitchAngle < -mMaxUpAngle)
			mPitchAngle = -mMaxUpAngle;
	}

	XMFLOAT3					mWorldUp;				/* 世界的上向量 */
	XMFLOAT3					mWalkDir;				/* 人物的行走方向 */
	f32							mPitchAngle;			/* 俯仰角度，即视线偏离行走方向的角度 */

	XMFLOAT3					mRight;
	XMFLOAT3					mUp;
	XMFLOAT3					mLook;
	XMFLOAT4X4					mRelativeTransform;		/* 摄像机相对的变换，用于 getRelativeTransformation 函数的返回值. */
};

#endif
