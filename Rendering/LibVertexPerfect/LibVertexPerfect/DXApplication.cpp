#include "stdafx.h"
#include "DXApplication.h"
#include "ApplicationListener.h"
#include "Graphics/DXGraphics.h"

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	LRESULT Result = 0;

	// catch any relevant messages here
	switch ( uMsg ) {
	case WM_ACTIVATE:

		break;
	case WM_CREATE:

		break;
	case WM_DESTROY:
		LVP::App->IsRunning = false;
		break;
	case WM_KEYDOWN:
	{
		if ( wParam == 'A' ) {
		}
		break;
	}
	default:
		Result = DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return Result;
}

int DXApplication::Play( const DXApplicationConfig& config ) {
	if ( FAILED( InitWindow( config ) ) ) {
		return 0;
	}

	//TODO
	Width = 900;
	Height = 900;

	LVP::App = this;
	LVP::Graphics = new DXGraphics();

	// todo
	Listener = config.app;
	Listener->Start();

	// init time
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency( &Frequency );

	SecondsPerTick = 1.0 / (double)Frequency.QuadPart;

	LARGE_INTEGER Counter;
	QueryPerformanceCounter( &Counter );
	TimePassed = Counter.QuadPart;

	float Oldtime = 0;

	MSG Msg = { 0 };
	while ( IsRunning ) {
		float NewTime = FloatTime();

		// Check in with OS
		while ( PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &Msg );
			DispatchMessage( &Msg );
		}
		float dt = NewTime - Oldtime;
		Listener->Update( dt );
		Listener->Render();

		Oldtime = NewTime;
	}

}

DXApplication::~DXApplication() {
	SAFE_DELETE( Listener );
}

HRESULT DXApplication::InitWindow( const DXApplicationConfig& config ) {
	this->MainInstance = config.hInstance;

	WNDCLASSEXA Wc = { 0 };
	Wc.cbSize = sizeof( Wc );
	Wc.lpfnWndProc = MainWndProc;
	Wc.hInstance = MainInstance;
	Wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
	Wc.lpszClassName = config.className;

	if ( !RegisterClassExA( &Wc ) )
		exit( EXIT_FAILURE );

	// Adjust and create window

	DWORD WindowExStyle = 0;
	DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	// create rectangle for window
	RECT r = { 0 };
	r.right = 900;
	r.bottom = 900;
	AdjustWindowRectEx( &r, WindowStyle, 0, WindowExStyle );

	// create our window
	MainWindow = CreateWindowExA(
		WindowExStyle, config.className,
		config.title, WindowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		r.right - r.left, r.bottom - r.top,
		NULL, NULL,
		MainInstance, 0 );

	ShowWindow( MainWindow, config.nCmdShow );

	return S_OK;
}

float DXApplication::FloatTime() {
	LARGE_INTEGER Counter;
	QueryPerformanceCounter( &Counter );

	__int64 Interval = Counter.QuadPart - TimeCount;
	TimeCount = Counter.QuadPart;

	double SecondsGoneBy = (double)Interval * SecondsPerTick;
	TimePassed += SecondsGoneBy;

	return (float)TimePassed;
}
