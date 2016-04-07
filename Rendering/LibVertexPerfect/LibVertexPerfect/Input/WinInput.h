#pragma once

#include "Input.h"

class WinInput : public Input {
public:
	WinInput();
	~WinInput();

	// Inherited via Input
	virtual float GetMouseX() override;
	virtual float GetMouseY() override;
	virtual float GetMouseDeltaX() override;
	virtual float GetMouseDeltaY() override;

	bool IsKeyDown( char key );

	bool Keys[256] = { 0 };
};

