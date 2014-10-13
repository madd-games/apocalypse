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

#ifndef APOC_ENTITY_WORLD_H
#define APOC_ENTITY_WORLD_H

#include <Apoc/Entity/Entity.h>
#include <Apoc/Video/Camera.h>
#include <Apoc/Particles/Emitter.h>
#include <vector>

using namespace std;

/**
 * \brief Describes the world.
 */
class World
{
private:
	static vector<Entity*> entities;
	static vector<Entity*> addQueue;
	static vector<Emitter*> emitters;
	static Camera *camera;

public:
	/**
	 * \brief Add an entity to the world.
	 *
	 * It will appear in the entity list on the next update.
	 */
	static void addEntity(Entity *ent);

	/**
	 * \brief Updates the world.
	 *
	 * This function basically calls the update() method of every Entity.
	 */
	static void update();

	/**
	 * \brief Render the world.
	 */
	static void render(bool setMatrix = true);

	/**
	 * \brief Render the particles.
	 */
	static void renderParticles();

	/**
	 * \brief Set the Camera.
	 */
	static void setCamera(Camera *camera);

	/**
	 * \brief Add an emitter to the world.
	 */
	static void addEmitter(Emitter *emitter);

	friend class Entity;
};

#endif
