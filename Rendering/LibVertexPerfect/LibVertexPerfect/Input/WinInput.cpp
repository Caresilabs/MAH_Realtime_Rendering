#include "stdafx.h"
#include "WinInput.h"


WinInput::WinInput() {
}


WinInput::~WinInput() {
}

float WinInput::GetMouseX() {
	return 0.0f;
}

float WinInput::GetMouseY() {
	return 0.0f;
}

float WinInput::GetMouseDeltaX() {
	return 0.0f;
}

float WinInput::GetMouseDeltaY() {
	return 0.0f;
}

bool WinInput::IsKeyDown( char key ) {
	return Keys[key];
}

