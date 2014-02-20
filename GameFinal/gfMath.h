#ifndef __GF_MATH_H__
#define __GF_MATH_H__

struct SAxisAlignedBox
{
	XMFLOAT3				Center;            // Center of the box.
	XMFLOAT3				Extents;           // Distance from the center to each side.
};


class Math
{
public:
	static XMMATRIX InverseTranspose(CXMMATRIX M);
};

#endif
