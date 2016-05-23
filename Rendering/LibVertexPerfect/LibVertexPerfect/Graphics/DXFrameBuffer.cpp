#include "stdafx.h"
#include "DXFrameBuffer.h"
#include "ScreenGrab.h"
#include <wincodec.h>

DXFrameBuffer::DXFrameBuffer( int width, int height, bool depthView, bool colorView, bool useShaderResource ) : FrameBuffer( width, height, depthView, colorView ) {
	Graphics = dynamic_cast<DXGraphics*>(LVP::Graphics);

	if ( Color ) {
		CreateRenderTargetView();
	}

	if ( Depth ) {
		CreateDepthStencilView( width, height, useShaderResource );
	}
	
	// New viewport
	ViewPort.Width = (float)width;
	ViewPort.Height = (float)height;
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
}

void DXFrameBuffer::Bind() {
	Graphics->DeviceContext->RSSetViewports( 1, &ViewPort );

	if ( Color )
		Graphics->DeviceContext->OMSetRenderTargets( 1, &RenderTargetView, DepthStencilView );
	else
		Graphics->DeviceContext->OMSetRenderTargets( 0, nullptr, DepthStencilView );
}

void DXFrameBuffer::Unbind() {
	Graphics->ScreenBuffer->Bind();
}

void DXFrameBuffer::Clear( float r, float g, float b ) {
	static float ClearColor[4] = { r, g, b, 1 };

	if ( Color )
		Graphics->DeviceContext->ClearRenderTargetView( RenderTargetView, ClearColor );

	if ( Depth )
		Graphics->DeviceContext->ClearDepthStencilView( DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

HRESULT DXFrameBuffer::CreateDepthStencilView( int width, int height, bool useShaderResource ) {
	HRESULT hr = S_OK;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof( descDepth ) );

	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;

	if ( useShaderResource ) {
		descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	} else {
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}

	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = Graphics->Device->CreateTexture2D( &descDepth, nullptr, &DepthStencil );
	if ( FAILED( hr ) )
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof( descDSV ) );

	if ( useShaderResource ) {
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	} else {
		descDSV.Format = descDepth.Format;
	}

	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = Graphics->Device->CreateDepthStencilView( DepthStencil, &descDSV, &DepthStencilView );

	return hr;
}


HRESULT DXFrameBuffer::CreateRenderTargetView() {
	HRESULT hr = S_OK;
	// Create a render target view
	ID3D11Texture2D* pBackBuffer;
	if ( SUCCEEDED( hr = Graphics->SwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer ) ) ) {
		hr = Graphics->Device->CreateRenderTargetView( pBackBuffer, nullptr, &RenderTargetView );
		SAFE_RELEASE( pBackBuffer );
	}

	return hr;
}

DXFrameBuffer::~DXFrameBuffer() {
	SAFE_RELEASE( RenderTargetView );
	SAFE_RELEASE( DepthStencil );
	SAFE_RELEASE( DepthStencilView );
}

