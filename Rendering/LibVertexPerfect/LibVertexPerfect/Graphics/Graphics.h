#pragma once

#include "../ApplicationListener.h"

enum class ShaderType {
	VERTEX, FRAGMENT
};

class Graphics {
public:
	virtual void Render( ApplicationListener* listener ) = 0;

	//virtual void
};