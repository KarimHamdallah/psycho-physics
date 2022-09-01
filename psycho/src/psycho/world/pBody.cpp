#include "pBody.h"
#include <psycho/math/pmath.h>
#include <psycho/world/pWorld.h>
#include <psycho/logger/passerts.h>

pBody::~pBody()
{
	if(vertices.size())
		vertices.clear();
	if (transformed_vertices.size())
		transformed_vertices.clear();
}

pBody* pBody::creatCircleBody(const pvec2& position, f32 raduis, f32 mass, f32 restitution, f32 isStatic)
{
	PSYCHO_ASSERT_MSG(raduis > 0.0f, "raduis is a minus value or equal to zero");

	restitution = pmath::clampf(restitution, 0.0f, 1.0f);

	return new pBody(position, mass, restitution, isStatic, raduis, 0.0f, 0.0f, pBodyShape::Circle);
}

pBody* pBody::creatBoxBody(const pvec2& position, f32 width, f32 height, f32 mass, f32 restitution, f32 isStatic)
{
	PSYCHO_ASSERT_MSG(width > 0.0f, "width is a minus value or equal to zero");
	PSYCHO_ASSERT_MSG(height > 0.0f, "height is a minus value or equal to zero");

	restitution = pmath::clampf(restitution, 0.0f, 1.0f);

	return new pBody(position, mass, restitution, isStatic, 0.0f, width, height, pBodyShape::Box);
}

void pBody::move(const pvec2 & amount)
{
	this->position += amount;
	transformUpdateRequired = true;
	AABBUpdateRequired = true;
}

void pBody::moveTo(const pvec2 & new_position)
{
	this->position = new_position;
	transformUpdateRequired = true;
	AABBUpdateRequired = true;
}

void pBody::rotate(f32 amount)
{
	this->rotation += amount;
	transformUpdateRequired = true;
	AABBUpdateRequired = true;
}

void pBody::set_angle(f32 angle)
{
	rotation = angle;
	transformUpdateRequired = true;
	AABBUpdateRequired = true;
}

void pBody::set_scale(f32 new_width, f32 new_height)
{
	this->width = new_width;
	this->height = new_height;
	transformUpdateRequired = true;
	AABBUpdateRequired = true;
}

void pBody::set_raduis(f32 new_raduis)
{
	raduis = new_raduis;
	diameter = raduis * 2.0f;
	AABBUpdateRequired = true;
}

void pBody::set_position(f32 new_x, f32 new_y)
{
	position.x = new_x;
	position.y = new_y;
	transformUpdateRequired = true;
	AABBUpdateRequired = true;
}

void pBody::set_position(const pvec2 & new_position)
{
	position = new_position;
	transformUpdateRequired = true;
	AABBUpdateRequired = true;
}

void pBody::set_isStatic(bool is_static)
{
	this->isStatic = is_static;
	if (!isStatic)
		invMass = 1.0f / mass;
	else
		invMass = 0.0f;
}

pAABB pBody::get_AABB()
{
	if (AABBUpdateRequired)
	{
		if (shapeType == pBodyShape::Circle)
		{
			AABB.min = pmath::sub(position, pvec2(raduis));
			AABB.max = pmath::add(position, pvec2(raduis));
		}
		else if (shapeType == pBodyShape::Box)
		{
			f32 minx = FLT_MAX, miny = FLT_MAX, maxx = FLT_MIN, maxy = FLT_MIN;
			std::vector<pvec2> v = getTransformedVertices();

			for (size_t i = 0; i < 4; i++)
			{
				if (v[i].x < minx) { minx = v[i].x; }
				if (v[i].x > maxx) { maxx = v[i].x; }
				if (v[i].y < miny) { miny = v[i].y; }
				if (v[i].y > maxy) { maxy = v[i].y; }
			}

			AABB.min = pvec2(minx, miny);
			AABB.max = pvec2(maxx, maxy);
		}
	}
	AABBUpdateRequired = false;
	return AABB;
}

std::vector<pvec2> pBody::getTransformedVertices()
{
	if (transformUpdateRequired)
	{
		transformed_vertices.clear();
		for (size_t i = 0; i < vertices.size(); i++)
		{
			// translate vertex
			pvec2 vertex = vertices[i];
			pmath::transformVector(vertex, position, pvec2(width, height), rotation);
			transformed_vertices.push_back(vertex);
		}

		//transformUpdateRequired = false;
	}
	return transformed_vertices;
}

std::vector<pvec2> pBody::calculateBoxVerticesinlocalSpace()
{
	std::vector<pvec2> result;
	result.push_back(pvec2(-0.5f, -0.5f));
	result.push_back(pvec2(-0.5f, 0.5f));
	result.push_back(pvec2(0.5f, 0.5f));
	result.push_back(pvec2(0.5f, -0.5f));
	
	return result;
}

pRenderAABB pBody::getRenderAABB(pAABB * aabb)
{
	pRenderAABB result;
	
	f32 width = aabb->max.x - aabb->min.x;
	f32 height = aabb->max.y - aabb->min.y;
	result.half_scale = pvec2(width * 0.5f, height * 0.5f);
	result.psoition = pmath::add(aabb->min, result.half_scale);
	
	return result;
}