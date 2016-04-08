#include "MyGame.h"

MyGame::MyGame() {
}

void MyGame::Start() {
	wheel = new OBJMesh( "../assets/sphere/sphere.obj" );	// new OBJMesh( "../assets/tyre/Tyre.obj" );
	//wheel = new CubeMesh();

	instance = new MeshInstance( wheel );
	shader = new PhongShader();

	camera = new Camera( fPI / 4,				/*field-of-view*/
		(float)LVP::App->Width / LVP::App->Height,					/*aspect ratio*/
		.5f,								/*z-near plane (everything closer will be clipped/removed)*/
		500.0f );
	camera->MoveTo( { 0, 0, 5 } );
}

void MyGame::Update( float delta ) {

	if ( LVP::Input->IsKeyDown( VK_ESCAPE ) )
		LVP::Input->SetCatchMouse( false );

	static float speed = 3;

	if ( LVP::Input->IsKeyDown( 'S' ) ) {
		camera->MoveForward( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'W' ) ) {
		camera->MoveForward( delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'A' ) ) {
		camera->MoveSideways( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'D' ) ) {
		camera->MoveSideways( delta * speed );
	}

	if ( LVP::Input->IsKeyDown( 'Q' ) ) {
		camera->MoveVertical( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'E' ) ) {
		camera->MoveVertical( delta * speed );
	}

	camera->Look( LVP::Input->GetMouseDeltaX() * delta, -LVP::Input->GetMouseDeltaY() *delta );
	camera->UpdateFrustrum();


	instance->Transform = instance->Transform * mat4f::rotation( delta, { 0, 1, 0 } );

}

void MyGame::Render() {
	LVP::Graphics->ClearScreen( 0.2f, 0.5f, 0.3f );

	shader->Begin( *camera );

	shader->Render( instance );

	shader->End();
}

void MyGame::Resize() {
	camera->SetAspectRatio( LVP::App->Width / (float)LVP::App->Height );
}
