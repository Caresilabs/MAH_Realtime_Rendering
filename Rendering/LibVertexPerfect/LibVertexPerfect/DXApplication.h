#pragma once

#include <string>
#include <windows.h>
#include "Application.h"

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

	float					FloatTime();

	HINSTANCE				MainInstance;
	HWND					MainWindow;

	ApplicationListener*	Listener;

	int						Width;
	int						Height;


	float					SecondsPerTick;
	__int64					TimeCount;
	double					TimePassed;
};

