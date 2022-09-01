#pragma once
#include <psycho/defines.h>
#include <psycho/math/pmath.h>

class pTransform
{
public:
	f32 posx;
	f32 posy;
	f32 sin;
	f32 cos;

	pTransform(const pvec2& position, f32 angle)
	{
		posx = position.x;
		posy = position.y;
		sin = sinf(angle);
		cos = cosf(angle);
	}
	pTransform(f32 position_x, f32 position_y, f32 angle)
	{
		posx = position_x;
		posy = position_y;
		sin = sinf(angle);
		cos = cosf(angle);
	}
};