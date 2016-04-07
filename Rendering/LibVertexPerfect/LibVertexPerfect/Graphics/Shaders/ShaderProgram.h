#pragma once
#include "../Camera.h"
#include "../MeshInstance.h"
#include <D3D11.h>

struct MatrixBuffer_t {
	mat4f ModelToWorldMatrix;
	mat4f WorldToViewMatrix;
	mat4f ProjectionMatrix;
};

class ShaderProgram {
public:
	ShaderProgram();

	virtual void Begin( Camera& camera );

	virtual void Render( MeshInstance* instance );

	virtual void End();

	void MapMatrixBuffers(
		ID3D11DeviceContext* device_context,
		mat4f ModelToWorldMatrix,
		mat4f WorldToViewMatrix,
		mat4f ProjectionMatrix );

	void InitShaderBuffers();

	~ShaderProgram();

protected:
	void					Compile(  );

private:

	bool					CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );

	ID3D11InputLayout*		InputLayout = nullptr;
	ID3D11VertexShader*		VertexShader = nullptr;
	ID3D11PixelShader*		PixelShader = nullptr;

	ID3D11Buffer*			MatrixBuffer = nullptr;

	Camera* cam;


};

