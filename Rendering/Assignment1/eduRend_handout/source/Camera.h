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

	float hz, vz;
	float mouseSense;

public:

	camera_t(
		float vfov,
		float aspect,
		float zNear,
		float zFar ) :
		vfov( vfov ), aspect( aspect ), zNear( zNear ), zFar( zFar ), mouseSense( 5 ), hz( PI ), vz( 0 ), direction( 0, 0, -1 ), up( 0, 1, 0 ) {
	}

	void moveTo( const vec3f& p ) {
		position = p;
	}

	void move( const vec3f& v ) {
		position += v;// *(mat3f::rotation( hz, 0, 1, 0 )* mat3f::rotation( vz, 1, 0, 0 ));
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

	void look( float horizontal, float vertical ) {
		if ( horizontal == 0 && vertical == 0 )
			return;

		hz = horizontal * mouseSense;
		vz = vertical * mouseSense;

		auto rot = mat3f::rotation( hz, 0, 1, 0 ) * mat3f::rotation( vz, 0, 1, 0 );
		direction = direction * mat3f::rotation( hz, up.x, up.y, up.z );
		auto tmp = (direction % up).normalize();
		direction = direction * mat3f::rotation( vz, tmp.x, tmp.y, tmp.z );

	}

	mat4f makeRotationDir() const {
		vec3f xaxis = (up %direction);
		xaxis.normalize();

		vec3f yaxis = direction % xaxis;
		yaxis.normalize();

		mat4f mat;

		mat.col[0] = vec4f( xaxis.x, yaxis.x, direction.x, 0 );

		mat.col[1] = vec4f( xaxis.y, yaxis.y, direction.y, 0 );

		mat.col[2] = vec4f( xaxis.z, yaxis.z, direction.z, 0 );

		mat.col[3] = vec4f( 0, 0, 0, 1 );
		return mat;
	}

	mat4f get_WorldToViewMatrix() const {
		return  makeRotationDir() * mat4f::translation( position );
	}

	mat4f get_ProjectionMatrix() const {
		return mat4f::projection( vfov, aspect, zNear, zFar );
	}
};

#endif