#include "stdafx.h"
#include "PhongShader.h"
#include "Graphics/DXFrameBuffer.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

PhongShader::PhongShader( FrameBuffer* buffer, Camera* shadowCamera ) : ShadowCamera(shadowCamera) {
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

	// Create shadowmap Resource
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory( &shaderResourceViewDesc, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	auto dxBuffer = dynamic_cast<DXFrameBuffer*>(buffer);
	if ( FAILED( Device->CreateShaderResourceView( dxBuffer->DepthStencil, &shaderResourceViewDesc, &ShadowMapSRV ) ) ) {
		MessageBoxA( nullptr, "Failed to create shadowmap resource.", "Error", MB_OK | MB_ICONERROR );
	}

	// Load cubemap
	DirectX::CreateWICTextureFromFile( Device, DeviceContext, L"assets/skybox.png", &SkyBoxTex, &SkyBoxSRV );
}

void PhongShader::Begin( Camera& camera ) {
	ShaderProgram::Begin( camera );

	DeviceContext->PSSetSamplers( 0, 1, &SamplerState );

	PerFrameBufferData* frameCBuffer = GetCBuffer<PerFrameBufferData>( 0 );
	{
		frameCBuffer->ProjectionMatrix = linalg::transpose( camera.GetProjectionMatrix() );
		frameCBuffer->WorldToViewMatrix = linalg::transpose( camera.GetWorldToViewMatrix() );

		frameCBuffer->LightProjectionMatrix =  linalg::transpose(  ShadowCamera->GetProjectionMatrix() );
		frameCBuffer->LightToViewMatrix = linalg::transpose( ShadowCamera->GetWorldToViewMatrix() );

		frameCBuffer->IsDirectionalLight = false;
		frameCBuffer->LightPosition = vec4f( ShadowCamera->GetPosition(), 1);	//vec4f( 0.2f, 4, 0.2f, 0 ); //vec4f( 0, 7, 0, 0 );
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
		drawCBuffer->NormalUseTexture = material.map_Normal_TexSRV != nullptr && !LVP::Input->IsKeyDown( 'R' );
		drawCBuffer->MaskUseTexture = material.map_Mask_TexSRV != nullptr;

		drawCBuffer->Ka = material.Ka;
		drawCBuffer->Kd = material.Kd;
		drawCBuffer->Ks = material.Ks;
	}
	FlushCBuffer( 2 );

	if ( material.map_Kd_TexSRV != nullptr )
		DeviceContext->PSSetShaderResources( 0, 1, &material.map_Kd_TexSRV );

	if ( material.map_Ks_TexSRV != nullptr )
		DeviceContext->PSSetShaderResources( 1, 1, &material.map_Ks_TexSRV );

	if ( material.map_Normal_TexSRV != nullptr )
		DeviceContext->PSSetShaderResources( 2, 1, &material.map_Normal_TexSRV );

	if ( material.map_Mask_TexSRV != nullptr )
		DeviceContext->PSSetShaderResources( 3, 1, &material.map_Mask_TexSRV );

	// Bind Shadowmap
	DeviceContext->PSSetShaderResources( 4, 1, &ShadowMapSRV );

	DeviceContext->PSSetShaderResources( 5, 1, &SkyBoxSRV );
}

PhongShader::~PhongShader() {
	SAFE_RELEASE( ShadowMapSRV );
	SAFE_RELEASE( SamplerState );
}

