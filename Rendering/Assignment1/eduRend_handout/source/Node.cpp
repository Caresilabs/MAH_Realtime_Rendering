#include "Node.h"

Node::Node( std::string name, Geometry_t * geometry ) : name(name), geometry(geometry), parent(nullptr), scale(1,1,1) {
}

void Node::calculateTransform( bool recursive ) {
	if ( parent != nullptr ) {
		transform = parent->transform * ( mat4f::translation( position ) * mat4f::rotation( rotation.z, rotation.y, rotation.x ) * mat4f::scaling( scale ) );
	} else {
		transform = mat4f::translation( position ) * mat4f::rotation( rotation.z, rotation.y, rotation.x ) * mat4f::scaling( scale );
	}

	if ( recursive ) {
		for each (auto node in nodes) {
			node.second->calculateTransform( recursive );
		}
	}
}

void Node::render( ID3D11DeviceContext * device_context, ID3D11Buffer* matrixBuffer, camera_t* camera ) const {
	if ( geometry != nullptr ) {
		geometry->MapMatrixBuffers( device_context, matrixBuffer, transform, camera->get_WorldToViewMatrix(), camera->get_ProjectionMatrix() );
		geometry->render( device_context );
	}

	for each (auto node in nodes) {
		node.second->render( device_context, matrixBuffer, camera );
	}
}

void Node::add( Node* node ) {
	nodes[node->name] = node;
	node->parent = this;
}

Node* Node::find( std::string name, bool recursive ) {
	Node* node = nullptr;

	auto search = nodes.find( name );
	if ( search != nodes.end() ) {
		node = search->second;
	}

	if ( node == nullptr && recursive ) {
		for each (auto loopNode in nodes) {
			node = loopNode.second->find( name, recursive );
			if ( node != nullptr )
				return node;
		}
	}
	return node;
}

mat4f Node::getWorldMatrix() const {
	return transform;
}

Node::~Node() {
}
