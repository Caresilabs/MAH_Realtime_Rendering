#pragma once

#include "ApplicationListener.h"
#include "LVP.h"

#include "Graphics/Mesh.h"
#include "PhongShader.h"

class MyGame : public ApplicationListener {
public:
	MyGame();

	// Inherited via ApplicationListener
	virtual void			Start() override;
	virtual void			Update( float delta ) override;
	virtual void			Render() override;
	virtual void			Resize() override;

	VPtr<OBJMesh>			wheel;
	VPtr<MeshInstance>		instance;
	VPtr<ShaderProgram>		shader;
	VPtr<Camera>			camera;

};

