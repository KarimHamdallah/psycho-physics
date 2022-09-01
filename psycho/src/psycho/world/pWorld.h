#pragma once
#include <psycho/defines.h>
#include <psycho/collisions/collisions.h>
#include <psycho/world/pBody.h>
#include <psycho/collisions/collision manifold.h>

#define MIN_SCALE 0.0f

#define MIN_DENISTY 0.5f
#define MAX_DENISTY 21.4f

class pWorld
{
public:
	static pvec2 gravity;
	static std::vector<pBody*> bodies;
	static std::vector<pCollision_manifold*> contactList;
	static std::vector<pvec2> contactPointsList;


	static void world_init();
	static u32 addCircleBody(const pvec2& position, f32 raduis, f32 mass, f32 restitution, f32 isStatic);
	static u32 addBoxeBody(const pvec2& position, f32 width, f32 height, f32 mass, f32 restitution, f32 isStatic);
	static void removeBody(u32 body_index);
	static pBody* getBody(u32 body_index);
	static void worldUpdate(f32 deltaTime, pvec2 gravity, u32 iterations = 1);
	static inline u32 getBodyCount() { return bodies.size(); }


	static void free_memory();

private:
	static void resolve_collsion(pCollision_manifold* manifold);
};