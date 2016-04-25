#include "stdafx.h"
#include "Camera.h"

Camera::Camera(
	float vfov,
	float aspect,
	float zNear,
	float zFar ) :
	vfov( vfov ), aspect( aspect ), zNear( zNear ), zFar( zFar ), mouseSense( 0.5f ), direction( 0, 0, -1 ), up( 0, 1, 0 ) {
}

void Camera::MoveTo( const vec3f & p ) {
	position = p;
}

void Camera::Move( const vec3f & v ) {
	position += v;
}

void Camera::MoveSideways( float speed ) {
	auto side = direction % up;

	position += side * speed;
}

void Camera::MoveVertical( float speed ) {
	position += up * speed;
}

void Camera::MoveForward( float speed ) {
	position += direction * speed;
}

void Camera::UpdateFrustrum() {
	invViewMatrix = (LookAtMatrix());
	projectionMatrix = mat4f::projection( vfov, aspect, zNear, zFar );
}

void Camera::Look( float horizontal, float vertical ) {
	if ( horizontal == 0 && vertical == 0 )
		return;

	float hz = -horizontal * mouseSense;
	float vz = -vertical * mouseSense;

	direction = mat3f::rotation( hz, up.x, up.y, up.z ) * direction;

	auto tmp = (up % direction).normalize();
	direction = mat3f::rotation( vz, tmp.x, tmp.y, tmp.z ) * direction;
	direction.normalize();

}

mat4f Camera::LookAtMatrix() const {
	vec3f tempDir = -direction;

	vec3f right = up %  tempDir;
	right.normalize();

	vec3f realUp = tempDir % right;
	realUp.normalize();

	mat4f mat( right.x, realUp.x, tempDir.x, 0.f,
			right.y, realUp.y, tempDir.y, 0.f,
			right.z, realUp.z, tempDir.z, 0.f,
			0.f, 0.f, 0.f, 1.0f					///-linalg::dot( right, position ), -linalg::dot( realUp, position ), -linalg::dot( tempDir, position )
	);

	return   linalg::transpose( mat ) * mat4f::translation(- position ) ;
}
