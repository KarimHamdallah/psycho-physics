#include "SandBox.h"

// TODO::REMOVE
static glm::vec2 toVec2(const pvec2& v)
{
	return glm::vec2(v.x, v.y); 
}
static pvec2 toVec2(const glm::vec2& v)
{
	return pvec2(v.x, v.y);
}

#define OFF_WHITE color {0.8f,0.8f,0.8f,1.0f}
#define OFF_RED color {0.8f,0.0f,0.0f,1.0f}
#define OFF_GREEN color{0.0f, 0.5f, 0.0f, 1.0f}

color colors_array[] = { RED, GREEN, BLUE, CYAN, OFF_WHITE, OFF_RED };


SandBox::SandBox()
{
	init();
}

void SandBox::init()
{
	// init engine systems

	// init logger system
	Log::init();

	// init window
	win = std::make_shared<window>("sandbox", 800.0f, 600.0f);

	// init input system
	InputSystem inputsystem;
	inputsystem.init(win);

	// init render system
	renderer::renderer_init();

	// init time system
	Time::time_init(60.0f);

	// init physics system
	physicsWorld::physics_init();


	// init psycho physics
	psycho::psycho_init();
}

void SandBox::start()
{
	Input::show_cursor(true);
	windowCenter = pvec2(win->getWidth() * 0.5f, win->getHeight() * 0.5f);
	u32 width = win->getWidth();
	u32 height = win->getHeight();

	ground = pWorld::addBoxeBody(pvec2(windowCenter.x, windowCenter.y - 200.0f), 600.0f, 80.0f, 1.0f, 1.0f, true);
	colors.push_back(OFF_GREEN);
}

void SandBox::update()
{
	for (size_t i = 0; i < pWorld::getBodyCount(); i++)
	{
		pBody* b = pWorld::getBody(i);
		if (b->position.x > win->getWidth() || b->position.x < 0 || b->position.y > win->getHeight() || b->position.y < 0)
		{
			pWorld::removeBody(i);
			colors.erase(colors.begin() + i);
		}
	}
	if (Input::isButtonDown(MouseButton::MouseButtonLeft))
	{
		pvec2 position = toVec2(Input::getMousePos());
		f32 width = random(10.0f, 50.0f);
		f32 height = random(10.0f, 50.0f);
		pWorld::addBoxeBody(position, width, height, 1.0f, 0.5f, false);
		colors.push_back(colors_array[random(0, 5)]);
	}

	if (Input::isButtonDown(MouseButton::MouseButtonRight))
	{
		pvec2 position = toVec2(Input::getMousePos());
		f32 raduis = random(10.0f, 30.0f);
		pWorld::addCircleBody(position, raduis, 1.0f, 0.5f, false);
		colors.push_back(colors_array[random(0, 5)]);
	}

	pWorld::worldUpdate(Time::m_deltaTime, pvec2(0.0f, -150.0f));
}

void SandBox::render()
{
	for (size_t i = 0; i < pWorld::getBodyCount(); i++)
	{
		pBody* body = pWorld::getBody(i);

		if (body->shapeType == pBodyShape::Box)
		{
			renderer::render_quad(toVec2(body->position), glm::vec2(body->width, body->height), colors[i]);
		}
		else if (body->shapeType == pBodyShape::Circle)
		{
			renderer::render_smoothcircle(toVec2(body->position), body->diameter, colors[i], 0.1f);
		}
	}

	
	for (size_t i = 0; i < pWorld::contactPointsList.size(); i++)
	{
		renderer::render_quad(toVec2(pWorld::contactPointsList[i]), glm::vec2(5.0f), WHITE);
	}
}

void SandBox::shutdown()
{
	// free phsyics memory
	physicsWorld::free_memory();
	pWorld::free_memory();
}

SandBox::~SandBox()
{
	shutdown();
}

void SandBox::run()
{
	start();

	while (!win->shouldClose())
	{
		Time::time_update();
		win->update();
		update();

		renderer::renderer_begin();
		render();
		renderer::renderer_end(win->getRenderingWindow());
		//Time::time_update_late();
	}
}