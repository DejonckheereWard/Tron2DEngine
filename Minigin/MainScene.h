#pragma once
#include "Scene.h"

class MainScene final: public Engine::Scene
{
public:
	MainScene(): Scene("MainScene") {};

	// Inherited via Scene
	virtual void OnCreate() override;
	virtual void OnImGui() override {};

};

