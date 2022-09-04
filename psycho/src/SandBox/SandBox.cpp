#include "SandBox.h"

#define OFF_WHITE color {0.8f,0.8f,0.8f,1.0f}
#define OFF_RED color {0.8f,0.0f,0.0f,1.0f}
#define OFF_GREEN color{0.0f, 0.5f, 0.0f, 1.0f}

u32 ground;

#define BODIES_COUNT 5

color colors_array[] = { RED, GREEN, BLUE, CYAN, OFF_WHITE, OFF_RED };

glm::vec2 toVec2(const pvec2& vector)
{
	return glm::vec2(vector.x, vector.y);
}

pvec2 toVec2(const glm::vec2& vector)
{
	return pvec2(vector.x, vector.y);
}


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
}

void SandBox::start()
{
	windowCenter = pvec2(window::getInstance()->getWidth() * 0.5f, window::getInstance()->getHeight() * 0.5f);

	f32 width = window::getInstance()->getWidth();
	f32 height = window::getInstance()->getHeight();

	world = std::make_shared<physicsWorld>();

	ground = world->Add_Box_Body(windowCenter - pvec2(0.0f, 200.0f),
		0.0f,
		500.0f, 80.0f, 1.0f, 1.0f, 1.0f, true);
	
	colors.push_back(OFF_GREEN);
}

void SandBox::update()
{
	if (Input::isButtonDown(MouseButton::MouseButtonRight))
	{
		pvec2 position = toVec2(Input::getMousePos());
		f32 rotation = 0.0f;
		f32 w = random(20.0f, 50.0f);
		f32 h = random(20.0f, 50.0f);
		f32 raduis = random(10.0f, 50.0f);
		f32 mass = 1.0f;
		f32 restitution = random(0.1f, 0.9f);
		bool isStatic = false;

		world->Add_Circle_Body(position, rotation, raduis, mass, 1.0f, restitution, isStatic);
		colors.push_back(colors_array[random(0, 5)]);
	}

	if (Input::isButtonDown(MouseButton::MouseButtonLeft))
	{
		pvec2 position = toVec2(Input::getMousePos());
		f32 rotation = 0.0f;
		f32 w = random(20.0f, 50.0f);
		f32 h = random(20.0f, 50.0f);
		f32 raduis = random(10.0f, 50.0f);
		f32 mass = 1.0f;
		f32 restitution = random(0.1f, 0.9f);
		bool isStatic = false;

		world->Add_AABB_Body(position, w, h, mass, 1.0f, restitution, isStatic);
		colors.push_back(colors_array[random(0, 5)]);
	}


	world->update(Time::m_deltaTime);
}

void SandBox::render()
{
	/*
	f32 forceMag = 200.0f;
	if (Input::isKeyPressed(Key::D))
		body->addForce(pvec2(forceMag, 0.0f));
	if (Input::isKeyPressed(Key::A))
		body->addForce(pvec2(-forceMag, 0.0f));
	if (Input::isKeyPressed(Key::W))
		body->addForce(pvec2(0.0f, forceMag));
	if (Input::isKeyPressed(Key::S))
		body->addForce(pvec2(0.0f, -forceMag));
	*/

	for (size_t i = 0; i < world->Get_Body_Count(); i++)
	{
		PhysicsBody* Body = world->Get_Body(i);
		if (Body->getShape() == BodyShape::AABB)
			renderer::render_aabb(_AABB{ toVec2(Body->getAABB().center), toVec2(Body->getAABB().half_scale) }, colors[i]);
		else if (Body->getShape() == BodyShape::BOX)
		{
			renderer::render_quad(toVec2(Body->getPos()), glm::vec2(Body->getWidth(), Body->getHeight()), Body->getRotation(), colors[i]);
			//renderer::render_aabb(_AABB{ toVec2(Body->getAABB().center), toVec2(Body->getAABB().half_scale) }, color{1.0f, 1.0f, 1.0f, 0.5f});
		}
		else
		{
			renderer::render_smoothcircle(toVec2(Body->getPos()), Body->getRaduis() * 2.0f, colors[i], 0.1f);
		}
	}
}

void SandBox::shutdown()
{
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
		Time::time_update_late();
	}
}