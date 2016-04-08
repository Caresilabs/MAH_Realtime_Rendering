#pragma once

#include <string>
#include <windows.h>
#include <windowsx.h>
#include "Application.h"
#include "Input/WinInput.h"
#include "Graphics/DXGraphics.h"

class ApplicationListener;

struct DXApplicationConfig {
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPWSTR lpCmdLine;
	int nCmdShow;

	int width;
	int height;

	char* title;
	char* className;

	ApplicationListener* app;
};

class DXApplication : public Application {
public:
	DXApplication() = default;

	int Play( const DXApplicationConfig& config );

	~DXApplication();

	HINSTANCE				MainInstance;
	HWND					MainWindow;

	ApplicationListener*	Listener;

private:
	HRESULT					InitWindow( const DXApplicationConfig& config );

	WinInput*				Input;
	DXGraphics*				Graphics;

};

