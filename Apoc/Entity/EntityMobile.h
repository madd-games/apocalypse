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

#ifndef APOC_ENTITY_MOBILE_H
#define APOC_ENTITY_MOBILE_H

#include <Apoc/Entity/Entity.h>
#include <Apoc/Video/Camera.h>
#include <Apoc/Math/Vector.h>

/**
 * \brief Mobile entities ("mobs").
 */
class EntityMobile : public Entity, public Camera
{
private:
	Vector eyePos;
	Vector eyeRef;
	float theta, phi;

public:
	bool forward, backwards, left, right;

	/**
	 * \brief Constructor.
	 * \param defs The object definitions, passed to the constructor of Entity.
	 * \param eye The position of the eye in model space.
	 * \param ref The point of reference in model space.
	 */
	EntityMobile(Model::ObjDef *defs, Vector eye, Vector ref);

	virtual Vector getEye();
	virtual Vector getRef();
	virtual Vector getUpVector();

	/**
	 * \brief Moves the camera.
	 */
	void moveCamera(float deltaTheta, float deltaPhi);

	/**
	 * \brief Update the position of the entity according to params.
	 */
	virtual void update();
};

#endif
