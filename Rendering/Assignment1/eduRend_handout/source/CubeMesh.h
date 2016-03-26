#pragma once
#include "Geometry.h"

class CubeMesh : public Geometry_t {
public:
	CubeMesh( ID3D11Device* device );

	void render( ID3D11DeviceContext* device_context ) const override;

private:
	// our local vertex and index arrays
	std::vector<vertex_t> vertices;
	std::vector<unsigned> indices;
	unsigned nbr_indices = 0;
};

