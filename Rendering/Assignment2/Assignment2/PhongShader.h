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
		vec3f Ka;
		float Pad1;
		
		vec3f Kd;
		float Pad2;

		vec3f Ks;
		float Pad3;
	};


};

