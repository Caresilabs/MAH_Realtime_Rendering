#include "stdafx.h"
#include "MeshInstance.h"
#include "Shaders/ShaderProgram.h"

MeshInstance::MeshInstance( VPtr<Mesh>& mesh ) : MyMesh( mesh ), Scale( 1, 1, 1 ) {
	UpdateTransform();
}

void MeshInstance::Render( ShaderProgram& shader ) {
	shader.RenderObject( this );
	MyMesh->Render( shader );
}

void MeshInstance::UpdateTransform() {
	Transform = mat4f::translation( Position ) * mat4f::rotation( Rotation.x, Rotation.y, Rotation.z ) * mat4f::scaling( Scale );
}
