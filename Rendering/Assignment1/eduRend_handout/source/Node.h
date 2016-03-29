#pragma once
#include <map>
#include "Geometry.h"
#include "Camera.h"

class Node {
public:
							Node(std::string name, Geometry_t* geometry = nullptr);

	void					calculateTransform(bool recursive = true);

	void					render( ID3D11DeviceContext* device_context, ID3D11Buffer* matrixBuffer, camera_t* camera ) const;

	void					add( Node* node );

	Node*					find( std::string name, bool recursive = true );

	mat4f					getWorldMatrix() const;

	virtual					~Node();

	std::unordered_map<std::string, Node*>	nodes;
	vec3f					position;
	vec3f					rotation;
	vec3f					scale;

private:
	std::string				name;
	Geometry_t*				geometry;
	Node*					parent;
	mat4f					transform;

};

