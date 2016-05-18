#pragma once
#include "Mesh.h"
#include "../Math/vec.h"
#include "../Math/mat.h"

class ShaderProgram;

class MeshInstance {
public:
	MeshInstance( VPtr<Mesh>& mesh );

	void Render( ShaderProgram& shader );

	void UpdateTransform();

	mat4f Transform;

	vec3f Position;
	vec3f Rotation;
	vec3f Scale;

	VPtr<Mesh> MeshPtr;

};

