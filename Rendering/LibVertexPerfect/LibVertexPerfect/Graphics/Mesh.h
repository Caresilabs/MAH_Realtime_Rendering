#pragma once

#include <D3D11.h>

class Mesh {
public:

protected:

	// pointers to device vertex & index arrays
	ID3D11Buffer* vertex_buffer = nullptr;
	ID3D11Buffer* index_buffer = nullptr;
};
