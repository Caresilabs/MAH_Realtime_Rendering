#include "stdafx.h"
#include "DXApplication.h"
#include "ApplicationListener.h"
#include "Input/WinInput.h"

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	LRESULT Result = 0;

	PAINTSTRUCT ps;
	HDC hdc;

	// catch any relevant messages here
	switch ( uMsg ) {
	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		EndPaint( hWnd, &ps );
		break;
	case WM_ACTIVATE:

		break;
	case WM_KILLFOCUS:
		LVP::Input->SetCatchMouse( false );
		break;
	case WM_SIZING:
	{
		DXGraphics* Graphics = static_cast<DXGraphics*>(LVP::Graphics);
		DXApplication* App = static_cast<DXApplication*>(LVP::App);

		RECT size;
		if ( GetClientRect( App->MainWindow, &size ) ) {
			App->Width = size.right - size.left;
			App->Height = size.bottom - size.top;

			//Graphics->SetViewport( App->Width, App->Height );
			App->Listener->Resize();
		}
		break;
	}
	case WM_CREATE:

		break;
	case WM_DESTROY:
		LVP::App->IsRunning = false;
		break;
	case WM_KEYDOWN:							// Here's what you're interested in! -- check to see if someone pressed a key.
	{
		break;
	}
	case WM_KEYUP:								// This message arrives when person lets go of key.
	{
		break;
	}
	default:
		Result = DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return Result;
}

int DXApplication::Play( const DXApplicationConfig& config ) {
	Width = config.width;
	Height = config.height;

	// Console
	AllocConsole();
	freopen( "conin$", "r", stdin );
	freopen( "conout$", "w", stdout );
	freopen( "conout$", "w", stderr );

	if ( FAILED( InitWindow( config ) ) ) {
		return 0;
	}

	LVP::App = this;

	Graphics = new DXGraphics( MainWindow, Width, Height, true );
	LVP::Graphics = Graphics;
	Graphics->Init();

	Input = new WinInput();
	LVP::Input = Input;

	// todo
	Listener = config.app;
	Listener->Start();

	// init time
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency( (LARGE_INTEGER*)&cntsPerSec );
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter( (LARGE_INTEGER*)&prevTimeStamp );

	MSG Msg = { 0 };
	while ( IsRunning ) {
		// Check in with OS
		while ( PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE ) ) {
			switch ( Msg.message ) {
			case WM_KEYDOWN:
				Input->Keys[Msg.wParam] = true;
				break;
			case WM_KEYUP:
				Input->Keys[Msg.wParam] = false;
				break;
			case WM_LBUTTONDOWN:
			{
				// Bad
				Input->ResetCursor( MainWindow );
				Input->SetCatchMouse( true );
				break;
			}
			case WM_MOUSEMOVE:
			{
				break;
			}
			default:
				break;
			}

			TranslateMessage( &Msg );
			DispatchMessage( &Msg );
		}

		__int64 currTimeStamp = 0;
		QueryPerformanceCounter( (LARGE_INTEGER*)&currTimeStamp );
		float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;

		Input->Update( MainWindow );

		Listener->Update( dt );
		LVP::Graphics->Render( Listener );

		prevTimeStamp = currTimeStamp;
	}

	FreeConsole();

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
	r.right = Width;
	r.bottom = Height;
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

