#include "stdafx.h"
#include "DXGraphics.h"
#include "DXFrameBuffer.h"

DXGraphics::DXGraphics(HWND& window, int width, int height, bool vsync) : MainWindow(window), Width(width), Height(height) {
}

void DXGraphics::Init() {
	HRESULT hr = S_OK;

	if ( SUCCEEDED( hr = InitDirect3DAndSwapChain( Width, Height ) ) ) {
		InitRasterizerState();

		//if ( SUCCEEDED( hr = CreateRenderTargetView() ) &&
		//	SUCCEEDED( hr = CreateDepthStencilView( width, height ) ) ) {

		ScreenBuffer = new DXFrameBuffer( Width, Height, true );

		//SetViewport( Width, Height );

		//DeviceContext->OMSetRenderTargets( 1, &RenderTargetView, DepthStencilView );

		//if ( SUCCEEDED( hr = CreateShadersAndInputLayout() ) ) {
		//InitShaderBuffers();
		//initObjects();
		//}
		//}
	}
}

void DXGraphics::Render(ApplicationListener* listener) {
	ScreenBuffer->Bind();

	listener->Render();
	SwapChain->Present( 0, 0 );
}

void DXGraphics::ClearScreen( float r, float g, float b ) {
	ScreenBuffer->Clear( r, g, b );
}


DXGraphics::~DXGraphics() {
	SAFE_RELEASE( SwapChain );
	SAFE_RELEASE( RasterState );
}

HRESULT DXGraphics::InitDirect3DAndSwapChain( int width, int height ) {
	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE };

	DXGI_SWAP_CHAIN_DESC sd;
	memset( &sd, 0, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = MainWindow;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	HRESULT hr = E_FAIL;
	for ( UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE( driverTypes ) && FAILED( hr ); driverTypeIndex++ ) {
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			driverTypes[driverTypeIndex],
			nullptr,
			0,
			featureLevelsToTry,
			ARRAYSIZE( featureLevelsToTry ),
			D3D11_SDK_VERSION,
			&sd,
			&SwapChain,
			&Device,
			&initiatedFeatureLevel,
			&DeviceContext );
	}
	
	SwapChain->Present( 1, 0 );

	return hr;
}

void DXGraphics::InitRasterizerState() {
	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.CullMode = D3D11_CULL_BACK;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;

	Device->CreateRasterizerState( &rasterizerState, &RasterState );
	DeviceContext->RSSetState( RasterState );
}


