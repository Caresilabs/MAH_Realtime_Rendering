#pragma once
#include "Graphics/Shaders/ShaderProgram.h"
#include "Graphics/MeshInstance.h"

class PhongShader : public ShaderProgram {
public:
	PhongShader();
	
	virtual void Begin( Camera& camera ) override;

	virtual void RenderObject( MeshInstance* instance ) override;

	virtual void RenderDrawcall( const material_t& material ) override;

private:

	ID3D11SamplerState* SamplerState;

	struct PerFrameBufferData {
		mat4f WorldToViewMatrix;
		mat4f ProjectionMatrix;
		vec4f LightPosition;
		vec4f ViewDirection;
		int   IsDirectionalLight;
		vec3f Padding;
		
	};

	struct PerObjectBufferData {
		mat4f ModelToWorldMatrix;
	};

	struct PerDrawcallCBufferData {
		int	  UseTexture;
		vec3f Ka;
		
		vec3f Kd;
		float Pad2;

		vec3f Ks;
		float Pad3;
	};


};

