#include "MyGame.h"


MyGame::MyGame() {
}

void MyGame::Start() {
	Shader = new PhongShader();

	VPtr<Mesh> City = new OBJMesh( "../assets/carbody/carbody.obj" );
	VPtr<Mesh> Hand = new OBJMesh( "../assets/hand/hand.obj" );

	CityInstance = new MeshInstance( City );

	HandInstance = new MeshInstance( Hand );

	HandInstance->Scale = { 2, 2, 2 };
	HandInstance->Position.y = 3;

	Cam = new Camera( fPI / 4,				/*field-of-view*/
		(float)LVP::App->Width / LVP::App->Height,					/*aspect ratio*/
		.5f,								/*z-near plane (everything closer will be clipped/removed)*/
		500.0f );
	Cam->MoveTo( { 0, 0, 5 } );
}

void MyGame::Update( float delta ) {

	if ( LVP::Input->IsKeyDown( VK_ESCAPE ) )
		LVP::Input->SetCatchMouse( false );

	static float speed = 4.5f;

	if ( LVP::Input->IsKeyDown( 'S' ) ) {
		Cam->MoveForward( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'W' ) ) {
		Cam->MoveForward( delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'A' ) ) {
		Cam->MoveSideways( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'D' ) ) {
		Cam->MoveSideways( delta * speed );
	}

	if ( LVP::Input->IsKeyDown( 'Q' ) ) {
		Cam->MoveVertical( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'E' ) ) {
		Cam->MoveVertical( delta * speed );
	}

	Cam->Look( LVP::Input->GetMouseDeltaX() * delta, -LVP::Input->GetMouseDeltaY() *delta );
	Cam->UpdateFrustrum();


	HandInstance->Rotation.y += delta;
	HandInstance->UpdateTransform();
}

void MyGame::Render() {
	LVP::Graphics->ClearScreen( 0.2f, 0.5f, 0.3f );

	Shader->Begin( *Cam );

	HandInstance->Render( Shader );

	CityInstance->Render( Shader );

	Shader->End();
}

void MyGame::Resize() {
	Cam->SetAspectRatio( LVP::App->Width / (float)LVP::App->Height );
}
