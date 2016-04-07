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

	mat4f projectionMatrix;
	mat4f invViewMatrix;

public:

	Camera(
		float vfov,
		float aspect,
		float zNear,
		float zFar ); 

	void MoveTo( const vec3f& p ) {
		position = p;
	}

	void Move( const vec3f& v ) {
		position += v;
	}

	void MoveSideways( float speed ) {
		auto side = up % direction;

		position += side * speed;
	}

	void MoveVertical( float speed ) {
		position += up * speed;
	}

	void MoveForward( float speed ) {
		position += direction * speed;
	}

	void UpdateFrustrum() {
		invViewMatrix = (LookAtMatrix().inverse());
		projectionMatrix = mat4f::projection( vfov, aspect, zNear, zFar );
	}

	void Look( float horizontal, float vertical ) {
		if ( horizontal == 0 && vertical == 0 )
			return;

		float hz = horizontal * mouseSense;
		float vz = vertical * mouseSense;

		direction = mat3f::rotation( hz, up.x, up.y, up.z ) * direction;
		auto tmp = (direction % up).normalize();
		direction = mat3f::rotation( vz, tmp.x, tmp.y, tmp.z ) * direction;
		direction.normalize();

	}

	mat4f LookAtMatrix() const {
		vec3f tempDir = -direction;

		vec3f right = tempDir % up;
		right.normalize();

		vec3f realUp = right % tempDir;
		realUp.normalize();

		mat4f mat;

		mat.col[0] = vec4f( right.x, right.y, right.z, 0 );
		mat.col[1] = vec4f( realUp.x, realUp.y, realUp.z, 0 );
		mat.col[2] = vec4f( tempDir.x, tempDir.y, tempDir.z, 0 );
		mat.col[3] = vec4f( position.x, position.y, position.z, 1 );
		return mat;
	}

	mat4f GetWorldToViewMatrix() const {
		return  invViewMatrix;
	}

	mat4f GetProjectionMatrix() const {
		return projectionMatrix;
	}

	void SetAspectRatio(float value) {
		aspect = value;
	}
};
