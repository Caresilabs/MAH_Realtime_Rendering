//
//  Camera.h
//
//	Basic camera class
//

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vec\vec.h"
#include "vec\mat.h"

using namespace linalg;

class camera_t {
	float vfov, aspect;	// aperture attributes
	float zNear, zFar;	// clip planes
	vec3f position;

	vec3f up, direction;
	float mouseSense;

	mat4f projectionMatrix;
	mat4f invViewMatrix;

public:

	camera_t(
		float vfov,
		float aspect,
		float zNear,
		float zFar ) :
		vfov( vfov ), aspect( aspect ), zNear( zNear ), zFar( zFar ), mouseSense( 3 ), direction( 0, 0, -1 ), up( 0, 1, 0 ) {
	}

	void moveTo( const vec3f& p ) {
		position = p;
	}

	void move( const vec3f& v ) {
		position += v;
	}

	void moveSideways( float speed ) {
		auto side = up % direction;

		position += side * speed;
	}

	void moveVertical( float speed ) {
		position += up * speed;
	}

	void moveForward( float speed ) {
		position += direction * speed;
	}

	void updateFrustrum() {
		invViewMatrix = (lookAtMatrix().inverse());
		projectionMatrix = mat4f::projection( vfov, aspect, zNear, zFar );
	}

	void look( float horizontal, float vertical ) {
		if ( horizontal == 0 && vertical == 0 )
			return;

		float hz = horizontal * mouseSense;
		float vz = vertical * mouseSense;

		direction = mat3f::rotation( hz, up.x, up.y, up.z ) * direction;
		auto tmp = (direction % up).normalize();
		direction = mat3f::rotation( vz, tmp.x, tmp.y, tmp.z ) * direction;
		direction.normalize();

	}

	mat4f lookAtMatrix() const {
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

	mat4f get_WorldToViewMatrix() const {
		return  invViewMatrix;
	}

	mat4f get_ProjectionMatrix() const {
		return projectionMatrix;
	}
};

#endif