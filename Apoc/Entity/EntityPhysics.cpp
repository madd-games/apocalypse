/*
	Copyright (c) 2014, Madd Games.
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	
	* Redistributions of source code must retain the above copyright notice, this
	  list of conditions and the following disclaimer.
	
	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Apoc/Entity/EntityPhysics.h>
#include <SDL.h>
#include <Apoc/Entity/World.h>

EntityPhysics::EntityPhysics(Model::ObjDef *defs, float mass)
	: Entity(defs), velocity(0, 0, 0, 0), phMass(mass), angularVelocity(0, 0, 0, 0),
	  restitution(0.0)
{
	phTimer = SDL_GetTicks();
};

void EntityPhysics::setRestitution(float rest)
{
	restitution = rest;
};

void EntityPhysics::update()
{
	int dt = SDL_GetTicks() - phTimer;
	World::ActFields(this, dt);
	Entity *collide = move(velocity * dt);
	if (collide != NULL)
	{
		EntityPhysics *phe = dynamic_cast<EntityPhysics*>(collide);
		if (phe != NULL)
		{
			// TODO: make the MTV more precise.
			Vector mtv = (phe->getCenter() - getCenter()).normalize() * velocity.length() * (-dt);
			float im1 = 1 / phMass;
			float im2 = 1 / phe->getMass();

			Vector v = velocity - phe->getVelocity();
			float vn = v.dot(mtv.normalize());

			float i = (-(1.0f + restitution) * vn) / (im1 + im2) / 2;
			Vector impulse = mtv * i;

			applyImpulse(impulse);
			phe->applyImpulse(-impulse);
		}
		else
		{
			velocity = velocity * (-restitution);
		};
	};
	rotate(angularVelocity.x(), angularVelocity.y(), angularVelocity.z());
	phTimer = SDL_GetTicks();
};

float EntityPhysics::getMass()
{
	return phMass;
};

void EntityPhysics::applyImpulse(Vector force)
{
	velocity = velocity + (1/phMass) * force;
};

Vector EntityPhysics::getVelocity()
{
	return velocity;
};

Vector EntityPhysics::getCenter()
{
	return getModelMatrix()[3];
};
