#pragma once
#include <psycho/math/pmath.h>
#include <psycho/world/pBody.h>

class pCollision_manifold
{
public:
	pBody* BodyA = nullptr;
	pBody* BodyB = nullptr;
	pvec2 normal = pvec2(0.0f);
	f32 depth = 0.0f;
	pvec2 contact1 = pvec2(0.0f);
	pvec2 contact2 = pvec2(0.0f);
	u32 contactCount = 0;

	pCollision_manifold(
		pBody* BodyA,
		pBody* BodyB,
		const pvec2& normal,
		f32 depth,
		const pvec2& contact1,
		const pvec2& contact2,
		u32 contactCount)
	{
		this->BodyA = BodyA;
		this->BodyB = BodyB;
		this->normal = normal;
		this->depth = depth;
		this->contact1 = contact1;
		this->contact2 = contact2;
		this->contactCount = contactCount;
	}
};