#pragma once
#include "FrameBuffer.h"
#include "DXGraphics.h"

class DXFrameBuffer : public FrameBuffer {
public:
	DXFrameBuffer( int width, int height,  bool depth = true, bool colorView = true, bool useShaderResource = false );

	virtual void	Bind() override;

	virtual void	Unbind() override;

	virtual void	Clear( float r, float g, float b ) override;

	HRESULT			CreateDepthStencilView( int width, int height, bool useShaderResource );
	HRESULT			CreateRenderTargetView();

	virtual ~DXFrameBuffer();

	ID3D11RenderTargetView* RenderTargetView = nullptr;
	ID3D11DepthStencilView* DepthStencilView = nullptr;

	ID3D11Texture2D*        DepthStencil = nullptr;

	D3D11_VIEWPORT			ViewPort;

private:
	DXGraphics*				Graphics;
};

