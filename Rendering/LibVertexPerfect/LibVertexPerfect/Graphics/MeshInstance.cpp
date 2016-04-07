#include "stdafx.h"
#include "MeshInstance.h"

MeshInstance::MeshInstance( Mesh * mesh ) : MyMesh(mesh), Scale(1,1,1) {
	Transform = mat4f::rotation( 45, 0.0f, 1.0f, 0.0f );// (mat4f::TRS( Position, 0, Rotation, Scale ));
}

void MeshInstance::Render() {
	MyMesh->Render();
}
