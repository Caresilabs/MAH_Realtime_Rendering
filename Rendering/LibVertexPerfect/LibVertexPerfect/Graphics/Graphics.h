#pragma once

#include "../ApplicationListener.h"

enum class ShaderType {
	VERTEX, FRAGMENT
};

class Graphics {
public:
	virtual void Render( ApplicationListener* listener ) = 0;

	virtual void ClearScreen( float r, float g, float b ) = 0;
};