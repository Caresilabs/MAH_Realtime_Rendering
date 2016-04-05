#pragma once
#include "Graphics.h"

#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>
#include <dinput.h>

class DXGraphics :
	public Graphics {
public:
	DXGraphics( HWND& window, bool vsync );

	// Inherited via Graphics
	virtual void Render(ApplicationListener* listener) override;

	~DXGraphics();
private:

	HRESULT InitDirect3DAndSwapChain( int width, int height );
	void SetViewport( int width, int height );
	void InitRasterizerState();
	HRESULT CreateDepthStencilView( int width, int height );
	HRESULT CreateRenderTargetView();

	HWND&					MainWindow;

	IDXGISwapChain*         SwapChain = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	ID3D11Texture2D*        DepthStencil = nullptr;
	ID3D11DepthStencilView* DepthStencilView = nullptr;
	ID3D11Device*			Device = nullptr;
	ID3D11DeviceContext*	DeviceContext = nullptr;
	ID3D11RasterizerState*	RasterState = nullptr;
};

