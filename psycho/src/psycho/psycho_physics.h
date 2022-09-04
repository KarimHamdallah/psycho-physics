#pragma once
#include <vector>
#include <memory>
#include <string>

//////////////////////////////////////////////// defines //////////////////////////////////////////////
typedef float f32;
typedef unsigned int u32;

static std::string ErrorCollector;

/////////////////////////////////////////////// math /////////////////////////////////////////////////
#define PI 3.14159265359
#define TO_RAD(degree) degree * 0.01745329251

class pvec2
{
public:
	union
	{
		struct { f32 x, y; };
		f32 arr[2];
	};


	pvec2() { x = 0.0f; y = 0.0f; }
	pvec2(f32 value) { this->x = value; this->y = value; }
	pvec2(f32 x, f32 y) { this->x = x; this->y = y; }
	pvec2(const pvec2& other) { x = other.x; y = other.y; }


	// operators
	f32 operator [] (u32 index) { return arr[index]; }
	friend pvec2 operator + (const pvec2& right, const pvec2& left) { return pvec2(right.x + left.x, right.y + left.y); }
	friend pvec2 operator - (const pvec2& right, const pvec2& left) { return pvec2(right.x - left.x, right.y - left.y); }
	friend pvec2 operator * (const pvec2& right, const pvec2& left) { return pvec2(right.x * left.x, right.y * left.y); }
	friend pvec2 operator / (const pvec2& right, const pvec2& left) { return pvec2(right.x / left.x, right.y / left.y); }

	friend pvec2 operator + (const pvec2& right, f32 left) { return pvec2(right.x + left, right.y + left); }
	friend pvec2 operator - (const pvec2& right, f32 left) { return pvec2(right.x - left, right.y - left); }
	friend pvec2 operator * (const pvec2& right, f32 left) { return pvec2(right.x * left, right.y * left); }
	friend pvec2 operator / (const pvec2& right, f32 left) { return pvec2(right.x / left, right.y / left); }



	pvec2 operator += (const pvec2& other) { this->x += other.x; this->y += other.y; return *this; }
	pvec2 operator -= (const pvec2& other) { this->x -= other.x; this->y -= other.y; return *this; }
	pvec2 operator *= (const pvec2& other) { this->x *= other.x; this->y *= other.y; return *this; }
	pvec2 operator /= (const pvec2& other) { this->x /= other.x; this->y /= other.y; return *this; }

	pvec2 operator += (f32 other) { this->x += other; this->y += other; return *this; }
	pvec2 operator -= (f32 other) { this->x -= other; this->y -= other; return *this; }
	pvec2 operator *= (f32 other) { this->x *= other; this->y *= other; return *this; }
	pvec2 operator /= (f32 other) { this->x /= other; this->y /= other; return *this; }

	//friend bool operator == (const pvec2& left, const pvec2& right) { return left->x == right.x && left.y == right.y; }
};

inline f32 length(const pvec2& v) { return sqrtf(v.x * v.x + v.y * v.y); }

inline f32 length_sqrd(const pvec2& v) { return v.x * v.x + v.y * v.y; }

inline f32 distance(const pvec2& v1, const pvec2& v2)
{
	f32 dx = v1.x - v2.x;
	f32 dy = v1.y - v2.y;
	return sqrtf(dx * dx + dy * dy);
}

inline f32 distance_sqrd(const pvec2& v1, const pvec2& v2)
{
	f32 dx = v1.x - v2.x;
	f32 dy = v1.y - v2.y;
	return dx * dx + dy * dy;
}

inline pvec2 normalize(const pvec2& v)
{
	f32 mag = length(v);
	if (mag != 0.0f)
		return pvec2(v.x / mag, v.y / mag);
	else
		return pvec2(0.0f, 0.0f);
}

inline f32 dot(const pvec2& v1, const pvec2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

inline f32 cross(const pvec2& v1, const pvec2& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

inline f32 clampf(f32 value, f32 min, f32 max)
{
	if (min > max)
		ErrorCollector = "clampf:: min value = " + std::to_string(min) + " is larger than max value = " + std::to_string(max) + "\n";

	return std::max(min, std::min(max, value));
}

inline u32 clamp(u32 value, u32 min, u32 max)
{
	if (min > max)
		ErrorCollector = "clamp:: min value = " + std::to_string(min) + " is larger than max value = " + std::to_string(max) + "\n";

	return std::max(min, std::min(max, value));
}

inline pvec2 transform_vector(const pvec2& vector_to_transform, const pvec2& translation, const pvec2& scale, f32 rotation)
{
	pvec2 vector = vector_to_transform;

	rotation = TO_RAD(rotation);
	// SRT

	// scale
	vector.x *= scale.x;
	vector.y *= scale.y;
	// roatate
	// scaled vector
	f32 scaled_x = vector.x;
	f32 scaled_y = vector.y;
	vector.x = cosf(rotation) * scaled_x - sinf(rotation) * scaled_y;
	vector.y = sinf(rotation) * scaled_x + cosf(rotation) * scaled_y;
	// translate
	vector.x += translation.x;
	vector.y += translation.y;

	return vector;
}

inline std::vector<pvec2> Transform_vertices(const std::vector<pvec2>& vertices, const pvec2& translation, const pvec2& scale, f32 rotation)
{
	std::vector<pvec2> result;
	for (size_t i = 0; i < vertices.size(); i++)
		result.push_back(transform_vector(vertices[i], translation, scale, rotation));
	return result;
}

////////////////////////////////////////////// collisions //////////////////////////////////////////////
struct AABB
{
	pvec2 center;
	pvec2 half_scale;
	pvec2 min;
	pvec2 max;
};

class collisions
{
public:
	// AABB vs AABB
	// normal vector is in (a -> b) direction, to resolve subtract from (a) center or add to (b) center
	static bool AABB_vs_AABB(const AABB& a, const AABB& b, pvec2& normal, f32& depth)
	{
		normal = pvec2(0.0f);
		depth = 0.0f;

		AABB minkowski_aabb = minkowski_difference(a, b);
		
		pvec2 min = minkowski_aabb.min;
		pvec2 max = minkowski_aabb.max;

		if (point_vs_aabb(pvec2(0.0f), min, max))
		{
			f32 min_dist = FLT_MAX;

			if (-min.x < min_dist)
			{
				// left
				min_dist = fabs(min.x);
				normal = pvec2(-1.0f, 0.0f);
			}
			if (max.x < min_dist)
			{
				// right
				min_dist = fabs(max.x);
				normal = pvec2(1.0f, 0.0f);
			}
			if (-min.y < min_dist)
			{
				// down
				min_dist = fabs(min.y);
				normal = pvec2(0.0f, -1.0f);
			}
			if (max.y < min_dist)
			{
				// up
				min_dist = fabs(max.y);
				normal = pvec2(0.0f, 1.0f);
			}

			depth = min_dist;
			return true;
		}

		return false;
	}
	// AABB vs Circle : (normal is in Circle >> AABB direction)
	static bool AABB_vs_Circle(const AABB& aabb, const pvec2& circlePosition, f32 raduis, pvec2& normal, f32& depth)
	{
		// get difference vector between both centers
		pvec2 dif = circlePosition - aabb.center;
		pvec2 clamped = pvec2(clampf(dif.x, -aabb.half_scale.x, aabb.half_scale.x), clampf(dif.y, -aabb.half_scale.y, aabb.half_scale.y));

		// add clamped value to AABB_center and we get the value of box closest to circle
		pvec2 closest = aabb.center + clamped;

		// retrieve vector between center circle and closest point AABB and check if length <= radius
		dif = closest - circlePosition;

		f32 dif_length = length(dif);

		if (dif_length <= raduis)
		{
			// normalize diff vector
			normal = dif_length == 0.0f ? pvec2(0.0f, 0.0f) : dif / dif_length;
			depth = raduis - dif_length;

			return true;
		}
		else
		{
			normal = pvec2(0.0f);
			depth = 0.0f;
			return false;
		}
	}
	// AABB vs BOX :: (SAT) : (normal is in A >> B direction)
	static bool AABB_vs_Polygon(const AABB& aabb,
		const pvec2& position, pvec2* vertices, u32 vertices_count, pvec2& normal, f32& depth)
	{
		std::vector<pvec2> verticesA =
		{
			pvec2(aabb.center.x - aabb.half_scale.x, aabb.center.y + aabb.half_scale.y),
			pvec2(aabb.center.x + aabb.half_scale.x, aabb.center.y + aabb.half_scale.y),
			pvec2(aabb.center.x + aabb.half_scale.x, aabb.center.y - aabb.half_scale.y),
			pvec2(aabb.center.x - aabb.half_scale.x, aabb.center.y - aabb.half_scale.y)	
		};

		return Polygon_vs_Polygon(aabb.center, &verticesA[0], verticesA.size(), position, vertices, vertices_count, normal, depth);
	}

	// BOX vs BOX (SAT) : (normal is in A >> B direction)
	static bool Polygon_vs_Polygon(const pvec2& positionA, pvec2* verticesA, u32 verticesA_count,
		const pvec2& positionB, pvec2* verticesB, u32 verticesB_count, pvec2& normal, f32& depth)
	{
		// SAT
		normal = pvec2(0.0f);
		depth = FLT_MAX;

		// check all axies of polygonA
		for (size_t i = 0; i < verticesA_count; i++)
		{
			// get axis
			pvec2 va = verticesA[i];
			pvec2 vb = verticesA[(i + 1) % verticesA_count];

			pvec2 side = vb - va;
			pvec2 axis = pvec2(-side.y, side.x);
			axis = normalize(axis);

			// calculate min and max
			f32 minA = FLT_MAX;
			f32 maxA = FLT_MIN;
			f32 minB = FLT_MAX;
			f32 maxB = FLT_MIN;

			// projection of A vertices into axis
			for (size_t j = 0; j < verticesA_count; j++)
			{
				f32 proj = dot(axis, verticesA[j]);

				if (minA > proj)
					minA = proj;
				if (maxA < proj)
					maxA = proj;
			}

			// projection of B vertices into axis
			for (size_t j = 0; j < verticesB_count; j++)
			{
				f32 proj = dot(axis, verticesB[j]);

				if (minB > proj)
					minB = proj;
				if (maxB < proj)
					maxB = proj;
			}

			// check for gab
			if (minB >= maxA || minA >= maxB)
			{
				normal = pvec2(0.0f);
				depth = 0.0f; // avoid return INFINITY value
				return false;
			}

			// calculate depth and normal in this axis
			f32 axis_depth = std::min(maxB - minA, maxA - minB);
			if (axis_depth < depth)
			{
				depth = axis_depth;
				normal = axis;
			}
		}

		// check all axies of polygonB
		for (size_t i = 0; i < verticesB_count; i++)
		{
			// get axis
			pvec2 va = verticesB[i];
			pvec2 vb = verticesB[(i + 1) % verticesB_count];

			pvec2 side = vb - va;
			pvec2 axis = pvec2(-side.y, side.x);
			axis = normalize(axis);

			// calculate min and max
			f32 minA = FLT_MAX;
			f32 maxA = FLT_MIN;
			f32 minB = FLT_MAX;
			f32 maxB = FLT_MIN;

			// projection of A vertices into axis
			for (size_t j = 0; j < verticesA_count; j++)
			{
				f32 proj = dot(axis, verticesA[j]);

				if (minA > proj)
					minA = proj;
				if (maxA < proj)
					maxA = proj;
			}

			// projection of B vertices into axis
			for (size_t j = 0; j < verticesB_count; j++)
			{
				f32 proj = dot(axis, verticesB[j]);

				if (minB > proj)
					minB = proj;
				if (maxB < proj)
					maxB = proj;
			}

			// check for gab
			if (minB >= maxA || minA >= maxB)
			{
				normal = pvec2(0.0f);
				depth = 0.0f; // avoid return INFINITY value
				return false;
			}

			// calculate depth and normal in this axis
			f32 axis_depth = std::min(maxB - minA, maxA - minB);
			if (axis_depth < depth)
			{
				depth = axis_depth;
				normal = axis;
			}
		}

		// correct normal to get it in A >> B direction
		if (dot(normal, positionB - positionA) < 0.0f) // if normal is directed from B >> A , reflect it
			normal *= -1.0f;

		return true;
	}
	
	// BOX vs Circle : (normal is in circle >> Box direction)
	static bool Polygon_vs_Circle(const pvec2& position, pvec2* vertices, u32 vertices_count,
		const pvec2& circlePosition, f32 raduis, pvec2& normal, f32& depth)
	{
		normal = pvec2(0.0f);
		depth = FLT_MAX;


		for (size_t i = 0; i < vertices_count; i++)
		{
			pvec2 va = vertices[i];
			pvec2 vb = vertices[(i + 1) % vertices_count];

			pvec2 side = vb - va;
			pvec2 axis = pvec2(-side.y, side.x);
			axis = normalize(axis);

			// calculate min and max
			f32 minA = FLT_MAX;
			f32 maxA = FLT_MIN;
			f32 minB = FLT_MAX;
			f32 maxB = FLT_MIN;

			for (size_t j = 0; j < vertices_count; j++)
			{
				f32 proj = dot(vertices[j], axis);
				if (minA > proj)
					minA = proj;
				if (maxA < proj)
					maxA = proj;
			}

			pvec2 min_vector = circlePosition - (axis * raduis);
			pvec2 max_vector = circlePosition + (axis * raduis);
			minB = dot(min_vector, axis);
			maxB = dot(max_vector, axis);

			if (minB > maxB)
			{
				f32 temp = maxB;
				maxB = minB;
				minB = temp;
			}

			// check for gap
			if (minB >= maxA || minA > maxB)
			{
				normal = pvec2(0.0f);
				depth = 0.0f;
				return false;
			}

			// calculate depth and normal in this axis
			f32 axis_depth = std::min(maxB - minA, maxA - minB);
			if (axis_depth < depth)
			{
				depth = axis_depth;
				normal = axis;
			}
		}

		// get closest point to circle position
		u32 closestPointIndex = 0;
		f32 min_lengthSqrd = FLT_MAX;
		for (size_t i = 0; i < vertices_count; i++)
		{
			f32 lsqrd = length_sqrd(vertices[i] - circlePosition);
			if (min_lengthSqrd > lsqrd)
			{
				min_lengthSqrd = lsqrd;
				closestPointIndex = i;
			}
		}

		pvec2 axis = vertices[closestPointIndex] - circlePosition;
		axis = normalize(axis);

		// calculate min and max
		f32 minA = FLT_MAX;
		f32 maxA = FLT_MIN;
		f32 minB = FLT_MAX;
		f32 maxB = FLT_MIN;

		for (size_t j = 0; j < vertices_count; j++)
		{
			f32 proj = dot(vertices[j], axis);
			if (minA > proj)
				minA = proj;
			if (maxA < proj)
				maxA = proj;
		}

		pvec2 min_vector = circlePosition - (axis * raduis);
		pvec2 max_vector = circlePosition + (axis * raduis);
		minB = dot(min_vector, axis);
		maxB = dot(max_vector, axis);

		if (minB > maxB)
		{
			f32 temp = maxB;
			maxB = minB;
			minB = temp;
		}

		// check for gap
		if (minB >= maxA || minA > maxB)
		{
			normal = pvec2(0.0f);
			depth = 0.0f;
			return false;
		}

		// calculate depth and normal in this axis
		f32 axis_depth = std::min(maxB - minA, maxA - minB);
		if (axis_depth < depth)
		{
			depth = axis_depth;
			normal = axis;
		}


		// correct normal to get it in circle >> Box direction
		if (dot(normal, position - circlePosition) < 0.0f) // if normal is directed from Box >> Circle , reflect it
			normal *= -1.0f;

		return true;
	}
	// Circle vs Circle >> return normal from A to B >> to resolve subtract from A and add to B
	static bool Circle_vs_Circle(const pvec2& centerA, f32 raduisA, const pvec2& centerB, f32 raduisB, pvec2& normal, f32& depth)
	{
		normal = pvec2(0.0f);
		depth = 0.0f;

		pvec2 dist_vector = centerB - centerA;
		f32 dist = length(dist_vector);
		f32 radii = raduisA + raduisB;
		

		if (dist >= radii)
			return false;


		// normalize dist_vector
		normal =  dist == 0.0f ? pvec2(0.0f) :  pvec2(dist_vector.x / dist, dist_vector.y / dist);
		depth = radii - dist;
		
		return true;
	}



private:
	static bool point_vs_aabb(const pvec2& point, const pvec2& min, const pvec2& max)
	{
		return point.x >= min.x &&
			point.x <= max.x &&
			point.y >= min.y &&
			point.y <= max.y;
	}
	static AABB minkowski_difference(const AABB& a, const AABB& b)
	{
		AABB result;
		result.center = a.center - b.center;
		result.half_scale = a.half_scale + b.half_scale;
		result.min = result.center - result.half_scale;
		result.max = result.center + result.half_scale;

		return result;
	}
};

////////////////////////////////////////////// Body ////////////////////////////////////////////////////
enum class BodyShape
{
	AABB = 0,
	BOX = 1,
	CIRCLE = 2,
};

class PhysicsBody
{
public:
	pvec2 position;
	f32 rotation;
	f32 inertia;
	u32 width;  // for box Body (AABB/OBB)
	u32 height; // for box Body (AABB/OBB)
	u32 raduis; // for circle Body
	std::vector<pvec2> vertices; // for convex polygon Body
	std::vector<pvec2> transformed_vertices; // for convex polygon Body after rotation

	// physical properties
	pvec2 linearVelocity;
	f32 rotationalVelocity;
	pvec2 force;
	f32 density;
	f32 restitution;
	f32 mass;
	f32 invMass;
	AABB aabb;
	BodyShape shape;
	bool isStatic;

	bool updateTransformations;
	bool updateAABB;

	PhysicsBody()
		: position(pvec2(0.0f)), rotation(0.0f), inertia(0.0f), width(0.0f), height(0.0f), raduis(0.0f), linearVelocity(pvec2(0.0f)),
		rotationalVelocity(0.0f), force(0.0f),
		density(0.0f), restitution(0.0f), mass(0.0f), invMass(0.0f), updateTransformations(false), updateAABB(false), isStatic(false)
	{}




	pvec2 getPos() { return position; }
	f32 getRotation() { return rotation; }
	f32 getWidth() { return width; }
	f32 getHeight() { return height; }
	f32 getRaduis() { return raduis; }
	f32 getRestitution() { return restitution; }
	f32 getMass() { return mass; }
	BodyShape getShape() { return shape; }

	// only use with Box Body
	std::vector<pvec2> getTransformedVertices()
	{
		if (updateTransformations)
		{
			transformed_vertices.clear();
			transformed_vertices = Transform_vertices(vertices, position, pvec2(width, height), rotation);

			updateTransformations = false;
		}
		return transformed_vertices;
	}

	AABB getAABB()
	{
		if (updateAABB)
		{
			aabb.center = position;
			
			if (shape == BodyShape::AABB)
			{
				aabb.half_scale = pvec2(width * 0.5f, height * 0.5f);
				aabb.min = aabb.center - aabb.half_scale;
				aabb.max = aabb.center + aabb.half_scale;
			}
			else if (shape == BodyShape::CIRCLE)
			{
				aabb.half_scale = pvec2(raduis);
				aabb.min = aabb.center - aabb.half_scale;
				aabb.max = aabb.center + aabb.half_scale;
			}
			else if (shape == BodyShape::BOX)
			{
				f32 min_x = FLT_MAX;
				f32 min_y = FLT_MAX;

				f32 max_x = FLT_MIN;
				f32 max_y = FLT_MIN;

				for (size_t i = 0; i < getTransformedVertices().size(); i++)
				{
					pvec2 vertex = getTransformedVertices()[i];
					if (min_x > vertex.x)
						min_x = vertex.x;
					if (min_y > vertex.y)
						min_y = vertex.y;

					if (max_x < vertex.x)
						max_x = vertex.x;
					if (max_y < vertex.y)
						max_y = vertex.y;
				}

				aabb.min = pvec2(min_x, min_y);
				aabb.max = pvec2(max_x, max_y);
				aabb.half_scale = aabb.center - aabb.min;
			}
			
			updateAABB = false;
		}
		return aabb;
	}


	void move(pvec2 velocity)
	{
		position += velocity;
		updateTransformations = true;
		updateAABB = true;
	}
	void rotate(f32 rotaional_velocity)
	{
		rotation += rotaional_velocity;
		
		if (shape == BodyShape::BOX)
		{
			updateTransformations = true;
			updateAABB = true;
		}
	}
	void setPos(pvec2 new_pos)
	{
		position = new_pos;
		updateTransformations = true;
		updateAABB = true;
	}
	void setRotation(f32 new_rotation)
	{
		rotation = new_rotation;

		if (shape == BodyShape::BOX)
		{
			updateTransformations = true;
			updateAABB = true;
		}
	}
	// BOX features
	void setWidth(f32 new_width)
	{ 
		width = new_width;
		if (shape == BodyShape::BOX || shape == BodyShape::AABB)
		{
			updateTransformations = true;
			updateAABB = true;
		}
	}
	void setHeight(f32 new_height)
	{
		height = new_height;
		if (shape == BodyShape::BOX || shape == BodyShape::AABB)
		{
			updateTransformations = true;
			updateAABB = true;
		}
	}
	// Circle features
	void setRaduis(f32 new_raduis)
	{
		raduis = new_raduis;
		if(shape == BodyShape::CIRCLE)
			updateAABB = true;
	}
	// any body features
	void setRestitution(f32 new_restitution) { restitution = new_restitution; }

	void addForce(const pvec2& force)
	{
		this->force += force;
	}
};



///////////////////////////////////////// world /////////////////////////////////////////

class physicsWorld
{
public:
	physicsWorld() { drag = 0.99f; gravity = pvec2(0.0f, -100.0f); }
	// TODO:: DESTRUCTOR
	~physicsWorld()
	{
		for (size_t i = 0; i < Bodies.size(); i++)
			Bodies[i].reset();
	}


	u32 Add_AABB_Body(const pvec2& position, u32 width, u32 height, f32 mass, f32 density, f32 restitution, bool isStatic)
	{
		std::shared_ptr<PhysicsBody> Body = std::make_shared<PhysicsBody>();
		Body->position = position;
		Body->width = width;
		Body->height = height;

		Body->density = density;
		Body->restitution = clampf(restitution, 0.0f, 1.0f);
		Body->mass = mass;
		Body->invMass = !isStatic ? mass == 0.0f ? 0.0f : 1.0f / mass : 0.0f;

		Body->aabb.center = position;
		Body->aabb.half_scale = pvec2(width * 0.5f, height * 0.5f);
		Body->aabb.min = Body->aabb.center + Body->aabb.half_scale;
		Body->aabb.max = Body->aabb.center + Body->aabb.half_scale;

		Body->shape = BodyShape::AABB;
		Body->isStatic = isStatic;

		Bodies.push_back(Body);

		// return index
		return Bodies.size() - 1;
	}

	u32 Add_Box_Body(const pvec2& position, f32 rotation, u32 width, u32 height, f32 mass, f32 density, f32 restitution, bool isStatic)
	{
		std::shared_ptr<PhysicsBody> Body = std::make_shared<PhysicsBody>();
		Body->position = position;
		Body->rotation = rotation;
		Body->width = width;
		Body->height = height;

		Body->density = density;
		Body->restitution = clampf(restitution, 0.0f, 1.0f);
		Body->mass = mass;
		Body->invMass = !isStatic ? mass == 0.0f ? 0.0f : 1.0f / mass : 0.0f;
		Body->inertia = 0.08333333333f * mass * (width * width + height * height); // inertia for rectangular plate around Z axis
		Body->vertices = {   // vertices in local space
			pvec2(-0.5f,  0.5f),
			pvec2( 0.5f,  0.5f),
			pvec2( 0.5f, -0.5f),
			pvec2(-0.5f, -0.5f)
		};

		// update transformations
		Body->transformed_vertices = Transform_vertices(Body->vertices, Body->position, pvec2(Body->width, Body->height), Body->rotation);

		// calculate AABB
		// calculate min, max
		pvec2 min = pvec2(FLT_MAX, FLT_MAX);
		pvec2 max = pvec2(FLT_MIN, FLT_MIN);

		for (size_t i = 0; i < Body->transformed_vertices.size(); i++)
		{
			if (min.x > Body->transformed_vertices[i].x)
				min.x = Body->transformed_vertices[i].x;
			if (min.y > Body->transformed_vertices[i].y)
				min.y = Body->transformed_vertices[i].y;

			if (max.x < Body->transformed_vertices[i].x)
				max.x = Body->transformed_vertices[i].x;
			if (max.y < Body->transformed_vertices[i].y)
				max.y = Body->transformed_vertices[i].y;
		}

		Body->aabb.center = position;
		Body->aabb.min = min;
		Body->aabb.max = max;
		Body->aabb.half_scale = Body->aabb.center - Body->aabb.min;

		Body->shape = BodyShape::BOX;
		Body->isStatic = isStatic;

		Bodies.push_back(Body);

		// return index
		return Bodies.size() - 1;
	}

	u32 Add_Circle_Body(const pvec2& position, f32 rotation, u32 raduis, f32 mass, f32 density, f32 restitution, bool isStatic)
	{
		std::shared_ptr<PhysicsBody> Body = std::make_shared<PhysicsBody>();
		Body->position = position;
		Body->rotation = rotation;
		Body->raduis = raduis;

		Body->density = density;
		Body->restitution = clampf(restitution, 0.0f, 1.0f);
		Body->mass = mass;
		Body->invMass = !isStatic ? mass == 0.0f ? 0.0f : 1.0f / mass : 0.0f;
		Body->inertia = 0.5f * mass * (raduis * raduis); // inertia for circluar disc around Z axis

		Body->aabb.center = position;
		Body->aabb.half_scale = pvec2(raduis);
		Body->aabb.min = Body->aabb.center + Body->aabb.half_scale;
		Body->aabb.max = Body->aabb.center + Body->aabb.half_scale;

		Body->shape = BodyShape::CIRCLE;
		Body->isStatic = isStatic;

		Bodies.push_back(Body);

		// return index
		return Bodies.size() - 1;
	}





	PhysicsBody* Get_Body(u32 index) { return Bodies[index].get(); }
	u32 Get_Body_Count() { return Bodies.size(); }
	void Set_Gravity(pvec2 gravity_vector) { gravity = gravity_vector; }

	void update(f32 deltaTime, u32 iterations = 1)
	{
		deltaTime /= (f32)iterations;

		while (iterations > 0)
		{


			for (size_t i = 0; i < Bodies.size(); i++)
			{
				PhysicsBody* Body = Get_Body(i);

				// Skip if is static
				if (Body->isStatic)
					continue;

				//pvec2 acceleration = Body->force * Body->invMass;
				//Body->linearVelocity += acceleration * deltaTime;
				Body->linearVelocity += gravity * deltaTime;
				Body->move(Body->linearVelocity * deltaTime);

				Body->rotate(Body->rotationalVelocity * deltaTime);

				//Body->linearVelocity *= drag;
				//Body->force = pvec2(0.0f);
			}

			for (size_t a = 0; a < Bodies.size(); a++)
			{
				PhysicsBody* BodyA = Get_Body(a);
				for (size_t b = a + 1; b < Bodies.size(); b++)
				{
					PhysicsBody* BodyB = Get_Body(b);

					if (BodyA->isStatic && BodyB->isStatic)
						continue;

					pvec2 collision_normal;
					f32 collision_depth;
					if (isCollided(BodyA, BodyB, collision_normal, collision_depth))
					{
						// rseolve collision
						if (BodyA->isStatic)
							BodyB->move(collision_normal * collision_depth);
						else if (BodyB->isStatic)
							BodyA->move(collision_normal * -collision_depth);
						else
						{
							BodyA->move(collision_normal * collision_depth * -0.5f);
							BodyB->move(collision_normal * collision_depth *  0.5f);
						}
						// impulse resolution
						impulse_resolution(BodyA, BodyB, collision_normal);
					}
				} // b
			} // a
			iterations--;
		} // iterations
	}

private:
	std::vector<std::shared_ptr<PhysicsBody>> Bodies;
	pvec2 gravity;
	f32 drag;

	bool isCollided(PhysicsBody* BodyA, PhysicsBody* BodyB, pvec2& collison_normal, f32& collision_depth)
	{
		bool is_collided = false;

		if (BodyA->shape == BodyShape::AABB)
		{
			if (BodyB->shape == BodyShape::AABB)
				is_collided = collisions::AABB_vs_AABB(BodyA->getAABB(), BodyB->getAABB(), collison_normal, collision_depth);
			if (BodyB->shape == BodyShape::BOX)
				is_collided = collisions::AABB_vs_Polygon(BodyA->getAABB(), BodyB->position, &BodyB->getTransformedVertices()[0], 4, collison_normal, collision_depth);
			if (BodyB->shape == BodyShape::CIRCLE)
			{
				is_collided = collisions::AABB_vs_Circle(BodyA->getAABB(), BodyB->position, BodyB->raduis, collison_normal, collision_depth);
				collison_normal *= -1.0f;
			}
		}

		else if (BodyA->shape == BodyShape::BOX)
		{
			if (BodyB->shape == BodyShape::AABB)
			{
				is_collided = collisions::AABB_vs_Polygon(BodyB->getAABB(), BodyA->position, &BodyA->getTransformedVertices()[0], 4, collison_normal, collision_depth);
				collison_normal *= -1.0f;
			}
			if (BodyB->shape == BodyShape::BOX)
				is_collided = collisions::Polygon_vs_Polygon(BodyA->position, &BodyA->getTransformedVertices()[0], 4, BodyB->position, &BodyB->getTransformedVertices()[0], 4, collison_normal, collision_depth);
			if (BodyB->shape == BodyShape::CIRCLE)
			{
				is_collided = collisions::Polygon_vs_Circle(BodyA->position, &BodyA->getTransformedVertices()[0], 4, BodyB->position, BodyB->raduis, collison_normal, collision_depth);
				collison_normal *= -1.0f;
			}
		}

		else if (BodyA->shape == BodyShape::CIRCLE)
		{
			if (BodyB->shape == BodyShape::AABB)
				is_collided = collisions::AABB_vs_Circle(BodyB->getAABB(), BodyA->position, BodyA->raduis, collison_normal, collision_depth);
			if (BodyB->shape == BodyShape::BOX)
				is_collided = collisions::Polygon_vs_Circle(BodyB->position, &BodyB->getTransformedVertices()[0], 4, BodyA->position, BodyA->raduis, collison_normal, collision_depth);
			if (BodyB->shape == BodyShape::CIRCLE)
				is_collided = collisions::Circle_vs_Circle(BodyA->position, BodyA->raduis, BodyB->position, BodyB->raduis, collison_normal, collision_depth);
		}

		return is_collided;
	}

	void impulse_resolution(PhysicsBody* BodyA, PhysicsBody* BodyB, const pvec2 collision_normal)
	{
		pvec2 relative_velocity = BodyB->linearVelocity - BodyA->linearVelocity;
		f32 d = dot(relative_velocity, collision_normal);

		if (d > 0.0f) // the relative velocity is in the same direction of vector normal
			return;

		f32 e = std::min(BodyA->getRestitution(), BodyB->getRestitution());
		f32 j = -(1 + e) * d;
		j /= (BodyA->invMass + BodyB->invMass);

		pvec2 impulse = collision_normal * j; // get impulse vector along the normal

		BodyA->linearVelocity -= impulse * BodyA->invMass; // scale impulse vector by inverse mass
		BodyB->linearVelocity += impulse * BodyB->invMass; // scale impulse vector by inverse mass
	}
};