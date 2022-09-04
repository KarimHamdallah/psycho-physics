#pragma once
#include <snap_engine.h>
#include "psycho/psycho_physics.h"

class SandBox
{
public:
	SandBox();
	void init();
	void start();
	void update();
	void render();
	void shutdown();
	~SandBox();

	void run();


private:
	std::shared_ptr<window> win;

	// helper variables
	pvec2 windowCenter;

	// psycho
	std::vector<color> colors;
	std::shared_ptr<physicsWorld> world;
};