#pragma once
//
//  material.h
//
//  CJG 2016, cjgribel@gmail.com
//

#include "../stdafx.h"
#include <D3D11.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Math/vec.h"

using namespace linalg;

struct vertex_t {
	vec3f Pos;
	vec3f Normal, Tangent, Binormal;
	vec2f TexCoord;
};

//
// Phong-esque material
//
struct material_t {
	//  Phong color components: ambient, diffuse & specular
	vec3f Ka = { 0, 0.5, 0 }, Kd = { 0, 0.5, 0 }, Ks = { 1, 1, 1 };

	float ReflectionValue = 0;

	std::string name;		// material name
	std::string map_Kd;		// file path
	std::string map_bump;	// file path
	std::string map_Ks;		// file path
	std::string map_mask;	// file path

							// device texture pointers
	ID3D11ShaderResourceView*	map_Kd_TexSRV = nullptr;
	ID3D11Resource*				map_Kd_Tex = nullptr;

	ID3D11ShaderResourceView*	map_Ks_TexSRV = nullptr;
	ID3D11Resource*				map_Ks_Tex = nullptr;

	ID3D11ShaderResourceView*	map_Normal_TexSRV = nullptr;
	ID3D11Resource*				map_Normal_Tex = nullptr;

	ID3D11ShaderResourceView*	map_Mask_TexSRV = nullptr;
	ID3D11Resource*				map_Mask_Tex = nullptr;

	~material_t() {
		SAFE_RELEASE( map_Kd_Tex );
		SAFE_RELEASE( map_Kd_TexSRV );

		SAFE_RELEASE( map_Ks_Tex );
		SAFE_RELEASE( map_Ks_TexSRV );

		SAFE_RELEASE( map_Normal_Tex );
		SAFE_RELEASE( map_Normal_TexSRV );

		SAFE_RELEASE( map_Mask_Tex );
		SAFE_RELEASE( map_Mask_TexSRV );
	}
};

static material_t default_mtl = material_t();

typedef std::unordered_map<std::string, material_t> mtl_hash_t;

struct triangle_t {
	unsigned vi[3];
};

struct quad_t_ {
	unsigned vi[4];
};

struct drawcall_t {
	std::string group_name;
	int mtl_index = -1;
	std::vector<triangle_t> tris;
	std::vector<quad_t_> quads;

	// make sortable
	bool operator < ( const drawcall_t& dc ) const {
		return mtl_index < dc.mtl_index;
	}
};

