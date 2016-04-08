#include "stdafx.h"
#include "Camera.h"

Camera::Camera(
	float vfov,
	float aspect,
	float zNear,
	float zFar ) :
	vfov( vfov ), aspect( aspect ), zNear( zNear ), zFar( zFar ), mouseSense( 2 ), direction( 0, 0, -1 ), up( 0, 1, 0 ) {
}

void Camera::MoveTo( const vec3f & p ) {
	position = p;
}

void Camera::Move( const vec3f & v ) {
	position += v;
}

void Camera::MoveSideways( float speed ) {
	auto side = up % direction;

	position += side * speed;
}

void Camera::MoveVertical( float speed ) {
	position += up * speed;
}

void Camera::MoveForward( float speed ) {
	position += direction * speed;
}

void Camera::UpdateFrustrum() {
	invViewMatrix = (LookAtMatrix().inverse());
	projectionMatrix = mat4f::projection( vfov, aspect, zNear, zFar );
}

void Camera::Look( float horizontal, float vertical ) {
	if ( horizontal == 0 && vertical == 0 )
		return;

	float hz = horizontal * mouseSense;
	float vz = vertical * mouseSense;

	direction = mat3f::rotation( hz, up.x, up.y, up.z ) * direction;
	auto tmp = (direction % up).normalize();
	direction = mat3f::rotation( vz, tmp.x, tmp.y, tmp.z ) * direction;
	direction.normalize();

}

mat4f Camera::LookAtMatrix() const {
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
