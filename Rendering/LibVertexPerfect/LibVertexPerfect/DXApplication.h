#pragma once

#include <string>
#include <windows.h>
#include <windowsx.h>
#include "Application.h"
#include "Input/WinInput.h"

class ApplicationListener;

struct DXApplicationConfig {
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPWSTR lpCmdLine;
	int nCmdShow;

	char* title;
	char* className;

	ApplicationListener* app;
};

class DXApplication : public Application {
public:
	DXApplication() = default;

	int Play( const DXApplicationConfig& config );

	~DXApplication();
private:
	HRESULT					InitWindow( const DXApplicationConfig& config );

	HINSTANCE				MainInstance;
	HWND					MainWindow;

	ApplicationListener*	Listener;

	WinInput*				Input;

	int						Width;
	int						Height;
};

