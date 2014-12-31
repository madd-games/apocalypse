/*
	Copyright (c) 2014-2015, Madd Games.
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

#ifndef APOC_PHYSICS_FIELD_WIND_H
#define APOC_PHYSICS_FIELD_WIND_H

#include <Apoc/Physics/Field.h>
#include <Apoc/Math/Vector.h>

/**
 * \brief The wind field.
 *
 * The wind field applies a single force to all objects in the world, and can also be changed
 * dynamically. Unlike FieldGravity, it will applies a constant <i>force</i> (mass * acceleration),
 * rather than a constant acceleration - meaning that heavier objects are less affected by the
 * wind.
 */
class FieldWind : public Field
{
private:
	Vector force;

public:
	/**
	 * \brief Constructor.
	 * \param force The initial force of the wind.
	 */
	FieldWind(Vector force);

	/**
	 * \brief Change the force of the wind.
	 */
	void setForce(Vector force);

	virtual void actOn(EntityPhysics *phe, int dt);
};

#endif
