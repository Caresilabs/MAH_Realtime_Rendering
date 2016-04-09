#pragma once

#include "ApplicationListener.h"
#include "LVP.h"
#include "PhongShader.h"
#include "Graphics/MeshInstance.h"

class MyGame : public ApplicationListener {
public:
	MyGame();

	// Inherited via ApplicationListener
	virtual void			Start() override;
	virtual void			Update( float delta ) override;
	virtual void			Render() override;
	virtual void			Resize() override;

private:

	VPtr<MeshInstance>		CityInstance;
	VPtr<MeshInstance>		HandInstance;
	VPtr<ShaderProgram>		Shader;
	VPtr<Camera>			Cam;

};

