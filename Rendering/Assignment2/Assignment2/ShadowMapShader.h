#pragma once
#include "Graphics/Shaders/ShaderProgram.h"
#include "Graphics/MeshInstance.h"

class ShadowMapShader : public ShaderProgram {
public:
	ShadowMapShader( );

	virtual void Begin( Camera& camera ) override;

	virtual void RenderObject( MeshInstance* instance ) override;

	virtual void RenderDrawcall( const material_t& material ) override;

	virtual ~ShadowMapShader();

private:

	struct PerFrameBufferData {
		mat4f WorldToViewMatrix;
		mat4f ProjectionMatrix;
	};

	struct PerObjectBufferData {
		mat4f ModelToWorldMatrix;
	};

};

