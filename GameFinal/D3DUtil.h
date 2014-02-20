#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
	HRESULT hr = (x);                                      \
if (FAILED(hr))                                         \
		{                                                      \
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#ifndef ReleaseCOM 
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#endif

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------

#ifndef SafeDelete
#define SafeDelete(x) { delete x; x = 0; }
#endif

//---------------------------------------------------------------------------------------
// Utility classes.
//---------------------------------------------------------------------------------------

#ifndef __D3DUTIL_FUNCTION_H__
#define __D3DUTIL_FUNCTION_H__

#include "gfGIFormat.h"
#include "EMemoryUsage.h"

DXGI_FORMAT getDxgiFormat(E_GI_FORMAT format);

D3D11_USAGE getD3d11Usage(E_MEMORY_USAGE usage);

#endif
