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

#include <Apoc/Entity/World.h>
#include <Apoc/Video/RenderHandler.h>
#include <Apoc/Game.h>
#include <math.h>

#ifdef ENABLE_OPENAL
#include <AL/alut.h>
#endif

#ifndef M_PI
#define	M_PI 3.14159265359
#endif

extern RenderHandler *apocRenderHandler;
extern Game *apocGameInstance;

vector<Entity*> World::entities;
vector<Entity*> World::addQueue;
vector<Emitter*> World::emitters;
vector<Field*> World::fields;
Camera *World::camera = NULL;

void World::ActFields(EntityPhysics *phe, int dt)
{
	vector<Field*>::iterator it;
	for (it=fields.begin(); it!=fields.end(); ++it)
	{
		(*it)->actOn(phe, dt);
	};
};

void World::addEntity(Entity *entity)
{
	addQueue.push_back(entity);
};

void World::update()
{
	vector<Entity*>::iterator it;
	for (it=addQueue.begin(); it!=addQueue.end(); ++it)
	{
		entities.push_back(*it);
	};
	addQueue.clear();

	it = entities.begin();
	while (it != entities.end())
	{
		(*it)->update();
		if ((*it)->shouldRemove)
		{
			delete (*it);
			it = entities.erase(it);
		}
		else
		{
			it++;
		};
	};
};

void World::render(bool setMatrix)
{
	if (setMatrix)
	{
		GLint uViewMatrix = apocRenderHandler->getUniformLocation("uViewMatrix");
		GLint uEyePos = apocRenderHandler->getUniformLocation("uEyePos");
		Vector eye = Vector::Origin;
		if (camera != NULL)
		{
			eye = camera->getEye();
		};
		glUniform4fv(uEyePos, 1, &eye[0]);

		Matrix viewMatrix;
		if (camera != NULL)
		{
			viewMatrix = Matrix::LookAt(
				camera->getEye(),
				camera->getUpVector(),
				camera->getRef()
			);

#ifdef ENABLE_OPENAL
			Vector pos = camera->getEye();
			Vector orient = camera->getRef() - camera->getEye();

			alListenerfv(AL_POSITION, &pos[0]);
			alListenerfv(AL_ORIENTATION, &orient[0]);
#endif
		}
		else
		{
			viewMatrix = Matrix::LookAt(
				Vector(0.0, 0.0, 0.0),
				Vector(0.0, 1.0, 0.0),
				Vector(0.0, 0.0, 1.0)
			);
		};

		GLint uProjMatrix = apocRenderHandler->getUniformLocation("uProjectionMatrix");
		int width, height;
		apocGameInstance->getScreenSize(width, height);
		Matrix projMatrix = Matrix::Perspective((float)width, (float)height, 1.0, 1000.0, 60.0*M_PI/180.0);
		glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(uProjMatrix, 1, GL_FALSE, &projMatrix[0][0]);
	};

	vector<Entity*>::iterator it;
	for (it=entities.begin(); it!=entities.end(); ++it)
	{
		(*it)->renderObjects();
	};
};

void World::renderParticles()
{
	vector<Emitter*>::iterator eit = emitters.begin();
	while (eit != emitters.end())
	{
		(*eit)->update();
		if ((*eit)->shouldRemove)
		{
			delete (*eit);
			eit = emitters.erase(eit);
		}
		else
		{
			(*eit)->render();
			++eit;
		};
	};
};

void World::setCamera(Camera *camera)
{
	World::camera = camera;
};

void World::addEmitter(Emitter *emitter)
{
	emitters.push_back(emitter);
};

void World::addField(Field *field)
{
	fields.push_back(field);
};

vector<Entity*> World::findWithinRadius(Vector pos, float r)
{
	vector<Entity*> output;
	vector<Entity*>::iterator it;

	for (it=entities.begin(); it!=entities.end(); ++it)
	{
		Vector center = (*it)->getModelMatrix()[3];
		if ((pos-center).length() <= r)
		{
			output.push_back(*it);
		};
	};

	return output;
};
