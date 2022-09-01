#pragma once
#include <psycho/defines.h>
#include <psycho/math/pmath.h>

class pTriggers
{
public:
	static bool circle_vs_circle(const pvec2 & centerA, f32 raduisA, const pvec2 & centerB, f32 raduisB)
	{
		f32 distance_sqrd = pmath::distance_sqrd(centerA, centerB);
		f32 radii = raduisA + raduisB;
		f32 radii_sqrd = radii * radii;

		if (distance_sqrd >= radii_sqrd)
			return false;

		return true;
	}

	static bool polygon_vs_polygon(const pvec2* verticesA, u32 vertices_countA, const pvec2* verticesB, u32 vertices_countB)
	{
		// get all axeses of first plygon and project all vertices of both polygons to return min and max
		for (int i = 0; i < vertices_countA; i++)
		{
			pvec2 va = verticesA[i];
			pvec2 vb = verticesA[(i + 1) % vertices_countA];

			pvec2 edge = pmath::sub(vb, va);
			pvec2 edge_normal = pvec2(-edge.y, edge.x); // axis


			f32 minA = FLT_MAX;
			f32 maxA = FLT_MIN;
			f32 minB = FLT_MAX;
			f32 maxB = FLT_MIN;

			// loop through every vertex in first polygon and get min and max value of polygon 
			for (size_t a = 0; a < vertices_countA; a++)
			{
				f32 proj = pmath::dot(verticesA[a], edge_normal);
				if (proj < minA)
					minA = proj;
				if (proj > maxA)
					maxA = proj;
			}

			// loop through every vertex in seconed polygon and get min and max value of polygon 
			for (size_t b = 0; b < vertices_countB; b++)
			{
				f32 proj = pmath::dot(verticesB[b], edge_normal);
				if (proj < minB)
					minB = proj;
				if (proj > maxB)
					maxB = proj;
			}

			if (minA > maxB || minB > maxA)
				return false;
		}

		// get all axeses of seconed plygon and project all vertices of both polygons to return min and max
		for (int i = 0; i < vertices_countB; i++)
		{
			pvec2 va = verticesB[i];
			pvec2 vb = verticesB[(i + 1) % vertices_countB];

			pvec2 edge = pmath::sub(vb, va);
			pvec2 edge_normal = pvec2(-edge.y, edge.x); // axis


			f32 minA = FLT_MAX;
			f32 maxA = FLT_MIN;
			f32 minB = FLT_MAX;
			f32 maxB = FLT_MIN;

			// loop through every vertex in first polygon and get min and max value of polygon 
			for (size_t a = 0; a < vertices_countA; a++)
			{
				f32 proj = pmath::dot(verticesA[a], edge_normal);
				if (proj < minA)
					minA = proj;
				if (proj > maxA)
					maxA = proj;
			}

			// loop through every vertex in seconed polygon and get min and max value of polygon 
			for (size_t b = 0; b < vertices_countB; b++)
			{
				f32 proj = pmath::dot(verticesB[b], edge_normal);
				if (proj < minB)
					minB = proj;
				if (proj > maxB)
					maxB = proj;
			}

			if (minA > maxB || minB > maxA)
				return false;
		}

		return true;
	}

	static bool circle_vs_polygon(const pvec2& circle_center, f32 raduis, const pvec2* vertices, u32 vertices_count)
	{
		for (size_t i = 0; i < vertices_count; i++)
		{
			pvec2 va = vertices[i];
			pvec2 vb = vertices[(i + 1) % vertices_count];

			pvec2 edge = pmath::sub(vb, va);
			pvec2 edge_normal = pvec2(-edge.y, edge.x);

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
				return false;
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
				return false;

		}

		return true;
	}
};