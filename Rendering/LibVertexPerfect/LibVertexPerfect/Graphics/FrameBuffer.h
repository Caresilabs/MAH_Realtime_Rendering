#pragma once

class FrameBuffer {
public:
	FrameBuffer( int width, int height, bool depth = true, bool colorView = true) : Width(width), Height(height), Depth(depth), Color(colorView) {
	}

	virtual void Bind() = 0;

	virtual void Unbind() = 0;

	virtual void Clear( float r, float g, float b ) = 0;

protected:
	int Width;
	int Height;
	bool Depth;
	bool Color;
};
