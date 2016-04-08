#include "stdafx.h"
#include "MeshInstance.h"

MeshInstance::MeshInstance( VPtr<Mesh>& mesh ) : MyMesh( mesh ), Scale( 1, 1, 1 ) {
	UpdateTransform();
}

void MeshInstance::Render() {
	MyMesh->Render();
}

void MeshInstance::UpdateTransform() {
	Transform = mat4f::translation( Position ) * mat4f::rotation( Rotation.x, Rotation.y, Rotation.z ) * mat4f::scaling( Scale );
}
