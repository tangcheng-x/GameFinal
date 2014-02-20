// GameFinal.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "GameFinal.h"
#include "CWin32Device.h"

// 这是导出变量的一个示例
GAMEFINAL_API int nGameFinal=0;

// 这是导出函数的一个示例。
GAMEFINAL_API int fnGameFinal(void)
{
	return 42;
}

GAMEFINAL_API IDevice* createDevice(E_DRIVER_TYPE driverType, u32 width, u32 height, u32 style, bool vsync, const SDeviceContextSettings& settings)
{
	SCreationParameters creationParams;
	creationParams.ClientWidth = width;
	creationParams.ClientHeight = height;
	creationParams.DriverType = driverType;
	creationParams.DepthBits = settings.DepthBits;
	creationParams.StencilBits = settings.StencilBits;
	creationParams.MultiSamplingCount = settings.MultiSamplingCount;
	creationParams.MultiSamplingQuality = settings.MultiSamplingQuality;

	CWin32Device* device = new CWin32Device(creationParams);
	HRESULT hr = device->init();
	if (FAILED(hr))
		return NULL;
	return device;
}