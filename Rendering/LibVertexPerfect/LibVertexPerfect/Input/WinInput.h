#pragma once

#include "Input.h"
#include <windows.h>

class WinInput : public Input {
public:
	WinInput();

	void ResetCursor( HWND window );

	void Update( HWND window );

	// Inherited via Input
	virtual float GetMouseX() override;
	virtual float GetMouseY() override;
	virtual float GetMouseDeltaX() override;
	virtual float GetMouseDeltaY() override;

	virtual void SetCatchMouse( bool val ) override;

	bool IsKeyDown( char key );

	bool Keys[256] = { 0 };

	float MousePosX;
	float MousePosY;

	float LastMousePosX;
	float LastMousePosY;

};

