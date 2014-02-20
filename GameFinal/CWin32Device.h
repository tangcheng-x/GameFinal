#ifndef D3D11DEIVCE_H
#define D3D11DEIVCE_H

#include "IDevice.h"
#include "IVideoDriver.h"

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class CWin32Device : public IDevice 
{
public:
	CWin32Device(SCreationParameters& params);
	
	HRESULT init();
	
	bool run();

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	virtual ITimer* createTimer() const;
	
	virtual void setWindowCaption(const char* caption);

	virtual ~CWin32Device();
private:
	HWND			mHwnd;
	HINSTANCE		m_hInstance;
};

#endif