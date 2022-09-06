#include "SandBox.h"
#define PHYSAC_IMPLEMENTATION
#include "physac.h"

#define OFF_WHITE color {0.8f,0.8f,0.8f,1.0f}
#define OFF_RED color {0.8f,0.0f,0.0f,1.0f}
#define OFF_GREEN color{0.0f, 0.5f, 0.0f, 1.0f}

u32 ground;
u32 AABB1;
u32 AABB2;

f32 width;
f32 height;

#define BODIES_COUNT 5

color colors_array[] = { OFF_GREEN, GREEN, BLUE, CYAN, OFF_WHITE, OFF_GREEN };

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

	width = window::getInstance()->getWidth();
	height = window::getInstance()->getHeight();


	/*
	AABB1 = world->Add_Box_Body(pvec2(100, 500.0f), random(0.0f, 360.0f), random(50.0f, 100.0f), random(10.0f, 50.0f), 1.0f,
		1.0f, 0.5f, false);
	colors.push_back(OFF_GREEN);
	AABB2 = world->Add_Box_Body(pvec2(200, 500.0f), random(0.0f, 360.0f), random(50.0f, 100.0f), random(10.0f, 50.0f), 1.0f,
	world = std::make_shared<physicsWorld>();
		1.0f, 0.5f, false);
	colors.push_back(OFF_GREEN);
	*/
	/*

	ground = world->Add_Box_Body(windowCenter - pvec2(0.0f, 200.0f),
		0.0f,
		500.0f, 80.0f, 1.0f, 1.0f, 1.0f, true);
	*/
	
	//colors.push_back(OFF_GREEN);


	 // Initialize physics and default physics bodies
	InitPhysics();

	// Create floor rectangle physics body
	PhysicsBody floor = CreatePhysicsBodyRectangle(Vector2{ width / 2, height }, 500.0f, 100.0f, 10.0f);
	floor->enabled = false; // Disable body state to convert it to static (no dynamics, but collisions)

	// Create obstacle circle physics body
	PhysicsBody circle = CreatePhysicsBodyCircle(Vector2{ width / 2, height / 2 }, 45, 10);
	circle->enabled = false; // Disable body state to convert it to static (no dynamics, but collisions)
}

void SandBox::update()
{
	/*
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

		world->Add_Box_Body(position, 0.0f, w, h, mass, 1.0f, restitution, isStatic);
		colors.push_back(colors_array[random(0, 5)]);
	}


	world->update(Time::m_deltaTime);
	if (Input::isKeyDown(Key::Enter))
	{
		LOG_DEBUG("body count = {}", world->Get_Body_Count());
		LOG_INFO("frame rate = {}", Time::m_frameRate);
	}
	*/

	Vector2 mouse_pos = Vector2{ Input::getMousePos().x, Input::getMousePos().y };
	// Physics body creation inputs
	if (Input::isButtonDown(MouseButton::MouseButtonLeft))
		CreatePhysicsBodyPolygon(mouse_pos, random(20, 80), random(3, 8), 10);
	else if (Input::isButtonDown(MouseButton::MouseButtonRight))
		CreatePhysicsBodyCircle(mouse_pos, random(10, 45), 10);


	// Destroy falling physics bodies
	int bodiesCount = GetPhysicsBodiesCount();
	for (int i = bodiesCount - 1; i >= 0; i--)
	{
		PhysicsBody body = GetPhysicsBody(i);

		if ((body != NULL) && (body->position.y > height * 2))
			DestroyPhysicsBody(body);
	}
}

void SandBox::render()
{
	/*

	PhysicsBody* bodyA = world->Get_Body(AABB1);
	PhysicsBody* bodyB = world->Get_Body(AABB2);


	f32 forceMag = 1.0f;
	if (Input::isKeyPressed(Key::D))
		bodyA->move(pvec2(forceMag, 0.0f));
	if (Input::isKeyPressed(Key::A))
		bodyA->move(pvec2(-forceMag, 0.0f));
	if (Input::isKeyPressed(Key::W))
		bodyA->move(pvec2(0.0f, forceMag));
	if (Input::isKeyPressed(Key::S))
		bodyA->move(pvec2(0.0f, -forceMag));



	bodyA->rotate(Time::m_deltaTime * 50.0f);
	bodyB->rotate(Time::m_deltaTime * 100.0f);


	renderer::render_aabb(_AABB{ toVec2(bodyA->getAABB().center), toVec2(bodyA->getAABB().half_scale) },
		collisions::AABB_vs_AABB(bodyA->getAABB(), bodyB->getAABB()) ? RED : WHITE);
	renderer::render_aabb(_AABB{ toVec2(bodyB->getAABB().center), toVec2(bodyB->getAABB().half_scale) },
		collisions::AABB_vs_AABB(bodyA->getAABB(), bodyB->getAABB()) ? RED : WHITE);

	renderer::render_quad(toVec2(bodyA->getPos()), glm::vec2(bodyA->getWidth(), bodyA->getHeight()), bodyA->getRotation(), OFF_GREEN);
	renderer::render_quad(toVec2(bodyB->getPos()), glm::vec2(bodyB->getWidth(), bodyB->getHeight()), bodyB->getRotation(), OFF_GREEN);



	*/
	/*
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
	*/




	int bodiesCount = GetPhysicsBodiesCount();
	for (int i = 0; i < bodiesCount; i++)
	{
		PhysicsBody body = GetPhysicsBody(i);

		if (body != NULL)
		{
			int vertexCount = GetPhysicsShapeVerticesCount(i);
			for (int j = 0; j < vertexCount; j++)
			{
				// Get physics bodies shape vertices to draw lines
				// Note: GetPhysicsShapeVertex() already calculates rotation transformations
				Vector2 vertexA = GetPhysicsShapeVertex(body, j);

				int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
				Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

				renderer::render_wideline(glm::vec2(vertexA.x, vertexA.y), glm::vec2(vertexB.x, vertexB.y), GREEN);     // Draw a line between two vertex positions
			}
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
		renderer::renderer_begin();
		update();

		render();
		renderer::renderer_end(win->getRenderingWindow());
		Time::time_update_late();
	}
	ClosePhysics();       // Unitialize physics
}