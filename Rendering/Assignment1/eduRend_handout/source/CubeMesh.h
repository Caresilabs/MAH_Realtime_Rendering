#pragma once
#include "Geometry.h"

class CubeMesh : public Geometry_t {
public:

	CubeMesh( ID3D11Device* device );

	void render( ID3D11DeviceContext* device_context ) const override;

private:
	void CreateFace( vec3f p0, vec3f p1, vec3f p2, vec3f p3, vec3f normal );

	// our local vertex and index arrays
	std::vector<vertex_t> vertices;
	std::vector<unsigned> indices;
	unsigned nbr_indices = 0;
};

