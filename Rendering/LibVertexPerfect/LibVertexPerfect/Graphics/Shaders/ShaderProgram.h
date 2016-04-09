#pragma once
#include "../Camera.h"
#include <D3D11.h>
#include "../DXGraphics.h"
#include <vector>
#include "../DrawCall.h"

class MeshInstance;

class ShaderProgram {
public:
	ShaderProgram();

	virtual void Begin( Camera& camera );

	virtual void RenderObject( MeshInstance* instance ) = 0;

	virtual void RenderDrawcall( const material_t& material ) = 0;

	virtual void End();

	virtual		~ShaderProgram();

protected:

	struct ShaderConfig {
		char*						VertexFilePath;
		char*						VertexEntrypoint;
		char*						VertexTarget;

		char*						PixelFilePath;
		char*						PixelEntrypoint;
		char*						PixelTarget;

		D3D11_INPUT_ELEMENT_DESC*	InputElements;
		int							InputSize;
	};

	void							Compile( const ShaderConfig& config );

	template<typename T>
	void							AddCBuffer( ShaderType type = ShaderType::VERTEX );

	template<typename T>
	T*								GetCBuffer( int index );

	void							FlushCBuffer( int index );

	ID3D11DeviceContext*			DeviceContext;

private:
	struct CBufferContainer {
		ID3D11Buffer*				Buffer = nullptr;
		ShaderType					Type;
	};

	bool							CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );

	ID3D11InputLayout*				InputLayout = nullptr;
	ID3D11VertexShader*				VertexShader = nullptr;
	ID3D11PixelShader*				PixelShader = nullptr;

	std::unordered_map<int, CBufferContainer> CBuffers;
};


template<typename T>
inline void ShaderProgram::AddCBuffer( ShaderType type ) {

	if ( sizeof( T ) % 16 != 0 ) {
		std::cout << "Sorry, your shader needs to be divisible by 16 bytes";
		return;
	}

	CBufferContainer container;

	container.Type = type;

	HRESULT hr;

	// Matrix buffer
	D3D11_BUFFER_DESC MatrixBuffer_desc = { 0 };
	MatrixBuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBuffer_desc.ByteWidth = sizeof( T );
	MatrixBuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBuffer_desc.MiscFlags = 0;
	MatrixBuffer_desc.StructureByteStride = 0;

	ASSERT( hr = static_cast<DXGraphics*>(LVP::Graphics)->Device->CreateBuffer( &MatrixBuffer_desc, nullptr, &(container.Buffer) ) );

	CBuffers[CBuffers.size()] = container;
}

template<typename T>
inline T* ShaderProgram::GetCBuffer( int index ) {

	// map the resource buffer, obtain a pointer to it and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	DeviceContext->Map( CBuffers[index].Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );
	return (T*)resource.pData;
}
