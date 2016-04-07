#pragma once

class ApplicationListener {
public:
	virtual void Start() = 0;

	virtual void Update(float delta) = 0;

	virtual void Render( ) = 0;

	virtual void Resize() = 0;
};