#include "stdafx.h"
#include "ShaderProgram.h"
#include <D3D11.h>
#include <d3dCompiler.h>
#include <fstream>
#include "../DXGraphics.h"

ShaderProgram::ShaderProgram() {
	DeviceContext = static_cast<DXGraphics*>(LVP::Graphics)->DeviceContext;
}

void ShaderProgram::Begin( Camera & camera ) {
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

	for ( auto& buffer : CBuffers ) {
		switch ( buffer.second.Type ) {
		case ShaderType::VERTEX:
			DeviceContext->VSSetConstantBuffers( buffer.first, 1, &buffer.second.Buffer );
			break;
		case ShaderType::FRAGMENT:
			DeviceContext->PSSetConstantBuffers( buffer.first, 1, &buffer.second.Buffer );
			break;
		default:
			break;
		}
	}
}


void ShaderProgram::End() {
	//DeviceContext->Flush();
}

ShaderProgram::~ShaderProgram() {
	for ( auto& buffer : CBuffers ) {
		SAFE_RELEASE( buffer.second.Buffer );
	}

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

	}
	return *pCompiledShader != nullptr;
}

void ShaderProgram::Compile( const ShaderConfig& config ) {

	auto Device = static_cast<DXGraphics*>(LVP::Graphics)->Device;

	// Vertex shader
	ID3DBlob* pVertexShader = nullptr;
	if ( CompileShader( config.VertexFilePath, config.VertexEntrypoint, config.VertexTarget, nullptr, &pVertexShader ) ) {
		if ( Device->CreateVertexShader(
			pVertexShader->GetBufferPointer(),
			pVertexShader->GetBufferSize(),
			nullptr,
			&VertexShader ) == S_OK ) {

			Device->CreateInputLayout(
				config.InputElements,
				config.InputSize,	//ARRAYSIZE( inputDesc ),
				pVertexShader->GetBufferPointer(),
				pVertexShader->GetBufferSize(),
				&InputLayout );
		}
		SAFE_RELEASE( pVertexShader );
	} else {
		MessageBoxA( nullptr, "Failed to create vertex shader (check Output window for more info)", 0, 0 );
	}

	// Pixel shader
	ID3DBlob* pPixelShader = nullptr;
	if ( CompileShader( config.PixelFilePath, config.PixelEntrypoint, config.PixelTarget, nullptr, &pPixelShader ) ) {
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

void ShaderProgram::FlushCBuffer( int index ) {
	DeviceContext->Unmap( CBuffers[index].Buffer, 0 );
}
