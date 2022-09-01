#pragma once
#include <psycho/defines.h>
#include <psycho/math/pmath.h>
#include <psycho/world/pBody.h>

class pCollisons
{
public:
	static bool circle_vs_circle(const pvec2& centerA, f32 raduisA, const pvec2& centerB, f32 raduisB, pvec2& normal, f32& depth);
	static bool polygon_vs_polygon(const pvec2& centerA, pvec2 *verticesA, u32 vertices_countA, const pvec2& centerB, pvec2* verticesB, u32 vertices_countB, pvec2& normal, f32& depth);
	static bool circle_vs_polygon(const pvec2& circle_center, f32 raduis,const pvec2& polygon_center, const pvec2* vertices, u32 vertices_count, pvec2& normal, f32& depth);
	static bool collide(pBody* BodyA, pBody* BodyB, pvec2& normal, f32& depth);
	static void findContactPoints(pBody* BodyA, pBody* BodyB, pvec2& contactPoint1, pvec2& contactPoint2, u32& contactPointsCount);
	static bool AABB_vs_AABB(const pAABB& A, const pAABB& B);
private:
	static void projectVertices(pvec2* vertices, u32 vertices_count, const pvec2& axis, f32& min, f32& max);
	// circle-circle contact point
	static void findContactPoint(const pvec2& circle_centerA, f32 raduisA, const pvec2& circle_centerB, pvec2& contactPoint);
	// polygon-circle contact point
	static void findContactPoint(const pvec2& circle_center, f32 raduis, const pvec2& polygon_center, const pvec2* vertices, u32 vertices_count, pvec2& contactPoint);
	static void findContactPoint(pvec2* verticesA, u32 vertices_countA, pvec2* verticesB, u32 vertices_countB, pvec2& contact1, pvec2& contact2, u32& contactPointsCount);
	static void getClosestPointToLine(const pvec2& a, const pvec2& b, const pvec2& p, f32& distanceSqrd, pvec2& contact);
};