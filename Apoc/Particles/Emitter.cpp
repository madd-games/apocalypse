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

#include <Apoc/Particles/Emitter.h>
#include <Apoc/Video/RenderHandler.h>
#include <Apoc/Video/OpenGL.h>
#include <Apoc/Utils/Utils.h>
#include <stddef.h>
#include <random>

using namespace std;

extern RenderHandler *apocRenderHandler;

#ifdef ENABLE_OPENCL
extern cl_context p_context;
#endif

Emitter::Emitter(int count, Vector origin, Vector minVelocity, Vector maxVelocity, Vector acceleration, int minAge, int maxAge,
		const char *texName)
	: spawnDT(10), shouldRemove(false)
{
	tex = Texture::Get(texName);
	emitterSpawnTime = (int) SDL_GetTicks();
	static const GLint meshData[] = {
		1, 2, 3,
		4, 1, 3
	};

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.0,1.0);

	int i;
	for (i=0; i<count; i++)
	{
		Particle part;
		part.pos = origin;
		part.velocity = Vector(
			minVelocity.x() + distribution(generator) * (maxVelocity.x()-minVelocity.x()),
			minVelocity.y() + distribution(generator) * (maxVelocity.y()-minVelocity.y()),
			minVelocity.z() + distribution(generator) * (maxVelocity.z()-minVelocity.z()),
		0.0);
		part.acceleration = acceleration;
		part.acceleration.w() = 0.0;
		part.ageToLive = minAge + rand() % (maxAge-minAge);
		part.spawnTime = (int) SDL_GetTicks() - rand() % part.ageToLive;
		part.lastUpdate = (int) SDL_GetTicks();
		part.origin = origin;
		part.startVelocity = part.velocity;
		particles.push_back(part);
	};

	GLint attrPartPosIndex = apocRenderHandler->getAttrLocation("inPartPosIndex");
	GLint attrVertex = apocRenderHandler->getAttrLocation("inVertex");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vboMesh);
	glBindBuffer(GL_ARRAY_BUFFER, vboMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int)*6, meshData, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(attrPartPosIndex);
	glVertexAttribIPointer(
		attrPartPosIndex,
		1,
		GL_INT,
		0,
		(void*)0
	);
	glVertexAttribDivisor(attrPartPosIndex, 0);

	glGenBuffers(1, &vboParticles);
	glBindBuffer(GL_ARRAY_BUFFER, vboParticles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*count, &particles[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(attrVertex);
	glVertexAttribPointer(
		attrVertex,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Particle),
		(void*)offsetof(Particle, pos)
	);
	glVertexAttribDivisor(attrVertex, 1);

#ifdef ENABLE_OPENCL
	if (IsComputeEnabled())
	{
		cl_int error;
		memParticles = clCreateFromGLBuffer(p_context, CL_MEM_READ_WRITE, vboParticles, &error);
		if (error != CL_SUCCESS)
		{
			ApocFail("Could not create an OpenCL buffer for an emitter!");
		};
		knUpdate = new Kernel("ApocUpdateEmitter");
		knUpdate->setArg(0, sizeof(cl_mem), &memParticles);
	};
#endif
};

void Emitter::updateInSoftware(Particle &part)
{
	int dt = ((int)SDL_GetTicks()) - part.lastUpdate;
	int age = ((int)SDL_GetTicks()) - part.spawnTime;
	if (age > part.ageToLive)
	{
		part.pos = part.origin;
		part.velocity = part.startVelocity;
		part.spawnTime = (int) SDL_GetTicks();
	}
	else
	{
		int i;
		for (i=0; i<dt; i++)
		{
			part.pos = part.pos + part.velocity;
			part.velocity = part.velocity + part.acceleration;
		};
	};
	part.lastUpdate = (int) SDL_GetTicks();
};

void Emitter::markForRemoval()
{
	shouldRemove = true;
};

void Emitter::update()
{
	int duration = (int) SDL_GetTicks() - emitterSpawnTime;
	size_t count = (size_t) duration / spawnDT;
	if (count > particles.size()) count = particles.size();

#ifdef ENABLE_OPENCL
	if (IsComputeEnabled())
	{
		int icount = (int) count;
		int time = (int) SDL_GetTicks();
		knUpdate->setArg(1, sizeof(int), &count);
		knUpdate->setArg(2, sizeof(int), &time);
		AcquireGL(1, &memParticles);
		knUpdate->execute(count);
		ReleaseGL(1, &memParticles);
	}
	else
	{
#endif
	size_t i;
	for (i=0; i<count; i++)
	{
		updateInSoftware(particles[i]);
	};

	glBindBuffer(GL_ARRAY_BUFFER, vboParticles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*particles.size(), &particles[0], GL_DYNAMIC_DRAW);
#ifdef ENABLE_OPENCL
	};		// else
#endif
};

void Emitter::render()
{
	apocRenderHandler->bindDefaultTextures();
	glActiveTexture(GL_TEXTURE0);
	tex->bind();

	int duration = (int) SDL_GetTicks() - emitterSpawnTime;
	size_t count = (size_t) duration / spawnDT;
	if (count > particles.size()) count = particles.size();

	glBindVertexArray(vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
};
