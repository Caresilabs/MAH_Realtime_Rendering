#include "stdafx.h"

#include "DXGraphics.h"


DXGraphics::DXGraphics() {
	
}


DXGraphics::~DXGraphics() {
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
