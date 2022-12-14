#pragma once

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

#ifndef __RANDOM_H__
#define __RANDOM_H__


// Wrapper for the C++11 random generators

#include <chrono>
#include <random>

class Random {
	std::mt19937 _rng;
public:
	Random() {
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto seed = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
		_rng.seed(seed);
	}
	Random(unsigned int seed) { _rng.seed(seed); }

	float operator()(float max = 1.0f) {
		std::uniform_real_distribution<float> dist(0.0f, max);
		return dist(_rng);
	}
	float operator()(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(_rng);
	}

	int operator()(int max = std::mt19937::max()) {
		std::uniform_int_distribution<int> dist(0, max);
		return dist(_rng);
	}
	int operator()(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(_rng);
	}

	static Random& get() { static Random rng; return rng; }
};

#define random (Random::get())


#endif