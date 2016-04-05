#pragma once
#include "Graphics.h"

#include <D3D11.h>
#include <d3dCompiler.h>
#include <dinput.h>

class DXGraphics :
	public Graphics {
public:
	DXGraphics();
	~DXGraphics();
private:

	void InitRasterizerState();

	IDXGISwapChain*         SwapChain = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	ID3D11Texture2D*        DepthStencil = nullptr;
	ID3D11DepthStencilView* DepthStencilView = nullptr;
	ID3D11Device*			Device = nullptr;
	ID3D11DeviceContext*	DeviceContext = nullptr;
	ID3D11RasterizerState*	RasterState = nullptr;
};

