#include "MyGame.h"
#include "ShadowMapShader.h"

MyGame::MyGame() {
}

void MyGame::Start() {
	VPtr<Mesh> City = new OBJMesh( "assets/sponza2/sponza.obj" );
	VPtr<Mesh> Hand = new OBJMesh( "assets/hand/hand.obj" );
	VPtr<Mesh> Sphere = new OBJMesh( "assets/sphere/sphere.obj" );

	// Sphere
	SphereInstance = new MeshInstance( Sphere );
	SphereInstance->Position = { 0.2f, 2.f, -0.2f };
	SphereInstance->Scale = { 0.1f, 0.1f, 0.1f };
	SphereInstance->UpdateTransform();

	// City 
	CityInstance = new MeshInstance( City );
	CityInstance->Scale = { 0.01f, 0.01f, 0.01f };
	CityInstance->UpdateTransform();

	// Hand
	HandInstance = new MeshInstance( Hand );
	HandInstance->Scale = { 2, 2, 2 };
	HandInstance->Position.y = 0.5f;
	HandInstance->MeshPtr->materials[1].ReflectionValue = 1.0f;
	HandInstance->MeshPtr->materials[2].ReflectionValue = .5f;


	// Shadow stuff
	ShadowMapBuffer = new DXFrameBuffer( LVP::App->Width , LVP::App->Height, true, false, true );
	ShadowCam = new Camera( 0, 0, 0.2f, 100.0f, false );
	ShadowCam->MoveTo( {4, 11, 1} );
	ShadowCam->Look( -3.14/2, -1.6f );
	ShadowShader = new ShadowMapShader();


	// Cameras and shaders
	Cam = new Camera( fPI / 4,				/*field-of-view*/
		(float)LVP::App->Width / LVP::App->Height,					/*aspect ratio*/
		.2f,								/*z-near plane (everything closer will be clipped/removed)*/
		100.0f, true );
	Cam->MoveTo( { 0, 0, 5 } );

	Shader = new PhongShader( ShadowMapBuffer, ShadowCam );

	ActiveCam = Cam;
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

	if ( LVP::Input->IsKeyDown( '9' ) ) {
		ActiveCam = ShadowCam;
	} else if ( LVP::Input->IsKeyDown( '0' ) ) {
		ActiveCam = Cam;
	}

	if ( LVP::Input->IsKeyDown( 'S' ) ) {
		ActiveCam->MoveForward( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'W' ) ) {
		ActiveCam->MoveForward( delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'A' ) ) {
		ActiveCam->MoveSideways( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'D' ) ) {
		ActiveCam->MoveSideways( delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'Q' ) ) {
		ActiveCam->MoveVertical( -delta * speed );
	}
	if ( LVP::Input->IsKeyDown( 'E' ) ) {
		ActiveCam->MoveVertical( delta * speed );
	}

	ActiveCam->Look( LVP::Input->GetMouseDeltaX() * delta, -LVP::Input->GetMouseDeltaY() *delta );
	
	Cam->UpdateFrustrum();
	ShadowCam->UpdateFrustrum();

	HandInstance->Rotation.y += delta * 0.7f;
	HandInstance->UpdateTransform();
}

void MyGame::Render() {
	LVP::Graphics->ClearScreen( 0.0f, 0.6f, 0.95f );

	// Render Target

	ShadowMapBuffer->Clear( 0, 0, 0 );
	ShadowMapBuffer->Bind();
	{
		ShadowShader->Begin( *ShadowCam );
		{
			RenderScene( ShadowShader );
		}
		ShadowShader->End();
	}
	ShadowMapBuffer->Unbind();

	
	Shader->Begin( *ActiveCam );
	{
		RenderScene( Shader );
	}
	Shader->End();


}

void MyGame::RenderScene( ShaderProgram& shader ) {
	HandInstance->Render( shader );
	CityInstance->Render( shader );
	SphereInstance->Render( shader );
}

void MyGame::Resize() {
	Cam->SetAspectRatio( LVP::App->Width / (float)LVP::App->Height );
}
