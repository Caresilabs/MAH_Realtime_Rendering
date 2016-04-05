#include "stdafx.h"
#include "DXGraphics.h"

DXGraphics::DXGraphics(HWND& window, bool vsync) : MainWindow(window) {
	float width = 900;
	float height = 900;
	HRESULT hr = S_OK;

	if ( SUCCEEDED( hr = InitDirect3DAndSwapChain( width, height ) ) ) {
		InitRasterizerState();

		if ( SUCCEEDED( hr = CreateRenderTargetView() ) &&
			SUCCEEDED( hr = CreateDepthStencilView( width, height ) ) ) {
			SetViewport( width, height );

			DeviceContext->OMSetRenderTargets( 1, &RenderTargetView, DepthStencilView );

			//if ( SUCCEEDED( hr = CreateShadersAndInputLayout() ) ) {
				//InitShaderBuffers();
				//initObjects();
			//}
		}
	}
}

void DXGraphics::Render(ApplicationListener* listener) {
	listener->Render();
	SwapChain->Present( 0, 0 );
}

void DXGraphics::ClearScreen( float r, float g, float b ) {
	static float ClearColor[4] = { r, g, b, 1 };
	DeviceContext->ClearRenderTargetView( RenderTargetView, ClearColor );
}


DXGraphics::~DXGraphics() {
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

void DXGraphics::SetViewport( int width, int height ) {
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DeviceContext->RSSetViewports( 1, &vp );
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


HRESULT DXGraphics::CreateDepthStencilView( int width, int height ) {
	HRESULT hr = S_OK;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = Device->CreateTexture2D( &descDepth, nullptr, &DepthStencil );
	if ( FAILED( hr ) )
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof( descDSV ) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = Device->CreateDepthStencilView( DepthStencil, &descDSV, &DepthStencilView );

	return hr;
}


HRESULT DXGraphics::CreateRenderTargetView() {
	HRESULT hr = S_OK;
	// Create a render target view
	ID3D11Texture2D* pBackBuffer;
	if ( SUCCEEDED( hr = SwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer ) ) ) {
		hr = Device->CreateRenderTargetView( pBackBuffer, nullptr, &RenderTargetView );
		SAFE_RELEASE( pBackBuffer );
	}

	return hr;
}


