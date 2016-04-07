#pragma once

#include "ApplicationListener.h"
#include "LVP.h"

#include "Graphics/Mesh.h"
#include "Graphics/Shaders/ShaderProgram.h"

class MyGame :
	public ApplicationListener {
public:
	MyGame();
	~MyGame();

	// Inherited via ApplicationListener
	virtual void Start() override;
	virtual void Update( float delta ) override;
	virtual void Render() override;

	Mesh* wheel;
	MeshInstance* instance;
	ShaderProgram* shader;
	Camera* cam;
};

