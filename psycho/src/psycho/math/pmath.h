#pragma once
#include <psycho/math/pvec2.h>
#include <math.h>
#include <psycho/logger/passerts.h>
#include <psycho/world/pTransform.h>

#define PI 3.14159265359
#define TO_RAD(degree) degree * 0.01745329251

class pmath
{
public:
	static inline f32 length(const pvec2& v)
	{
		return sqrtf(v.x * v.x + v.y * v.y);
	}

	static inline f32 length_sqrd(const pvec2& v)
	{
		return v.x * v.x + v.y * v.y;
	}

	static inline f32 distance(const pvec2& v1, const pvec2& v2)
	{
		f32 dx = v1.x - v2.x;
		f32 dy = v1.y - v2.y;
		return sqrtf(dx * dx + dy * dy);
	}

	static inline f32 distance_sqrd(const pvec2& v1, const pvec2& v2)
	{
		f32 dx = v1.x - v2.x;
		f32 dy = v1.y - v2.y;
		return dx * dx + dy * dy;
	}

	static inline pvec2 normalize(const pvec2& v)
	{
		f32 mag = length(v);
		if (mag != 0.0)
			return pvec2(v.x / mag, v.y / mag);
		else
			return pvec2(0.0f, 0.0f);
	}

	static inline f32 dot(const pvec2& v1, const pvec2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	static inline f32 cross(const pvec2& v1, const pvec2& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}

	static f32 clampf(f32 value, f32 min, f32 max)
	{
		PSYCHO_ASSERT_MSG(min < max, "min value = " + std::to_string(min) + " is larger than max value = " + std::to_string(max));

		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	static u32 clamp(u32 value, u32 min, u32 max)
	{
		PSYCHO_ASSERT_MSG(min < max, "min value = " + std::to_string(min) + " is larger than max value = " + std::to_string(max));

		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	static pvec2 add(const pvec2& a, const pvec2& b)
	{
		return pvec2(a.x + b.x, a.y + b.y);
	}
	static pvec2 sub(const pvec2& a, const pvec2& b)
	{
		return pvec2(a.x - b.x, a.y - b.y);
	}
	static pvec2 scale(const pvec2& a, f32 b)
	{
		return pvec2(a.x * b, a.y * b);
	}
	static pvec2 div(const pvec2& a, f32 b)
	{
		return pvec2(a.x / b, a.y / b);
	}

	static f32 min(f32 a, f32 b)
	{
		if (a < b)
			return a;
		return b;
	}

	static pvec2 rotateVector(const pvec2& v, f32 angle)
	{
		// rotate vector (get new pos after rotation)
		f32 rx = cosf(angle) * v.x - sinf(angle) * v.y;
		f32 ry = sinf(angle) * v.x - cosf(angle) * v.y;
		return pvec2(rx, ry);
	}

	static void transformVector(pvec2& vector, const pvec2& translation, const pvec2& scale, f32 rotation)
	{
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
	}

	// if difference between them is less than 3 pixels return true else return false
	static bool nearlyEqual(f32 a, f32 b)
	{
		return fabs(a - b) < 3.0f;
	}

	// if difference between them is less than 3 pixels return true else return false
	static bool nearlyEqual(const pvec2& a, const pvec2& b)
	{
		return nearlyEqual(a.x, b.x) && nearlyEqual(a.y, b.y);
	}
};