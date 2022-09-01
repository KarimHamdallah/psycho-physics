#include "pWorld.h"

pvec2 pWorld::gravity;
std::vector<pBody*> pWorld::bodies;
std::vector<pCollision_manifold*> pWorld::contactList;
std::vector<pvec2> pWorld::contactPointsList;

void pWorld::world_init()
{
	gravity = pvec2(0.0f, -9.81f);
}

u32 pWorld::addCircleBody(const pvec2& position, f32 raduis, f32 mass, f32 restitution, f32 isStatic)
{
	pBody* body = pBody::creatCircleBody(position, raduis, mass, restitution, isStatic);
	bodies.push_back(body);
	return bodies.size() - 1;
}

u32 pWorld::addBoxeBody(const pvec2& position, f32 width, f32 height, f32 mass, f32 restitution, f32 isStatic)
{
	pBody* body = pBody::creatBoxBody(position, width, height, mass, restitution, isStatic);
	bodies.push_back(body);
	return bodies.size() - 1;
}

void pWorld::removeBody(u32 body_index)
{
	// free memory
	if (bodies[body_index])
		delete bodies[body_index];

	// remove with array shifting
	bodies.erase(bodies.begin() + body_index);
}

pBody* pWorld::getBody(u32 body_index)
{
	if (body_index >= 0 && body_index < bodies.size())
	{
		if (bodies[body_index])
			return bodies[body_index];
		else
		{
			PSYCHO_ERROR("cant find pbody in the world at index {}", body_index);
			return nullptr;
		}
	}
	else
	{
		PSYCHO_ERROR("body index {} is out of world body list range", body_index);
		return nullptr;
	}
}

void pWorld::worldUpdate(f32 deltaTime, pvec2 gravity, u32 iterations)
{
	iterations = pmath::clamp(iterations, 1, 128);
	deltaTime /= (f32)iterations;

	for (size_t it = 0; it < iterations; it++)
	{
		// integrate
		for (size_t i = 0; i < bodies.size(); i++)
		{
			if (bodies[i]->isStatic)
				continue;

			//pvec2 acceleration;
			//acceleration.x = bodies[i]->force.x / bodies[i]->mass;
			//acceleration.y = bodies[i]->force.y / bodies[i]->mass;

			bodies[i]->linearvelocity += pmath::scale(gravity, deltaTime);
			bodies[i]->position += pmath::scale(bodies[i]->linearvelocity, deltaTime);
			bodies[i]->rotation += bodies[i]->angularVelocity * deltaTime;
			bodies[i]->transformUpdateRequired = true;
			bodies[i]->AABBUpdateRequired = true;

			// reset forces
			bodies[i]->force = pvec2(0.0f);
		}


		pvec2 normal = pvec2(0.0f);
		f32 depth = 0.0f;
		bool collide;
		// free contact points memory
		for (size_t i = 0; i < contactList.size(); i++)
		{
			if (contactList[i])
				delete contactList[i];
		}
		// clear contact points
		contactList.clear();
		contactPointsList.clear();

		// collsion dtection and resoluttion
		for (size_t i = 0; i < bodies.size(); i++)
		{
			pBody* BodyA = bodies[i];
			pAABB bodyA_aabb = BodyA->get_AABB();
			for (size_t j = i + 1; j < bodies.size(); j++)
			{
				pBody* BodyB = bodies[j];
				pAABB bodyB_aabb = BodyB->get_AABB();

				if (BodyA->isStatic && BodyB->isStatic)
					continue;

				if (!pCollisons::AABB_vs_AABB(bodyA_aabb, bodyB_aabb))
					continue;

				// collide
				collide = pCollisons::collide(BodyA, BodyB, normal, depth);

				// resolve collison
				if (collide)
				{
					pvec2 penteration_vector = pmath::scale(normal, depth);

					if (BodyA->isStatic)
						BodyB->move(pmath::scale(penteration_vector, -1.0f));
					else if (BodyB->isStatic)
						BodyA->move(penteration_vector);
					else
					{
						BodyA->move(pmath::scale(penteration_vector, 0.5f));
						BodyB->move(pmath::scale(penteration_vector, -0.5f));
					}

					pvec2 contactPoint1;
					pvec2 contactPoint2;
					u32 contactPointsCount;
					pCollisons::findContactPoints(BodyA, BodyB, contactPoint1, contactPoint2, contactPointsCount);
					pCollision_manifold* contact = new pCollision_manifold(BodyA, BodyB, normal, depth, contactPoint1, contactPoint2, contactPointsCount);
					contactList.push_back(contact);
				}

			} // inner loop
		} // outer loop

		
		for (size_t i = 0; i < contactList.size(); i++)
		{
			pCollision_manifold* contact = contactList[i];
			resolve_collsion(contact);


			if (contact->contactCount > 0)
			{
				contactPointsList.push_back(contact->contact1);
				if (contact->contactCount > 1)
					contactPointsList.push_back(contact->contact2);
			}
		}
	} // iterations
}

void pWorld::free_memory()
{
	for (size_t i = 0; i < bodies.size(); i++)
	{
		if (bodies[i])
			delete bodies[i];
	}
	for (size_t i = 0; i < contactList.size(); i++)
	{
		if (contactList[i])
			delete contactList[i];
	}
}

void pWorld::resolve_collsion(pCollision_manifold* contact)
{
	pBody* BodyA = contact->BodyA;
	pBody* BodyB = contact->BodyB;
	pvec2 normal = contact->normal;
	f32 depth = contact->depth;


	pvec2 relativeVelocity = pmath::sub(BodyB->linearvelocity, BodyA->linearvelocity);
	f32 e = pmath::min(BodyA->restitution, BodyB->restitution);

	if (pmath::dot(relativeVelocity, normal) < 0)
		return;

	f32 j = -(1.0f + e) * pmath::dot(relativeVelocity, normal);
	j /= (BodyA->invMass) + (BodyB->invMass);

	BodyA->linearvelocity -= pmath::scale(normal, j * BodyA->invMass);
	BodyB->linearvelocity += pmath::scale(normal, j * BodyB->invMass);
}