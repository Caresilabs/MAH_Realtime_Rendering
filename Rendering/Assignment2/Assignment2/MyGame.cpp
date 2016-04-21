#include "MyGame.h"

MyGame::MyGame() {
}

void MyGame::Start() {
	Shader = new PhongShader();

	VPtr<Mesh> City = new OBJMesh( "assets/sponza2/sponza.obj" );
	VPtr<Mesh> Hand = new OBJMesh( "assets/hand/hand.obj" );
	VPtr<Mesh> Sphere = new OBJMesh( "assets/tyre/tyre.obj" );

	SphereInstance = new MeshInstance( Sphere );
	SphereInstance->Position.y = 1.2f;

	CityInstance = new MeshInstance( City );
	CityInstance->Scale = { 0.01f, 0.01f, 0.01f };
	CityInstance->UpdateTransform();

	HandInstance = new MeshInstance( Hand );
	HandInstance->Scale = { 2, 2, 2 };
	HandInstance->Position.y = 2;

	Cam = new Camera( fPI / 4,				/*field-of-view*/
		(float)LVP::App->Width / LVP::App->Height,					/*aspect ratio*/
		.3f,								/*z-near plane (everything closer will be clipped/removed)*/
		500.0f );
	Cam->MoveTo( { 0, 0, 5 } );
}

void MyGame::Update( float delta ) {

	if ( LVP::Input->IsKeyDown( VK_ESCAPE ) )
		LVP::Input->SetCatchMouse( false );

	static float speed = 0;
	if ( LVP::Input->IsKeyDown( VK_SHIFT ) ) {
		speed = 10.f;
	} else {
		speed = 4.5f;
	}

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

	SphereInstance->Rotation.x -= delta * 2.f;
	SphereInstance->UpdateTransform();
}

void MyGame::Render() {
	LVP::Graphics->ClearScreen( 0.0f, 0.3f, 0.8f );

	Shader->Begin( *Cam );
	{
		HandInstance->Render( Shader );

		CityInstance->Render( Shader );

		SphereInstance->Render( Shader );
	}
	Shader->End();
}

void MyGame::Resize() {
	Cam->SetAspectRatio( LVP::App->Width / (float)LVP::App->Height );
}
