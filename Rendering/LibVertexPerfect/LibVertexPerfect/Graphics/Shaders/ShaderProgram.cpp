#include "stdafx.h"
#include "ShaderProgram.h"
#include <D3D11.h>
#include <d3dCompiler.h>
#include <fstream>
#include "../DXGraphics.h"

ShaderProgram::ShaderProgram() {
	InitShaderBuffers();
	Compile();
}

void ShaderProgram::Begin( Camera & camera ) {
	auto DeviceContext = static_cast<DXGraphics*>(LVP::Graphics)->DeviceContext;

	//set topology
	DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ); /// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST

																					  //set vertex description
	DeviceContext->IASetInputLayout( InputLayout );

	//set shaders
	DeviceContext->VSSetShader( VertexShader, nullptr, 0 );
	DeviceContext->HSSetShader( nullptr, nullptr, 0 );
	DeviceContext->DSSetShader( nullptr, nullptr, 0 );
	DeviceContext->GSSetShader( nullptr, nullptr, 0 );
	DeviceContext->PSSetShader( PixelShader, nullptr, 0 );

	// set matrix buffers
	DeviceContext->VSSetConstantBuffers( 0, 1, &MatrixBuffer );

	cam = &camera;
}

void ShaderProgram::Render( MeshInstance* instance ) {
	MapMatrixBuffers( static_cast<DXGraphics*>(LVP::Graphics)->DeviceContext, instance->Transform, cam->GetWorldToViewMatrix(), cam->GetProjectionMatrix() );
	instance->Render();
}

void ShaderProgram::End() {
}

void ShaderProgram::MapMatrixBuffers( ID3D11DeviceContext * device_context, mat4f ModelToWorldMatrix, mat4f WorldToViewMatrix, mat4f ProjectionMatrix ) {
	// map the resource buffer, obtain a pointer to it and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	device_context->Map( MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );
	MatrixBuffer_t* matrix_buffer_ = (MatrixBuffer_t*)resource.pData;
	matrix_buffer_->ModelToWorldMatrix = linalg::transpose( ModelToWorldMatrix );
	matrix_buffer_->WorldToViewMatrix = linalg::transpose( WorldToViewMatrix );
	matrix_buffer_->ProjectionMatrix = linalg::transpose( ProjectionMatrix );
	device_context->Unmap( MatrixBuffer, 0 );
}

void ShaderProgram::InitShaderBuffers() {
	HRESULT hr;

	// Matrix buffer
	D3D11_BUFFER_DESC MatrixBuffer_desc = { 0 };
	MatrixBuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBuffer_desc.ByteWidth = sizeof( MatrixBuffer_t );
	MatrixBuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBuffer_desc.MiscFlags = 0;
	MatrixBuffer_desc.StructureByteStride = 0;

	ASSERT( hr = static_cast<DXGraphics*>(LVP::Graphics)->Device->CreateBuffer( &MatrixBuffer_desc, nullptr, &MatrixBuffer ) );
}

ShaderProgram::~ShaderProgram() {
	SAFE_RELEASE( InputLayout );
	SAFE_RELEASE( VertexShader );
	SAFE_RELEASE( PixelShader );
}

bool ShaderProgram::CompileShader( char * shaderFile, char * pEntrypoint, char * pTarget, D3D10_SHADER_MACRO * pDefines, ID3DBlob ** pCompiledShader ) {
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS |
		D3DCOMPILE_IEEE_STRICTNESS;

	std::string shader_code;
	std::ifstream in( shaderFile, std::ios::in | std::ios::binary );
	if ( in ) {
		in.seekg( 0, std::ios::end );
		shader_code.resize( (UINT)in.tellg() );
		in.seekg( 0, std::ios::beg );
		in.read( &shader_code[0], shader_code.size() );
		in.close();
	}

	ID3DBlob* pErrorBlob = nullptr;
	HRESULT hr = D3DCompile(
		shader_code.data(),
		shader_code.size(),
		nullptr,
		pDefines,
		nullptr,
		pEntrypoint,
		pTarget,
		dwShaderFlags,
		0,
		pCompiledShader,
		&pErrorBlob );

	if ( pErrorBlob ) {
		// output error message
		OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
#ifdef USECONSOLE
		printf( "%s\n", (char*)pErrorBlob->GetBufferPointer() );
#endif
		SAFE_RELEASE( pErrorBlob );
		
		return true; // todo
	}
	return true;
}

void ShaderProgram::Compile( ) {

	auto Device = static_cast<DXGraphics*>(LVP::Graphics)->Device;

	ID3DBlob* pVertexShader = nullptr;
	if (  CompileShader( "Shaders/DrawTri.vs", "VS_main", "vs_5_0", nullptr, &pVertexShader ) ) {
		if (  Device->CreateVertexShader(
			pVertexShader->GetBufferPointer(),
			pVertexShader->GetBufferSize(),
			nullptr,
			&VertexShader )  == S_OK ){

			D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			Device->CreateInputLayout(
				inputDesc,
				ARRAYSIZE( inputDesc ),
				pVertexShader->GetBufferPointer(),
				pVertexShader->GetBufferSize(),
				&InputLayout );
		}
		SAFE_RELEASE( pVertexShader );
	} else {
		MessageBoxA( nullptr, "Failed to create vertex shader (check Output window for more info)", 0, 0 );
	}

	ID3DBlob* pPixelShader = nullptr;
	if ( CompileShader( "Shaders/DrawTri.ps", "PS_main", "ps_5_0", nullptr, &pPixelShader ) )  {
		Device->CreatePixelShader(
			pPixelShader->GetBufferPointer(),
			pPixelShader->GetBufferSize(),
			nullptr,
			&PixelShader );

		SAFE_RELEASE( pPixelShader );
	} else {
		MessageBoxA( nullptr, "Failed to create pixel shader (check Output window for more info)", 0, 0 );
	}

}
