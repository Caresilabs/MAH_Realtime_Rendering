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
	config.VertexFilePath = "../assets/Shaders/Phong.vs";
	config.VertexEntrypoint = "VS_main";
	config.VertexTarget = "vs_5_0";

	config.PixelFilePath = "../assets/Shaders/Phong.ps";
	config.PixelEntrypoint = "PS_main";
	config.PixelTarget = "ps_5_0";

	config.InputElements = &inputDesc[0];
	config.InputSize = ARRAYSIZE( inputDesc );

	Compile( config );

	AddCBuffer<PerFrameBufferData>();
	AddCBuffer<PerObjectBufferData>();
}

void PhongShader::Begin( Camera& camera ) {
	ShaderProgram::Begin( camera );

	PerFrameBufferData* frameCBuffer = GetCBuffer<PerFrameBufferData>( 0 );
	{
		frameCBuffer->ProjectionMatrix = linalg::transpose( camera.GetProjectionMatrix() );
		frameCBuffer->WorldToViewMatrix = linalg::transpose( camera.GetWorldToViewMatrix() );

		frameCBuffer->IsDirectionalLight = true;
		frameCBuffer->LightPosition = vec4f( 10, 10, 10, 0 );
		frameCBuffer->ViewDirection = vec4f(camera.GetDirection(), 0);
	}
	FlushCBuffer( 0 );
}

void PhongShader::Render( MeshInstance* instance ) {
	PerObjectBufferData* objectCBuffer = GetCBuffer<PerObjectBufferData>( 1 );
	{
		objectCBuffer->ModelToWorldMatrix = linalg::transpose( instance->Transform );
	}
	FlushCBuffer( 1 );

	instance->Render();
}

PhongShader::~PhongShader() {
}
