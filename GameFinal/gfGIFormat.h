#ifndef __E_GI_FORMAT_H_INCLUDE__
#define __E_GI_FORMAT_H_INCLUDE__

#include "gfTypes.h"

enum E_GI_FORMAT
{
	EGF_UNKNOWN = 0,
	EGF_R32G32B32A32_FLOAT,
	EGF_R32G32B32_FLOAT,
	EGF_R32G32_FLOAT,
	EGF_R8_UINT,
	EGF_R8G8_UINT,
	EGF_R8G8B8_UINT,
	EGF_R8G8B8A8_UINT
};

inline u32 getFormatOffset(E_GI_FORMAT format)
{
	switch (format)
	{
	case EGF_R32G32B32A32_FLOAT:		return 16;
	case EGF_R32G32B32_FLOAT:			return 12;
	case EGF_R32G32_FLOAT:				return 8;
	case EGF_R8_UINT:					return 1;
	case EGF_R8G8_UINT:					return 2;
	case EGF_R8G8B8_UINT:				return 3;
	case EGF_R8G8B8A8_UINT:				return 4;
	}

	return 0;
}

#endif

