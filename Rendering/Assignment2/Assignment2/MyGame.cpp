#include "MyGame.h"

MyGame::MyGame() {
}

void MyGame::Start() {
	//wheel = new OBJMesh( "../assets/tyre/Tyre.obj" );
	wheel = new CubeMesh();
	 instance = new MeshInstance( wheel );
	 shader = new PhongShader();

	 cam = new Camera( fPI / 4,				/*field-of-view*/
		 (float)LVP::App->Width / LVP::App->Height,					/*aspect ratio*/
		 .5f,								/*z-near plane (everything closer will be clipped/removed)*/
		 500.0f );
	 cam->MoveTo( { 0, 0, 5 } );
}

void MyGame::Update( float delta ) {
	static float speed = 3;

	if ( LVP::Input->IsKeyDown( 'S' ) ) {
		cam->MoveForward( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'W' ) ) {
		cam->MoveForward( delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'A' ) ) {
		cam->MoveSideways( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'D' ) ) {
		cam->MoveSideways( delta * speed );
	}

	if ( LVP::Input->IsKeyDown( 'Q' ) ) {
		cam->MoveVertical( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'E' ) ) {
		cam->MoveVertical( delta * speed );
	}

	cam->Look( LVP::Input->GetMouseDeltaX() * delta, -LVP::Input->GetMouseDeltaY() *delta );

	cam->UpdateFrustrum();
	instance->Transform = instance->Transform * mat4f::rotation( delta, { 0, 1, 0 } );

}

void MyGame::Render() {
	LVP::Graphics->ClearScreen( 0.2f, 0.5f, 0 );

	shader->Begin(*cam);

	shader->Render( instance );

	shader->End();
}

void MyGame::Resize() {
	cam->SetAspectRatio( LVP::App->Width / (float)LVP::App->Height );
}

MyGame::~MyGame() {
}