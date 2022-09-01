#pragma once
#include <psycho/math/pvec2.h>
#include <vector>


struct pAABB
{
	pvec2 min;
	pvec2 max;
};

struct pRenderAABB
{
	pvec2 psoition;
	pvec2 half_scale;
};

enum class pBodyShape
{
	Circle = 0,
	Box = 1
};


class pBody
{
public:
	pvec2 position;
	pvec2 linearvelocity;
	f32 angularVelocity;
	f32 rotation;

	pvec2 force;


	f32 mass;
	f32 invMass;
	f32 restitution;
	f32 inertia;
	f32 invInertia;

	bool isStatic;

	f32 width;
	f32 height;
	std::vector<pvec2> vertices;
	std::vector<pvec2> transformed_vertices;
	bool transformUpdateRequired;
	f32 raduis;
	f32 diameter;

	pBodyShape shapeType;
	pAABB AABB;
	bool AABBUpdateRequired;

public:
	pBody(const pvec2& position, f32 mass, f32 restitution, bool isStatic, f32 raduis, f32 width, f32 height, pBodyShape shapeType)
		: position(position), mass(mass), restitution(restitution), isStatic(isStatic), raduis(raduis),
		  width(width), height(height), shapeType(shapeType), linearvelocity(pvec2(0.0f)), angularVelocity(0.0f), rotation(0.0f), force(pvec2(0.0f))
	{
		diameter = raduis * 2.0f;
		if (shapeType == pBodyShape::Box)
		{
			vertices = calculateBoxVerticesinlocalSpace();
		}

		// calculate inertia
		if (shapeType == pBodyShape::Box)
		{
			inertia = (1.0f / 12.0f) * mass * (width * width + height * height);
		}
		else if (shapeType == pBodyShape::Circle)
		{
			inertia = (1.0f / 2.0f) * mass * raduis * raduis;
		}

		if (!isStatic)
		{
			invMass = 1.0f / mass;
			invInertia = 1.0f / inertia;
		}
		else
		{
			invMass = 0.0f;
			invInertia = 0.0f;
		}

		transformUpdateRequired = true;
		AABBUpdateRequired = true;
		get_AABB();
	}
	~pBody();


	static pBody* creatCircleBody(const pvec2& position, f32 raduis, f32 mass, f32 restitution, f32 isStatic);
	static pBody* creatBoxBody(const pvec2& position, f32 width, f32 height, f32 mass, f32 restitution, f32 isStatic);

	void move(const pvec2& amount);
	void moveTo(const pvec2& new_position);
	void rotate(f32 amount);
	void set_angle(f32 angle);
	void set_scale(f32 width, f32 height);
	void set_raduis(f32 new_raduis);
	void set_position(f32 new_x, f32 new_y);
	void set_position(const pvec2& new_position);
	void set_isStatic(bool is_static);

	inline pvec2 get_pos() { return position; }
	inline f32 get_raduis() { return raduis; }
	inline f32 get_diameter() { return diameter; }
	inline f32 get_width() { return width; }
	inline f32 get_height() { return height; }
	inline f32 get_mass() { return mass; }
	inline f32 get_restitutuin() { return restitution; }
	inline f32 get_angle() { return rotation; }
	pAABB get_AABB();

	void addForce(const pvec2& force)
	{
		this->force += force;
	}

	std::vector<pvec2> getTransformedVertices();
	
	// TODO:: REMOVE
	static pRenderAABB getRenderAABB(pAABB* aabb);
private:
	// TODO:: Can be optimized by return scaled vertices by width and height in case of unchanged scale rather than scaling each frame
	static std::vector<pvec2> calculateBoxVerticesinlocalSpace();
};