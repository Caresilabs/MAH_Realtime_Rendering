#pragma once
#include "Graphics/Shaders/ShaderProgram.h"
#include "Graphics/MeshInstance.h"

class PhongShader : public ShaderProgram {
public:
	PhongShader( FrameBuffer* buffer, Camera* shadowCamera );

	virtual void Begin( Camera& camera ) override;

	virtual void RenderObject( MeshInstance* instance ) override;

	virtual void RenderDrawcall( const material_t& material ) override;

	virtual ~PhongShader();

private:

	ID3D11SamplerState*			SamplerState;
	ID3D11ShaderResourceView*	ShadowMapSRV;

	Camera*						ShadowCamera;

	struct PerFrameBufferData {
		mat4f WorldToViewMatrix;
		mat4f ProjectionMatrix;
		vec4f LightPosition;
		vec4f CameraPosition;
		mat4f LightProjectionMatrix;
		int   IsDirectionalLight;
		vec3f Padding;
	};

	struct PerObjectBufferData {
		mat4f ModelToWorldMatrix;
	};

	struct PerDrawcallCBufferData {

		vec3f Ka;
		int	  KaUseTexture;

		vec3f Kd;
		int	  KdUseTexture;

		vec3f Ks;
		int	  KsUseTexture;

		int	  NormalUseTexture;
		int	  MaskUseTexture;

		vec2f Pad;
	};


};

