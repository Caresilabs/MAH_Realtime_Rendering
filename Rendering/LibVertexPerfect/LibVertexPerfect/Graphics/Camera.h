#pragma once

#include "../Math/vec.h"
#include "../Math/mat.h"

using namespace linalg;

class Camera {
	float vfov, aspect;	// aperture attributes
	float zNear, zFar;	// clip planes
	vec3f position;

	vec3f up, direction;
	float mouseSense;

	bool IsPerspective;

	mat4f projectionMatrix;
	mat4f viewMatrix;
	mat4f invViewMatrix;

public:

	Camera(
		float vfov,
		float aspect,
		float zNear,
		float zFar,
		bool perspective = true); 

	void MoveTo( const vec3f& p );

	void Move( const vec3f& v );

	void MoveSideways( float speed );

	void MoveVertical( float speed );

	void MoveForward( float speed );

	void UpdateFrustrum();

	void Look( float horizontal, float vertical );

	mat4f LookAtMatrix() const;

	mat4f GetWorldToViewMatrix() const {
		return invViewMatrix;
	}

	mat4f GetModelToWorld() const {
		return viewMatrix;
	}

	mat4f GetProjectionMatrix() const {
		return projectionMatrix;
	}

	void SetAspectRatio(float value) {
		aspect = value;
	}

	const vec3f& GetDirection() const {
		return direction;
	}

	const vec3f& GetPosition() const {
		return position;
	}
};
