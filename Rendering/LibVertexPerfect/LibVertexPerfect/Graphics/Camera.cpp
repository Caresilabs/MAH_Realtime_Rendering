#include "stdafx.h"
#include "Camera.h"

Camera::Camera(
	float vfov,
	float aspect,
	float zNear,
	float zFar ) :
	vfov( vfov ), aspect( aspect ), zNear( zNear ), zFar( zFar ), mouseSense( 2 ), direction( 0, 0, -1 ), up( 0, 1, 0 ) {
}

