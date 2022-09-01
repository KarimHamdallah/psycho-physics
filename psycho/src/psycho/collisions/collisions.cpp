#include "collisions.h"

bool pCollisons::circle_vs_circle(const pvec2 & centerA, f32 raduisA, const pvec2 & centerB, f32 raduisB, pvec2 & normal, f32& depth)
{
	normal = pvec2(0.0f);
	depth = 0.0f;

	f32 distance = pmath::distance(centerA, centerB);
	f32 radii = raduisA + raduisB;
	if (distance >= radii)
		return false;

	normal = pmath::normalize(pmath::sub(centerA, centerB));
	depth = radii - distance;
	return true;
}

bool pCollisons::polygon_vs_polygon(const pvec2& centerA, pvec2 *verticesA, u32 vertices_countA, const pvec2& centerB, pvec2* verticesB, u32 vertices_countB, pvec2& normal, f32& depth)
{
	normal = pvec2(0.0f);
	depth = FLT_MAX;

	// check axises of first polygon
	for (size_t i = 0; i < vertices_countA; i++)
	{
		// calculate polygon edge
		pvec2 va = verticesA[i];
		pvec2 vb = verticesA[(i + 1) % vertices_countA];

		pvec2 edge = pmath::sub(vb, va);
		// get edge normal to be used as axis for testing intersection
		pvec2 edge_normal = pvec2(-edge.y, edge.x);
		edge_normal = pmath::normalize(edge_normal);

		f32 minA, minB, maxA, maxB;
		projectVertices(verticesA, vertices_countA, edge_normal, minA, maxA);
		projectVertices(verticesB, vertices_countB, edge_normal, minB, maxB);

		// check if there is a gap
		if (minA >= maxB || minB >= maxA)
		{
			depth = 0.0f;
			return false;
		}

		// if there is no gap
		f32 axis_depth = pmath::min(maxA - minB, maxB - minA);
		if (axis_depth < depth)
		{
			depth = axis_depth;
			normal = edge_normal;
		}
	}
	// check axises of seconed polygon
	for (size_t i = 0; i < vertices_countB; i++)
	{
		// calculate polygon edge
		pvec2 va = verticesB[i];
		pvec2 vb = verticesB[(i + 1) % vertices_countB];

		pvec2 edge = pmath::sub(vb, va);
		// get edge normal to be used as axis for testing intersection
		pvec2 edge_normal = pvec2(-edge.y, edge.x);
		edge_normal = pmath::normalize(edge_normal);

		f32 minA, minB, maxA, maxB;
		projectVertices(verticesA, vertices_countA, edge_normal, minA, maxA);
		projectVertices(verticesB, vertices_countB, edge_normal, minB, maxB);

		// check if there is a gap
		if (minA >= maxB || minB >= maxA)
		{
			depth = 0.0f;
			return false;
		}

		// if there is no gap
		f32 axis_depth = pmath::min(maxA - minB, maxB - minA);
		if (axis_depth < depth)
		{
			depth = axis_depth;
			normal = edge_normal;
		}
	}

	pvec2 direction = pmath::sub(centerB, centerA);
	if (pmath::dot(direction, normal) > 0.0f)
	{
		normal.x *= -1.0f;
		normal.y *= -1.0f;
	}

	return true;
}

bool pCollisons::circle_vs_polygon(const pvec2 & circle_center, f32 raduis, const pvec2& polygon_center, const pvec2 * vertices, u32 vertices_count, pvec2 & normal, f32 & depth)
{
	normal = pvec2(0.0f);
	depth = FLT_MAX;

	for (size_t i = 0; i < vertices_count; i++)
	{
		pvec2 va = vertices[i];
		pvec2 vb = vertices[(i + 1) % vertices_count];

		pvec2 edge = pmath::sub(vb, va);
		pvec2 edge_normal = pvec2(-edge.y, edge.x);

		edge_normal = pmath::normalize(edge_normal);

		f32 minA = FLT_MAX;
		f32 maxA = FLT_MIN;
		f32 minB = FLT_MAX; // circle min
		f32 maxB = FLT_MIN; // circle max

		// loop through every vertex in first polygon and get min and max value of polygon 
		for (size_t a = 0; a < vertices_count; a++)
		{
			f32 proj = pmath::dot(vertices[a], edge_normal);
			if (proj < minA)
				minA = proj;
			if (proj > maxA)
				maxA = proj;
		}

		// project 2 vertices min and max of circle into axis
		pvec2 axis_direction = pmath::normalize(edge_normal);
		pvec2 circle_min = pmath::add(circle_center, pmath::scale(axis_direction, -raduis));
		pvec2 circle_max = pmath::add(circle_center, pmath::scale(axis_direction, raduis));
		minB = pmath::dot(circle_min, edge_normal);
		maxB = pmath::dot(circle_max, edge_normal);
		if (minB > maxB)
		{
			f32 temp = minB;
			minB = maxB;
			maxB = temp;
		}



		if (minB > maxA || minA > maxB)
		{
			depth = 0.0f;
			return false;
		}

		// if there is no gap
		f32 axis_depth = pmath::min(maxA - minB, maxB - minA);
		if (axis_depth < depth)
		{
			depth = axis_depth;
			normal = edge_normal;
		}
	}

	{
		// final axis test

		// get closest point to the circle
		u32 closest_vertex_idx;
		f32 min_distance_sqrd = FLT_MAX;
		for (size_t i = 0; i < vertices_count; i++)
		{
			f32 dist_sqrd = pmath::distance_sqrd(vertices[i], circle_center);
			if (dist_sqrd < min_distance_sqrd)
			{
				min_distance_sqrd = dist_sqrd;
				closest_vertex_idx = (u32)i;
			}
		}
		// calculate final axis
		pvec2 final_axis = pmath::sub(vertices[closest_vertex_idx], circle_center);
		final_axis = pmath::normalize(final_axis);

		// calculate min and max of each shape and test
		f32 minA = FLT_MAX;
		f32 maxA = FLT_MIN;
		f32 minB = FLT_MAX; // circle min
		f32 maxB = FLT_MIN; // circle max

		// loop through every vertex in first polygon and get min and max value of polygon 
		for (size_t a = 0; a < vertices_count; a++)
		{
			f32 proj = pmath::dot(vertices[a], final_axis);
			if (proj < minA)
				minA = proj;
			if (proj > maxA)
				maxA = proj;
		}

		// project 2 vertices min and max of circle into axis
		pvec2 axis_direction = pmath::normalize(final_axis);
		pvec2 circle_min = pmath::add(circle_center, pmath::scale(axis_direction, -raduis));
		pvec2 circle_max = pmath::add(circle_center, pmath::scale(axis_direction, raduis));
		minB = pmath::dot(circle_min, final_axis);
		maxB = pmath::dot(circle_max, final_axis);
		if (minB > maxB)
		{
			f32 temp = minB;
			minB = maxB;
			maxB = temp;
		}


		if (minB > maxA || minA > maxB)
		{
			depth = 0.0f;
			return false;
		}

		// if there is no gap
		f32 axis_depth = pmath::min(maxA - minB, maxB - minA);
		if (axis_depth < depth)
		{
			depth = axis_depth;
			normal = final_axis;
		}
	}

	pvec2 direction = pmath::sub(polygon_center, circle_center);
	if (pmath::dot(direction, normal) > 0.0f)
	{
		normal.x *= -1.0f;
		normal.y *= -1.0f;
	}

	return true;
}
void pCollisons::findContactPoint(const pvec2 & circle_centerA, f32 raduisA, const pvec2 & circle_centerB, pvec2 & contactPoint)
{
	pvec2 dir = pmath::normalize(pmath::sub(circle_centerB, circle_centerA));
	contactPoint = pmath::add(circle_centerA, pmath::scale(dir, raduisA));
}
void pCollisons::findContactPoint(const pvec2 & circle_center, f32 raduis, const pvec2 & polygon_center, const pvec2 * vertices, u32 vertices_count, pvec2 & contactPoint)
{
	contactPoint = pvec2(0.0f);

	f32 min_distanceSqrd = FLT_MAX;

	for (size_t i = 0; i < vertices_count; i++)
	{
		pvec2 va = vertices[i];
		pvec2 vb = vertices[(i + 1) % vertices_count];

		f32 distSqrd;
		pvec2 contact;
		getClosestPointToLine(va, vb, circle_center, distSqrd, contact);
		
		if (distSqrd < min_distanceSqrd)
		{
			min_distanceSqrd = distSqrd;
			contactPoint = contact;
		}
	}
}
void pCollisons::findContactPoint(pvec2 * verticesA, u32 vertices_countA, pvec2 * verticesB, u32 vertices_countB, pvec2 & contact1, pvec2 & contact2, u32 & contactPointsCount)
{
	contact1 = pvec2(0.0f);
	contact2 = pvec2(0.0f);
	contactPointsCount = 0;

	f32 minDistSq = FLT_MAX;

	for (int i = 0; i < vertices_countA; i++)
	{
		pvec2 p = verticesA[i];

		for (int j = 0; j < vertices_countB; j++)
		{
			pvec2 va = verticesB[j];
			pvec2 vb = verticesB[(j + 1) % vertices_countB];

			f32 distSq;
			pvec2 cp;

			getClosestPointToLine(va, vb, p, distSq, cp);

			if (pmath::nearlyEqual(distSq, minDistSq))
			{
				if (!pmath::nearlyEqual(cp, contact1) &&
					!pmath::nearlyEqual(cp, contact2))
				{
					contact2 = cp;
					contactPointsCount = 2;
				}
			}
			else if (distSq < minDistSq)
			{
				minDistSq = distSq;
				contactPointsCount = 1;
				contact1 = cp;
			}
		}
	}

	for (int i = 0; i < vertices_countB; i++)
	{
		pvec2 p = verticesB[i];

		for (int j = 0; j < vertices_countA; j++)
		{
			pvec2 va = verticesA[j];
			pvec2 vb = verticesA[(j + 1) % vertices_countA];

			f32 distSq;
			pvec2 cp;

			getClosestPointToLine(va, vb, p, distSq, cp);

			if (pmath::nearlyEqual(distSq, minDistSq))
			{
				if (!pmath::nearlyEqual(cp, contact1) &&
					!pmath::nearlyEqual(cp, contact2))
				{
					contact2 = cp;
					contactPointsCount = 2;
				}
			}
			else if (distSq < minDistSq)
			{
				minDistSq = distSq;
				contactPointsCount = 1;
				contact1 = cp;
			}
		}
	}
}
void pCollisons::getClosestPointToLine(const pvec2 & a, const pvec2 & b, const pvec2 & p, f32& distanceSqrd, pvec2 & contact)
{
	distanceSqrd = 0.0f;
	contact = pvec2(0.0f);

	pvec2 ab = pmath::sub(b, a);
	pvec2 ap = pmath::sub(p, a);

	f32 proj = pmath::dot(ap, ab);
	f32 abLengthSqred = pmath::length_sqrd(ab);
	f32 d = proj / abLengthSqred;

	if (d <= 0.0f)
		contact = a;
	else if (d >= 1.0f)
		contact = b;
	else
	{
		contact = pmath::add(a, pmath::scale(ab, d));
	}

	distanceSqrd = pmath::distance_sqrd(p, contact);
}
bool pCollisons::collide(pBody * BodyA, pBody * BodyB, pvec2 & normal, f32 & depth)
{
	bool collide = false;

	if (BodyA->shapeType == pBodyShape::Box)
	{
		if(BodyB->shapeType == pBodyShape::Box)
		{
			collide =
				pCollisons::polygon_vs_polygon(BodyA->position, &BodyA->getTransformedVertices()[0], 4,
					BodyB->position, &BodyB->getTransformedVertices()[0], 4,
					normal, depth);

		}
		else if (BodyB->shapeType == pBodyShape::Circle)
		{
			collide =
				pCollisons::circle_vs_polygon(BodyB->position, BodyB->raduis,
					BodyA->position, &BodyA->getTransformedVertices()[0], 4,
					normal, depth);

			normal.x *= -1.0f;
			normal.y *= -1.0f;
		}
	}
	if (BodyA->shapeType == pBodyShape::Circle)
	{
		if (BodyB->shapeType == pBodyShape::Box)
		{
			collide =
				pCollisons::circle_vs_polygon(BodyA->position, BodyA->raduis,
					BodyB->position, &BodyB->getTransformedVertices()[0], 4,
					normal, depth);
		}
		if (BodyB->shapeType == pBodyShape::Circle)
		{
			collide =
				pCollisons::circle_vs_circle(BodyA->position, BodyA->raduis, BodyB->position, BodyB->raduis,
					normal, depth);
		}
	}

	return collide;
}
void pCollisons::findContactPoints(pBody * BodyA, pBody * BodyB, pvec2 & contactPoint1, pvec2 & contactPoint2, u32 & contactPointsCount)
{
	contactPoint1 = pvec2(0.0f);
	contactPoint2 = pvec2(0.0f);
	contactPointsCount = 0;

	pBodyShape shapeA = BodyA->shapeType;
	pBodyShape shapeB = BodyB->shapeType;

	if (BodyA->shapeType == pBodyShape::Box)
	{
		if (BodyB->shapeType == pBodyShape::Box)
		{
			findContactPoint(&BodyA->getTransformedVertices()[0], 4, &BodyB->getTransformedVertices()[0], 4, contactPoint1, contactPoint2, contactPointsCount);
		}
		else if (BodyB->shapeType == pBodyShape::Circle)
		{
			findContactPoint(BodyB->position, BodyB->raduis, BodyA->position, &BodyA->getTransformedVertices()[0], 4, contactPoint1);
			contactPointsCount = 1;
		}
	}
	if (BodyA->shapeType == pBodyShape::Circle)
	{
		if (BodyB->shapeType == pBodyShape::Box)
		{
			findContactPoint(BodyA->position, BodyA->raduis, BodyB->position, &BodyB->getTransformedVertices()[0], 4, contactPoint1);
			contactPointsCount = 1;
		}
		if (BodyB->shapeType == pBodyShape::Circle)
		{
			findContactPoint(BodyA->position, BodyA->raduis, BodyB->position, contactPoint1);
			contactPointsCount = 1;
		}
	}
}
bool pCollisons::AABB_vs_AABB(const pAABB& A, const pAABB& B)
{
	if (A.max.x <= B.min.x ||
		A.min.x >= B.max.x ||
		A.max.y <= B.min.y ||
		A.min.y >= B.max.y)
		return false;
	
	return true;
}
void pCollisons::projectVertices(pvec2 * vertices, u32 vertices_count, const pvec2& axis, f32 & min, f32& max)
{
	min = FLT_MAX;
	max = FLT_MIN;

	for (size_t i = 0; i < vertices_count; i++)
	{
		f32 proj = pmath::dot(vertices[i], axis);
		if (proj < min)
			min = proj;
		if (proj > max)
			max = proj;
	}
}