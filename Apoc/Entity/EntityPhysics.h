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

#ifndef APOC_ENTITY_PHYSICS_H
#define APOC_ENTITY_PHYSICS_H

#include <Apoc/Entity/Entity.h>
#include <Apoc/Math/Vector.h>

/**
 * \brief Base class for entities which experience physics.
 */
class EntityPhysics : public Entity
{
private:
	Vector velocity;
	float phMass;
	Vector angularVelocity;
	float restitution;

protected:
	/**
	 * \brief Sets the coefficient of restitution for this entity.
	 * \param rest The coefficient of restitution.
	 *
	 * The coefficient of restitution is the ratio between the speed after collision
	 * and the speed before collision.
	 */
	void setRestitution(float rest);

public:
	/**
	 * \brief (deprecated) Construct the entity.
	 * \sa Entity
	 * \deprecated Use the other constructor.
	 */
	EntityPhysics(Model::ObjDef *defs, float mass = 1.0);

	/**
	 * \brief Construct the entity.
	 * \sa Entity
	 */
	EntityPhysics(string entname, float mass = 1.0);

	virtual void update();

	/**
	 * \brief Returns the mass of this physics entity.
	 */
	float getMass();

	/**
	 * \brief Applies an impulse to the entity.
	 * \param impulse The force to apply (<i><b>F</b> = m<b>a</b></i>).
	 */
	void applyImpulse(Vector force);

	/**
	 * \brief Returns the velocity of this entity.
	 */
	Vector getVelocity();

	/**
	 * \brief Returns the center of this entity.
	 */
	Vector getCenter();
};

#endif
