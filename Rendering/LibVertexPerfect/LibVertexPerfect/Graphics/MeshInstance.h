#pragma once
#include "Mesh.h"
#include "../Math/vec.h"
#include "../Math/mat.h"

class MeshInstance {
public:
	MeshInstance( Mesh* mesh );

	void Render();

	mat4f Transform;
protected:
	vec3f Position;
	vec3f Rotation;
	vec3f Scale;
private:
	Mesh* MyMesh;
};

