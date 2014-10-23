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

#ifndef APOC_PARTICLES_EMITTER_H
#define APOC_PARTICLES_EMITTER_H

#include <Apoc/Math/Vector.h>
#include <Apoc/Video/OpenGL.h>
#include <Apoc/Entity/Texture.h>
#ifdef ENABLE_OPENCL
#include <Apoc/Compute/Compute.h>
#include <Apoc/Compute/Kernel.h>
#endif

#include <vector>

using namespace std;

/**
 * \brief Represents particle emitters.
 */
class Emitter
{
public:
	/**
	 * \brief A structure describing an emitted particle.
	 */
	struct Particle
	{
		/**
		 * \brief Current position of the center of the particle.
		 */
		Vector pos;

		/**
		 * \brief The current velocity of the particle.
		 */
		Vector velocity;

		/**
		 * \brief Acceleration caused by stuff like gravity.
		 */
		Vector acceleration;

		/**
		 * \brief The spawn time of the particle, as returned by SDL_GetTicks().
		 */
		int spawnTime;

		/**
		 * \brief Time of last update.
		 */
		int lastUpdate;

		/**
		 * \brief Age to live.
		 */
		int ageToLive;

		/**
		 * \brief Origin to go back to.
		 */
		Vector origin;

		/**
		 * \brief Starting velocity.
		 */
		Vector startVelocity;
	} __attribute__ ((packed));

private:
	bool shouldRemove;
	vector<Particle> particles;
	GLuint vao;
	GLuint vboMesh;
	GLuint vboParticles;
	int spawnDT;
	int emitterSpawnTime;
	Texture *tex;
#ifdef ENABLE_OPENCL
	cl_mem memParticles;
	Kernel *knUpdate;
#endif

protected:
	/**
	 * \brief Update the particles in software.
	 * \param part The particle to update.
	 *
	 * Given a reference to a particle, this function shall move it according to some logic.
	 * By default, this function shall apply the velocity and acceleration.
	 */
	virtual void updateInSoftware(Particle &part);

	/**
	 * \brief Mark the emitter for removal.
	 */
	virtual void markForRemoval();

public:
	/**
	 * \brief Construct an emitter.
	 * \param count The number of particles.
	 * \param origin The position at which to spawn particles.
	 * \param minVelocity The minimum velocity in all dimensions of newly-spawned particles.
	 * \param maxVelocity The maximum velocity in all dimensions of newly-spawned particles.
	 * \param acceleration The acceleration applied to all particles.
	 * \param minAge The minimum time the particle shall live.
	 * \param maxAge The maximum time the particle shall live.
	 * \param texName The name of the texture to use for the particles.
	 */
	Emitter(int count, Vector origin, Vector minVelocity, Vector maxVelocity, Vector acceleration, int minAge, int maxAge,
			const char *texName);

	/**
	 * \brief Update the particles.
	 *
	 * It will automatically decide whether to do a software-based update or an OpenCL update.
	 */
	void update();

	/**
	 * \brief Render the particles.
	 */
	void render();

	friend class World;
};

#endif
