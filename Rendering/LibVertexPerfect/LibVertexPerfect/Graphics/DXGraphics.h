#pragma once
#include "Graphics.h"
#include "FrameBuffer.h"

#include <windows.h>
#include <D3D11.h>
#include <dinput.h>

class DXGraphics :
	public Graphics {
public:
	DXGraphics( HWND& window, int width, int height, bool vsync );

	void Init();

	// Inherited via Graphics
	virtual void Render(ApplicationListener* listener) override;

	virtual void ClearScreen( float r, float g, float b ) override;

	~DXGraphics();

	ID3D11Device*			Device = nullptr;
	ID3D11DeviceContext*	DeviceContext = nullptr;
	IDXGISwapChain*         SwapChain = nullptr;
	ID3D11RasterizerState*	RasterState = nullptr;

	HWND&					MainWindow;

	FrameBuffer*			ScreenBuffer = nullptr;

private:

	HRESULT					InitDirect3DAndSwapChain( int width, int height );
	void					InitRasterizerState();

	int Width, Height;

};

