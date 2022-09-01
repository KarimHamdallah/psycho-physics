#pragma once
#include <snap_engine.h>
#include <psycho/psycho.h>

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
	u32 ground;
	u32 cursorBody;
};