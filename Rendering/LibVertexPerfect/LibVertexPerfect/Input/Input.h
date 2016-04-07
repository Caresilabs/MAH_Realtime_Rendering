#pragma once

class Input {
public:
	virtual float GetMouseX() = 0;
	virtual float GetMouseY() = 0;

	virtual float GetMouseDeltaX() = 0;
	virtual float GetMouseDeltaY() = 0;

	virtual bool IsKeyDown( char key ) = 0;

};