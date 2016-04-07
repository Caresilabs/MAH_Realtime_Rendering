#include "MyGame.h"

MyGame::MyGame() {
}

void MyGame::Start() {
	//wheel = new OBJMesh( "../assets/tyre/Tyre.obj" );
	wheel = new CubeMesh();
	 instance = new MeshInstance( wheel );
	 shader = new ShaderProgram();

	 cam = new Camera( fPI / 4,				/*field-of-view*/
		 (float)900 / 900,					/*aspect ratio*/
		 .5f,								/*z-near plane (everything closer will be clipped/removed)*/
		 500.0f );
	 cam->MoveTo( { 0, 0, 5 } );
}

void MyGame::Update( float delta ) {
	cam->UpdateFrustrum();
	instance->Transform = instance->Transform * mat4f::rotation( delta, { 0, 1, 0 } );

	if ( LVP::Input->IsKeyDown( 'S' ) ) {
		cam->MoveForward( -delta );
	}
	if ( LVP::Input->IsKeyDown( 'W' ) ) {
		cam->MoveForward( delta );
	}
}

void MyGame::Render() {
	LVP::Graphics->ClearScreen( 1, 0.5f, 0 );

	shader->Begin(*cam);

	shader->Render( instance );

	shader->End();
}

MyGame::~MyGame() {
}