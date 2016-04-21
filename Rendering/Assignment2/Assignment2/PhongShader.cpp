#include "stdafx.h"
#include "PhongShader.h"


PhongShader::PhongShader() {
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ShaderConfig config = { 0 };
	config.VertexFilePath = "assets/Shaders/Phong.vs";
	config.VertexEntrypoint = "VS_main";
	config.VertexTarget = "vs_5_0";

	config.PixelFilePath = "assets/Shaders/Phong.ps";
	config.PixelEntrypoint = "PS_main";
	config.PixelTarget = "ps_5_0";

	config.InputElements = &inputDesc[0];
	config.InputSize = ARRAYSIZE( inputDesc );

	Compile( config );

	AddCBuffer<PerFrameBufferData>( ShaderType::VERTEX );
	AddCBuffer<PerObjectBufferData>( ShaderType::VERTEX );
	AddCBuffer<PerDrawcallCBufferData>( ShaderType::FRAGMENT );

	// Init sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 8;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	auto Device = static_cast<DXGraphics*>(LVP::Graphics)->Device;
	auto hr = Device->CreateSamplerState( &samplerDesc, &SamplerState );
	if ( FAILED( hr ) ) {
		MessageBoxA( nullptr, "Failed to create texture sampler.", "Error", MB_OK | MB_ICONERROR );
	}
}

void PhongShader::Begin( Camera& camera ) {
	ShaderProgram::Begin( camera );

	DeviceContext->PSSetSamplers( 0, 1, &SamplerState );

	PerFrameBufferData* frameCBuffer = GetCBuffer<PerFrameBufferData>( 0 );
	{
		frameCBuffer->ProjectionMatrix = linalg::transpose( camera.GetProjectionMatrix() );
		frameCBuffer->WorldToViewMatrix = linalg::transpose( camera.GetWorldToViewMatrix() );

		frameCBuffer->IsDirectionalLight = false;
		frameCBuffer->LightPosition = vec4f( 0, 7, 0, 0 );
		frameCBuffer->CameraPosition = vec4f( camera.GetPosition(), 1 );
	}
	FlushCBuffer( 0 );
}


void PhongShader::RenderObject( MeshInstance* instance ) {
	PerObjectBufferData* objectCBuffer = GetCBuffer<PerObjectBufferData>( 1 );
	{
		objectCBuffer->ModelToWorldMatrix = linalg::transpose( instance->Transform );
	}
	FlushCBuffer( 1 );
}

void PhongShader::RenderDrawcall( const material_t& material ) {
	PerDrawcallCBufferData* drawCBuffer = GetCBuffer<PerDrawcallCBufferData>( 2 );
	{
		drawCBuffer->KdUseTexture = material.map_Kd_TexSRV != nullptr;
		drawCBuffer->KsUseTexture = material.map_Ks_TexSRV != nullptr;

		drawCBuffer->Ka = material.Ka;
		drawCBuffer->Kd = material.Kd;
		drawCBuffer->Ks = material.Ks;
	}
	FlushCBuffer( 2 );

	if (material.map_Kd_TexSRV != nullptr )
		DeviceContext->PSSetShaderResources( 0, 1, &material.map_Kd_TexSRV );

	if ( material.map_Ks_TexSRV != nullptr )
		DeviceContext->PSSetShaderResources( 1, 1, &material.map_Ks_TexSRV );
}

