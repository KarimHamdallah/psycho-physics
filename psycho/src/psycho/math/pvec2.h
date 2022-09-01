#pragma once
#include <psycho/defines.h>

template <class T>
class vec2 {
public:
	T x, y;

	vec2() :x(0), y(0) {}
	vec2(T x, T y) : x(x), y(y) {}
	vec2(T value) : x(value), y(value) {}
	vec2(const vec2& v) : x(v.x), y(v.y) {}

	vec2& operator=(const vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	vec2 operator+(const vec2& v) {
		return vec2(x + v.x, y + v.y);
	}
	vec2 operator-(const vec2& v) {
		return vec2(x - v.x, y - v.y);
	}

	vec2& operator+=(const vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	vec2& operator-=(const vec2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec2 operator+(f32 s) {
		return vec2(x + s, y + s);
	}
	vec2 operator-(f32 s) {
		return vec2(x - s, y - s);
	}
	vec2 operator*(f32 s) {
		return vec2(x * s, y * s);
	}
	vec2 operator/(f32 s) {
		return vec2(x / s, y / s);
	}


	vec2& operator+=(f32 s) {
		x += s;
		y += s;
		return *this;
	}
	vec2& operator-=(f32 s) {
		x -= s;
		y -= s;
		return *this;
	}
	vec2& operator*=(f32 s) {
		x *= s;
		y *= s;
		return *this;
	}
	vec2& operator/=(f32 s) {
		x /= s;
		y /= s;
		return *this;
	}

	void set(T x, T y) {
		this->x = x;
		this->y = y;
	}
};

typedef vec2<f32> pvec2;