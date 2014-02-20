#include "stdafx.h"
#include "gfMath.h"

XMMATRIX Math::InverseTranspose(CXMMATRIX M)
{
	XMMATRIX A = M;
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));
}
