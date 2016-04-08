#pragma once
#include "ShaderProgram.h"

class PhongShader : public ShaderProgram {
public:
	PhongShader();
	
	virtual void Begin( Camera& camera ) override;

	virtual void Render( MeshInstance* instance ) override;

	~PhongShader();

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

};

