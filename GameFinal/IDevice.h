#ifndef IDEVICE_H
#define IDEVICE_H

#include "gfTypes.h"
#include "gfUtil.h"
#include "ITimer.h"

class IVideoDriver;
class ISceneManager;

enum E_WINDOW_STYLE
{
	EWS_NONE = 0,
	EWS_FULLSCREEN = 1 << 0,
	EWS_FULLRESOLUTION = 1 << 1
};

enum E_DRIVER_TYPE
{
	EDT_DIRECT3D11,
	EDT_DIRECT3D10,
	EDT_EIRECT3D9,
	EDT_OPENGL
};

// This is used as the parameters of the createDevice function by the user 
struct SDeviceContextSettings
{
	u32				DepthBits;
	u32				StencilBits;
	u32				MultiSamplingCount;
	u32				MultiSamplingQuality;

	SDeviceContextSettings()
	{
		DepthBits = 24;
		StencilBits = 8;
		MultiSamplingCount = 1;
		MultiSamplingQuality = 0;
	}
};

struct SCreationParameters
{
	E_DRIVER_TYPE	DriverType;
	u32				ClientWidth;
	u32				ClientHeight;
	E_WINDOW_STYLE  WindowStyle;
	bool			VsyncEnabled;
	u32				WindowHandle;

	/* fields below are from SDeviceContextSettings*/
	u32				DepthBits;
	u32				StencilBits;
	u32				MultiSamplingCount;
	u32				MultiSamplingQuality;


	SCreationParameters()
	{
		DriverType = EDT_DIRECT3D11;
		ClientHeight = 0;
		ClientWidth = 0;
		WindowStyle = EWS_NONE;
		VsyncEnabled = false;
		WindowHandle = 0;

		DepthBits = 24;
		StencilBits = 8;
		MultiSamplingCount = 1;
		MultiSamplingQuality = 0;
	}
};



class IDevice : public IReferenceCounted
{
public:
	IDevice(SCreationParameters& params) :m_CreationParams(params){}
		
	virtual bool run() = 0; 

	const SCreationParameters& getCreationParameters() const
	{
		return m_CreationParams;
	}

	IVideoDriver*	getVideoDriver() 
	{
		return mVideoDriver;
	}

	ISceneManager*	getSceneManager()
	{
		return mSceneManager;
	}

	u32 getClientWidth() const
	{
		return m_CreationParams.ClientWidth;
	}

	u32 getClientHeight() const
	{
		return m_CreationParams.ClientHeight;
	}

	virtual void setWindowCaption(const char* caption) = 0;

	virtual ITimer* createTimer() const = 0;

	virtual ~IDevice()
	{
		
	}

protected:
	SCreationParameters		m_CreationParams;
	IVideoDriver*			mVideoDriver;
	ISceneManager*			mSceneManager;
};


#endif