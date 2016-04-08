#pragma once
#include "Mesh.h"
#include "../Math/vec.h"
#include "../Math/mat.h"

class MeshInstance {
public:
	MeshInstance( VPtr<Mesh>& mesh );

	void Render();

	void UpdateTransform();

	mat4f Transform;

	vec3f Position;
	vec3f Rotation;
	vec3f Scale;
	
private:
	VPtr<Mesh> MyMesh;
};

