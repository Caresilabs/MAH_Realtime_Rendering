#include "stdafx.h"
#include "WinInput.h"

WinInput::WinInput() : LastMousePosX( -1 ), LastMousePosY( -1 ), MousePosX( -1 ), MousePosY( -1 ) {
}

void WinInput::Update( HWND window ) {
	if ( CatchMouse ) {


		POINT Point = { 0 };
		if ( GetCursorPos( &Point ) ) {
			if ( ScreenToClient( window, &Point ) ) {
				MousePosX = Point.x;
				MousePosY = Point.y;
			}
		}

		ResetCursor( window );


		/*if ( LastMousePosX == -1 )
			LastMousePosX = MousePosX;
		if ( LastMousePosY == -1 )
			LastMousePosY = MousePosY;*/


	}
}

void WinInput::ResetCursor( HWND window ) {
	POINT Point = { LVP::App->Width / 2, LVP::App->Height / 2 };
	LastMousePosX = Point.x;
	LastMousePosY = Point.y;
	if ( ClientToScreen( window, &Point ) ) {
		SetCursorPos( Point.x, Point.y );
	}
}

float WinInput::GetMouseX() {
	return MousePosX;
}

float WinInput::GetMouseY() {
	return MousePosY;
}

float WinInput::GetMouseDeltaX() {
	return LastMousePosX == -1 ? 0 : (MousePosX - LastMousePosX);
}

float WinInput::GetMouseDeltaY() {
	return LastMousePosY == -1 ? 0 : (MousePosY - LastMousePosY);
}

bool WinInput::IsKeyDown( char key ) {
	return Keys[key];
}

void WinInput::SetCatchMouse( bool val ) {
	if ( val == CatchMouse )
		return;

	CatchMouse = val;

	if ( CatchMouse ) {
		ShowCursor( FALSE );
	} else {
		ShowCursor( TRUE );
	}

}

